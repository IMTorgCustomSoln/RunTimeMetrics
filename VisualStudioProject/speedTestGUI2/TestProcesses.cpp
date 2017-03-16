#include "StdAfx.h"
#include "TestProcesses.h"
#include <psapi.h>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <strsafe.h>

#pragma comment(lib, "psapi.lib")
#define MAX_PATH 260

TestProcesses::TestProcesses(void)
{
	ChildStd_IN_Rd = NULL;
	ChildStd_IN_Wr = NULL;
	antipattern = 1;
}

void TestProcesses::setStatisticDirectoryName(const std::string& name)
{
	testPathLength(name);
	statInfoDirectoryName = name;
}

void TestProcesses::setDirectoryName(const std::string& dir)
{
	testPathLength(dir);
   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.
   directoryName = copyFromString(dir);
   directoryNameWithout = copyFromString(dir.substr(0,dir.length()-2));
}

void TestProcesses::testPathLength(const std::string& dir)
{
	// Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.
   if (dir.length() > MAX_PATH)
   {
      std::cout << "\nDirectory path is too long.\n";
      exit(-1);
   }

}

void TestProcesses::findFilesFromDirectory(TCHAR* delim)
{
   WIN32_FIND_DATA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError = 0;
   // Find the first file in the directory.
   hFind = FindFirstFile(directoryName, &ffd);
   if (INVALID_HANDLE_VALUE == hFind) 
   {
      printf("findFilesFromDirectory");
      return ;//dwError;
   }

   // List all the files in the directory and store them in execFiles member.
   TCHAR* token = NULL;
   do
   {
      if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
		 token = wcsstr(ffd.cFileName,delim);
		 if(token != NULL)
		 {
			 size_t ln = wcslen(ffd.cFileName)+1;
			 TCHAR* nm = new TCHAR[ln];
			 wcscpy(nm, ffd.cFileName);
			 //std::copy(ffd.cFileName,ffd.cFileName+ln,nm);
			 execFiles.insert( std::pair<TCHAR*,TCHAR*>(nm,nm) );
		 }
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);

   FindClose(hFind);
}

//Main loop for files to test them
void TestProcesses::evaluateAllFiles()
{
	//Get parameters from user
	for(std::map<TCHAR*,TCHAR*>::iterator it = execFiles.begin(); it != execFiles.end(); ++it)
		it->second = getParameterFromUser(it->first);

	for(int i = 0; i < iterations; ++i)
	{
		for(std::map<TCHAR*,TCHAR*>::iterator it = execFiles.begin(); it != execFiles.end(); ++it)
		{
			FILE *fp;
			char buffer[10];
			_itoa_s(i+1,buffer,10);
			std::string path = statInfoDirectoryName + "\\" + std::string(buffer) + ".txt";
			if((fp=fopen( path.c_str(), "w")) == NULL) // Another file is created with name like '#iteration.txt'
			{
				printf("Error opening file\n");
				exit(1);
			}
			evaluateFile(it->first, it->second, fp);
			fclose(fp);
		}
	}
	cleanBuffers();
}

//Get parameters for 'file' from user
TCHAR* TestProcesses::getParameterFromUser(const TCHAR* file)
{
	wprintf(TEXT("Input Number of parameter for %s:"), file);
	int numbPram;
	std::cin >> numbPram;
	if(numbPram == 0)
		return NULL;

	std::string parameters;
	for(int i = 0; i < numbPram; ++i)
	{
		std::cout << "Input " << i+1 << "th parameter: ";
		std::string s;
		if(i == 0)
			std::cin.get();//eats 'Enter' after entering number of parameters
		std::getline(std::cin, s);
		parameters.append(s + "\n");
	}
	return copyFromString(parameters);
}

