#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <psapi.h>
#include "PriorityClass.h"

class TestProcesses
{
	std::string statInfoDirectoryName;//Store directory name where statstic datas stored inside files
	TCHAR* directoryName; //Directory name where testing files are stored
	TCHAR* directoryNameWithout; //Directory name without '\*' where testing files are stored
	std::map<TCHAR*,TCHAR*> execFiles; //Mapping file name to its parameters

	HANDLE ChildStd_IN_Rd;//Handle for stdin of testing file to read from it
	HANDLE ChildStd_IN_Wr;//Handle for stdin of testing file to write to it

	int iterations;
	DWORD antipattern;
public:
	TestProcesses(void);

	void setIterations(int iter) { iterations=iter; };
	void setStatisticDirectoryName(const std::string&);
	void setDirectoryName(const std::string&);
	void findFilesFromDirectory(TCHAR* delim = L".exe");

	void evaluateAllFiles();
	void evaluateFile(TCHAR*, TCHAR*, FILE *);
	PROCESS_INFORMATION CreateChildProcess(TCHAR*, DWORD addp = 0);

	void printFileNames();
	ULONG64 getNumberTicksAllProcesses();

	~TestProcesses(void);

	struct Results
	{
		std::string fileName;
		SYSTEMTIME CreationTime;
		SYSTEMTIME ExitTime;
		SYSTEMTIME KernelTime;
		SYSTEMTIME UserTime;
		float LifeTime;
		ULONG64 CycleTime;
		double CPUusage;
		PROCESS_MEMORY_COUNTERS MemoryCounter;
	};
	Results evaluateFile(const std::string&, const std::string&, Priority prior = Normal);
	DWORD getProcessorsMask();

private:
	TCHAR* getParameterFromUser(const TCHAR*);
	void WriteToPipe(TCHAR*);
	TCHAR* copyFromString(const std::string& from);
	void ErrorExit(PTSTR lpszFunction);

	TCHAR* getFullFileName(TCHAR*, TCHAR*);
	ULONGLONG subtractTime(const FILETIME&, const FILETIME&);
	ULONGLONG sumTime(const FILETIME&, const FILETIME&);
	void testPathLength(const std::string&);
	std::vector<HANDLE> getProcesses();
	void cleanBuffers();
};