void TestProcesses::evaluateFile(TCHAR* fileName, TCHAR* param, FILE *fp)
{
	TCHAR* fullName = getFullFileName(fileName,directoryNameWithout);

	SECURITY_ATTRIBUTES saAttr; 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	// Create a pipe for the child process's STDIN. 
	if (! CreatePipe(&ChildStd_IN_Rd, &ChildStd_IN_Wr, &saAttr, 0)) 
		ErrorExit(TEXT("Stdin CreatePipe")); 
	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if ( ! SetHandleInformation(ChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
		ErrorExit(TEXT("Stdin SetHandleInformation")); 

   // Create the child process and write into its stdin.
	if(param != NULL)
		WriteToPipe(param);

   ULONG64 before = getNumberTicksAllProcesses();
   HANDLE childProcess = (CreateChildProcess(fullName)).hProcess;
   WaitForSingleObject(childProcess, INFINITE);
   ULONG64 after = getNumberTicksAllProcesses();
   ULONG64 TickBetween = after-before;

   if( childProcess != NULL )
   {	   
	   SYSTEMTIME CreationSystemTime, ExitSystemTime, KernelSystemTime, UserSystemTime;
	   FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	   ULONG64 CycleTime;
	   double CPUusage;
	   PROCESS_MEMORY_COUNTERS MemoryCounter;

	   if ( !GetProcessTimes( childProcess, &CreationTime,&ExitTime,&KernelTime,&UserTime ) )
		   ErrorExit(TEXT("GetProcessTimes"));
	   if ( !QueryProcessCycleTime( childProcess, &CycleTime) )
		   ErrorExit(TEXT("QueryProcessCycleTime"));
	   if ( !GetProcessMemoryInfo( childProcess, &MemoryCounter, sizeof(MemoryCounter)) )
		   ErrorExit(TEXT("GetProcessMemoryInfo"));

	   ULONGLONG UserSpentTime = subtractTime(ExitTime,CreationTime);
	   CPUusage = 100.0 * CycleTime/ (CycleTime + TickBetween);
	   FileTimeToSystemTime( &CreationTime, &CreationSystemTime );
	   FileTimeToSystemTime( &ExitTime, &ExitSystemTime );
	   FileTimeToSystemTime( &KernelTime, &KernelSystemTime );
	   FileTimeToSystemTime( &UserTime, &UserSystemTime );
	   
	   _fwprintf_p(fp,L"File: %s\n",fileName);
	   _fwprintf_p(fp,L"The process creation time is: %02d:%02d:%02d:%02d\n", CreationSystemTime.wHour, CreationSystemTime.wMinute, 
		   CreationSystemTime.wSecond, CreationSystemTime.wMilliseconds);
	   _fwprintf_p(fp,L"The process exit time is: %02d:%02d:%02d:%02d\n", ExitSystemTime.wHour, ExitSystemTime.wMinute, 
		   ExitSystemTime.wSecond, ExitSystemTime.wMilliseconds);
	   _fwprintf_p(fp,L"The process time in kernel mode is: %02d:%02d:%02d:%02d\n", KernelSystemTime.wHour, KernelSystemTime.wMinute, 
	   	   KernelSystemTime.wSecond, KernelSystemTime.wMilliseconds);
	   _fwprintf_p(fp,L"The process time in user mode is: %02d:%02d:%02d:%02d\n", UserSystemTime.wHour, UserSystemTime.wMinute, 
	   	   UserSystemTime.wSecond, UserSystemTime.wMilliseconds);
	   _fwprintf_p(fp,L"The process time running is: %f ms\n", ((float)UserSpentTime/10000));
	   _fwprintf_p(fp,L"CPU clock cycles: %d\n", CycleTime);
	   _fwprintf_p(fp,L"CPU usage is: %f percent\n\n", CPUusage);
	   _fwprintf_p(fp,L"Memory information\n");
	   _fwprintf_p( fp,L"The number of page faults: %d bytes\n", MemoryCounter.PageFaultCount );
	   _fwprintf_p( fp,L"The peak working set size: %d bytes\n", MemoryCounter.PeakWorkingSetSize );
	   _fwprintf_p( fp,L"The current working set size: %d bytes\n", MemoryCounter.WorkingSetSize );
	   _fwprintf_p( fp,L"The peak paged pool usage: %d bytes\n", MemoryCounter.QuotaPeakPagedPoolUsage );
	   _fwprintf_p( fp,L"The current paged pool usage: %d bytes\n", MemoryCounter.QuotaPagedPoolUsage );
	   _fwprintf_p( fp,L"The peak nonpaged pool usage: %d bytes\n", MemoryCounter.QuotaPeakNonPagedPoolUsage );
	   _fwprintf_p( fp,L"The current nonpaged pool usage: %d bytes\n", MemoryCounter.QuotaNonPagedPoolUsage );
	   _fwprintf_p( fp,L"The Commit Charge value: %d bytes\n", MemoryCounter.PagefileUsage ); 
	   _fwprintf_p( fp,L"The peak value of the Commit Charge: %d bytes\n\n\n", MemoryCounter.PeakPagefileUsage );
   }

   CloseHandle(ChildStd_IN_Rd);
   CloseHandle(childProcess);
}

TestProcesses::Results TestProcesses::evaluateFile(const std::string& fullFileNameC, const std::string& paramC, Priority prior)
{
	TCHAR* fullFileName, *param;
	fullFileName = copyFromString(fullFileNameC);
	param = copyFromString(paramC);

	SECURITY_ATTRIBUTES saAttr; 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	// Create a pipe for the child process's STDIN. 
	if (! CreatePipe(&ChildStd_IN_Rd, &ChildStd_IN_Wr, &saAttr, 0)) 
		ErrorExit(TEXT("Stdin CreatePipe")); 
	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if ( ! SetHandleInformation(ChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
		ErrorExit(TEXT("Stdin SetHandleInformation")); 

   // Create the child process and write into its stdin.
	if(param != NULL)
		WriteToPipe(param);


   PROCESS_INFORMATION childProcess = CreateChildProcess(fullFileName, CREATE_SUSPENDED);
   DWORD ProcessAffinityMask/* = NULL*/, SystemAffinityMask/* = NULL*/;
   SetPriorityClass(childProcess.hProcess, (DWORD)prior);
   GetProcessAffinityMask(childProcess.hProcess, &ProcessAffinityMask, &SystemAffinityMask);
   DWORD newpatt = (antipattern & ProcessAffinityMask);
   SetProcessAffinityMask(childProcess.hProcess, newpatt);

   ULONG64 before = getNumberTicksAllProcesses();
   ResumeThread(childProcess.hThread);

   WaitForSingleObject(childProcess.hProcess, INFINITE);
   ULONG64 after = getNumberTicksAllProcesses();
   ULONG64 TickBetween = after-before;
   
   TestProcesses::Results r;
   if( childProcess.hProcess != NULL )
   {	   
	   SYSTEMTIME CreationSystemTime, ExitSystemTime, KernelSystemTime, UserSystemTime;
	   FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	   ULONG64 CycleTime;
	   double CPUusage;
	   PROCESS_MEMORY_COUNTERS MemoryCounter;

	   if ( !GetProcessTimes( childProcess.hProcess, &CreationTime,&ExitTime,&KernelTime,&UserTime ) )
		   ErrorExit(TEXT("GetProcessTimes"));
	   if ( !QueryProcessCycleTime( childProcess.hProcess, &CycleTime) )
		   ErrorExit(TEXT("QueryProcessCycleTime"));
	   if ( !GetProcessMemoryInfo( childProcess.hProcess, &MemoryCounter, sizeof(MemoryCounter)) )
		   ErrorExit(TEXT("GetProcessMemoryInfo"));

	   ULONGLONG UserSpentTime = subtractTime(ExitTime,CreationTime);
	   CPUusage = 100.0 * CycleTime/ (CycleTime + TickBetween);

	   FileTimeToSystemTime( &CreationTime, &CreationSystemTime );
	   FileTimeToSystemTime( &ExitTime, &ExitSystemTime );
	   FileTimeToSystemTime( &KernelTime, &KernelSystemTime );
	   FileTimeToSystemTime( &UserTime, &UserSystemTime );
	   
	   r.CreationTime = CreationSystemTime;
	   r.ExitTime = ExitSystemTime;
	   r.KernelTime = KernelSystemTime;
	   r.UserTime = UserSystemTime;
	   r.LifeTime = ((float)UserSpentTime/10000);
	   r.CycleTime = CycleTime;
	   r.CPUusage = CPUusage;
	   r.MemoryCounter = MemoryCounter;
   }

   delete[] fullFileName;
   delete[] param;
   CloseHandle(ChildStd_IN_Rd);
   CloseHandle(childProcess.hProcess);
   return r;
}

// Create a child process that uses the previously created pipes for STDIN and STDOUT.
PROCESS_INFORMATION TestProcesses::CreateChildProcess(TCHAR* name, DWORD addp)
{ 
   //LPTSTR szCmdline = _tcsdup(name);
   PROCESS_INFORMATION piProcInfo; 
   STARTUPINFO siStartInfo;
   BOOL bSuccess = FALSE; 
 
// Set up members of the PROCESS_INFORMATION structure. 
   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// Set up members of the STARTUPINFO structure. 
// This structure specifies the STDIN and STDOUT handles for redirection.
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.hStdInput = ChildStd_IN_Rd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
 
// Create the child process. 
   bSuccess = CreateProcess(NULL, 
      name,     // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      addp,          // creation flags  
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION 
   
   // If an error occurs, exit the application. 
   if ( ! bSuccess ) 
      ErrorExit(TEXT("CreateProcess"));
   else 
	   return piProcInfo;
}

// write its contents to the pipe for the child's STDIN.
void TestProcesses::WriteToPipe(TCHAR* whatToWrite) 
{ 
   DWORD dwRead = wcslen(whatToWrite), dwWritten;
   
   for(int i = 0; i < dwRead; ++i)
	   BOOL fl = WriteFile(ChildStd_IN_Wr, &whatToWrite[i], 1, &dwWritten, NULL);
   
   // Close the pipe handle so the child process stops reading. 
   if ( ! CloseHandle(ChildStd_IN_Wr) ) 
     ErrorExit(TEXT("StdInWr CloseHandle")); 
} 

TCHAR* TestProcesses::copyFromString(const std::string& from)
{ 
   size_t len = from.size()+1;
   TCHAR* to = new TCHAR[len];
   memset(to, '\0', len*sizeof(TCHAR));
   std::copy(from.begin(),from.end(),to);
   to[len-1] = 0;
   return to;
}

void TestProcesses::printFileNames()
{
	wprintf(TEXT("Curent Directory is <%s>\n"),directoryName);
	//for(int i = 0; i < execFiles.size(); ++i)
	//	_tprintf(TEXT("%dth exec file <%s>\n"),i+1,execFiles.at(i));
}

// Format a readable error message, display a message box, 
// and exit from the application.
void TestProcesses::ErrorExit(PTSTR lpszFunction) 
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}

//Concate file name with its path and return full path to file
TCHAR* TestProcesses::getFullFileName(TCHAR* fileName, TCHAR* directoryNameWithout)
{
   size_t fnLen = wcslen(fileName);
   size_t dnLen = wcslen(directoryNameWithout); 
   size_t full = fnLen + dnLen  + 2;
   TCHAR* fullFileName = new TCHAR[full];

   wcscpy_s(fullFileName, full, directoryNameWithout);
   fullFileName[dnLen] = L'\\';
   wcscpy_s(fullFileName+dnLen+1, full, fileName);
   fullFileName[full-1] = 0;
   return fullFileName;
}

ULONG64 TestProcesses::getNumberTicksAllProcesses()
{
	// Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		ErrorExit(TEXT("EnumProcesses")); 

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
	ULONG64 CycleTime;
	ULONG64 AllCycleTimes;
	ZeroMemory( &AllCycleTimes, sizeof(ULONG64) );
    for ( unsigned int i = 0; i < cProcesses; i++ )
    {
		if( aProcesses[i] != 0 )
		{
			// Get a handle to the process.
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
										PROCESS_VM_READ,
										FALSE, aProcesses[i] );
			// Get the process CPU ticks.
			if (NULL != hProcess )
			{
				if ( !QueryProcessCycleTime( hProcess, &CycleTime) )
					ErrorExit(TEXT("QueryProcessCycleTime"));
				AllCycleTimes += CycleTime;
			}
		
			// Release the handle to the process.
			CloseHandle( hProcess );
		}
    }
	return AllCycleTimes;
}


DWORD TestProcesses::getProcessorsMask()//2147483647;//011111...
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	if(sysinfo.dwNumberOfProcessors > 1)
	{
		DWORD commonPattern = 4294967295;//find all common-used processors
		antipattern = 1;

		// Get the list of process identifiers.
		std::vector<HANDLE> hProcesses = getProcesses();

		DWORD ProcessAffinityMask, SystemAffinityMask;
		for ( unsigned int i = 0; i < hProcesses.size(); i++ )
		{
				if (NULL != hProcesses[i] )
				{
					GetProcessAffinityMask(hProcesses[i], &ProcessAffinityMask, &SystemAffinityMask);
					commonPattern &= ProcessAffinityMask;
				}
		}
		while(commonPattern & antipattern)//find first common-used processor
			antipattern <<= 1 ;

		for ( unsigned int i = 0; i < hProcesses.size(); i++ )
			if (NULL != hProcesses[i] )
			{
				SetProcessAffinityMask(hProcesses[i], (~antipattern & ProcessAffinityMask));
				CloseHandle( hProcesses[i] );
			}
    } 

	return antipattern;
}

void TestProcesses::cleanBuffers()
{
	for(std::map<TCHAR*,TCHAR*>::iterator it = execFiles.begin(); it != execFiles.end(); ++it)
	{
		delete[] it->first;
		delete[] it->second;
	}
	execFiles.clear();
}

ULONGLONG TestProcesses::subtractTime(const FILETIME &a, const FILETIME &b)
{
    ULARGE_INTEGER la, lb;
    la.LowPart = a.dwLowDateTime;
    la.HighPart = a.dwHighDateTime;
    lb.LowPart = b.dwLowDateTime;
    lb.HighPart = b.dwHighDateTime;
 
    return la.QuadPart - lb.QuadPart;
}

ULONGLONG TestProcesses::sumTime(const FILETIME &a, const FILETIME &b)
{
    ULARGE_INTEGER la, lb;
    la.LowPart = a.dwLowDateTime;
    la.HighPart = a.dwHighDateTime;
    lb.LowPart = b.dwLowDateTime;
    lb.HighPart = b.dwHighDateTime;
 
    return la.QuadPart + lb.QuadPart;
}

std::vector<HANDLE> TestProcesses::getProcesses()
{
	DWORD aProcesses[1024], cbNeeded;
	std::vector<HANDLE> hProcesses;
	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		ErrorExit(TEXT("EnumProcesses")); 
	hProcesses.resize(cbNeeded / sizeof(DWORD));

	DWORD ProcessAffinityMask, SystemAffinityMask;
	for ( unsigned int i = 0; i < hProcesses.size(); i++ )
	{
		if( aProcesses[i] != 0 )
			// Get a handle to the process.
			hProcesses[i] = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,	FALSE, aProcesses[i] );
	}
	return hProcesses;
}

TestProcesses::~TestProcesses(void)
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	if(sysinfo.dwNumberOfProcessors > 1)
	{
		std::vector<HANDLE> hProcesses = getProcesses();
		DWORD ProcessAffinityMask, SystemAffinityMask;
		for ( unsigned int i = 0; i < hProcesses.size(); i++ )
		{
				if (NULL != hProcesses[i] )
				{
					GetProcessAffinityMask(hProcesses[i], &ProcessAffinityMask, &SystemAffinityMask);
					SetProcessAffinityMask(hProcesses[i], (SystemAffinityMask | ProcessAffinityMask));
				}
			CloseHandle( hProcesses[i] );
		}
	}
}


