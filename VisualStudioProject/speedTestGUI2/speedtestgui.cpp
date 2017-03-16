#include "stdafx.h"
#include "speedtestgui.h"
#include "TestProcesses.h"
//#include "MyString.h"
#include <iostream>
#include <fstream>
#include <QDateTime>
#include <QMessageBox>

speedTestGUI::speedTestGUI(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	//ui.splitter_6->hide();
    //layout()->setSizeConstraint(QLayout::SetFixedSize);
	setWindowTitle("Process Statistic Data");

	//This connects click on pushbuttons to functions which processes the events
    connect(ui.pushButtonStatistFilesDir,SIGNAL(clicked()),this,SLOT(pushButtonStatistFilesDirClicked()));
    connect(ui.pushButtonTestDir,SIGNAL(clicked()),this,SLOT(pushButtonTestDirClicked()));
    connect(ui.pushButtonTestFiles,SIGNAL(clicked()),this,SLOT(pushButtonTestFilesClicked()));

    //Signals and Slots to work with qtreewidget class which represents tree of parameters
    connect(ui.pushButtonAdd,SIGNAL(clicked()),this,SLOT(addItem()));
    connect(ui.pushButtonDelete,SIGNAL(clicked()),this,SLOT(deleteItem()));
    connect(ui.treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
    connect(ui.treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
    connect(ui.checkBoxOneForAll,SIGNAL(stateChanged(int)),this,SLOT(checkBoxChanged(int)));


    //Signals and Slots to work with function which evaluates processes' statistical datas
    connect(ui.pushButtonEvaluate,SIGNAL(clicked()),this,SLOT(evaluateClicked()));

    //Section to customize QTreeWidget
    ui.treeWidget->setColumnCount(3);
    ui.treeWidget->setHeaderLabels(QStringList() << tr("File") << tr("Key(s)") << tr("Priority"));
    //ui->treeWidget->header()->setResizeMode(0,QHeaderView::Stretch);
    //ui->treeWidget->header()->setResizeMode(1,QHeaderView::Stretch);
}

//Sets directory where statistic csv-files  would be stored
void speedTestGUI::pushButtonStatistFilesDirClicked()
{
    if(setDirectory(whereStatistFiles))
        ui.lineEditAddressPathsStatistFiles->setText(whereStatistFiles.absolutePath());
}

//Sets directory where testing exe-files are stored and copies their names to QStringList
void speedTestGUI::pushButtonTestDirClicked()
{
    if(setDirectory(whereTestFiles))
    {
        testFiles = whereTestFiles.entryList(QDir::Files | QDir::Executable);
        setExecFilesLineEdit();
    }
}

//Sets exe-files are stored and copies their names to QStringList
void speedTestGUI::pushButtonTestFilesClicked()
{
    testFiles = QFileDialog::getOpenFileNames(this,
                                        "Select one or more files to test",
                                        QDir::homePath(),
                                        "Executables (*.exe)");
	//Deletes full path from file names
    if(!testFiles.empty())
    {
        int idx = (testFiles.at(0)).lastIndexOf("\\");
        QString dir = (testFiles.at(0)).mid(0,idx);
        whereTestFiles.setPath(dir);

        QStringList::Iterator it = testFiles.begin();
        while(it != testFiles.end())
        {
            it->remove(dir + "\\");
            ++it;
        }
    }
    setExecFilesLineEdit();
}

//Sets directory that user chooses
bool speedTestGUI::setDirectory(QDir& dirToSet)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
        return false;
    dirToSet.setPath(dir);
    return true;
}

//Prints out exe-file names to line on GUI
void speedTestGUI::setExecFilesLineEdit()
{
    QString text;
    for(int i = 0; i < testFiles.size(); ++i)
        text.append(testFiles.at(i) + " ");
    ui.lineEditAddressPathsTestFiles->setText(text);

    addFilesToTree();
}

//Add file names to table to bind them with parameters that user writes
void speedTestGUI::addFilesToTree()
{
    ui.treeWidget->clear();
    QTreeWidgetItem* parent = ui.treeWidget->invisibleRootItem();
    QTreeWidgetItem* item;

    //Add 'One For All' row
    item = new QTreeWidgetItem(parent);
    item->setText(0,tr("One For All"));
    item->setDisabled(true);
	QComboBox* combo = getNewComboBox();
	combo->setEnabled(false);
	ui.treeWidget->setItemWidget(item,2,combo);

    QStringList::Iterator it = testFiles.begin();
    while(it != testFiles.end())
    {
        item = new QTreeWidgetItem(parent);
        item->setText(0,*it);
		ui.treeWidget->setItemWidget(item,2,getNewComboBox());
        ++it;
    }
}

QComboBox* speedTestGUI::getNewComboBox()
{
	QComboBox* combo = new QComboBox(ui.treeWidget);
	QStringList lst;
	lst << "Below Normal" << "Normal" << "Above Normal" << "High" << "Realtime";
	combo->addItems(lst);
	return combo;
}

//Adds parameter to file on table
void speedTestGUI::addKeysToFile(QTreeWidgetItem* parent, QString keyStr)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent);
    item->setText(1,keyStr);
}

//Adds item to table on GUI
void speedTestGUI::addItem()
{
    if(!ui.treeWidget->currentItem())
        return ;

    QTreeWidgetItem* parent;
    if(!ui.treeWidget->currentItem()->parent())
        parent = ui.treeWidget->currentItem();
    else
        parent = ui.treeWidget->currentItem()->parent();

    addKeysToFile(parent,tr("New Key"));
}

//Deletes item from table
void speedTestGUI::deleteItem()
{
    if(ui.treeWidget->currentItem()->parent())
    {
        QTreeWidgetItem* item = ui.treeWidget->currentItem();
        int i = ui.treeWidget->indexOfTopLevelItem(item);
        ui.treeWidget->takeTopLevelItem(i);
        delete item;
    }
}

void speedTestGUI::itemDoubleClicked(QTreeWidgetItem* item,int column)
{
    if(item->parent())
    {
        if(column == 1)
        {
            ui.treeWidget->openPersistentEditor(item,1);
        }else
        {
            ui.treeWidget->closePersistentEditor(item,1);
        }
    }
}

//Processes click 'Evaluate' push button
//Writes info to csv-file
void speedTestGUI::evaluateClicked()
{
	if(testFiles.isEmpty())
	{
		QMessageBox::warning (this, "Warning", "There is no test files");
		return ;
	}
	
	bindFilesParams();
	QDateTime dt = QDateTime::currentDateTime();
	
	//Creates a new file with name Directory that user chose + dd.MM.yyyy.hh.mm.csv
	std::ofstream ofile(whereStatistFiles.absolutePath().toStdString() + "\\" + dt.toString("dd.MM.yyyy.hh.mm").toStdString() + ".csv");

	TestProcesses ob;
	if(ui.checkBoxReassign->isChecked())
		ob.getProcessorsMask();
	TestProcesses::Results res;

	ofile << "File" << ", " << "Iteration" << ", " << "Process creation time" << 
				", " << "Process exit time" << ", " << "Process time in kernel mode" << ", " << "Process time in user mode" << ", "  <<
				"Process time running (ms)" << ", " << "CPU clock cycles" << ", " << "CPU usage" << ", " << "Memory information" << ", " <<
				"Number of page faults (bytes)" << ", " << "Peak working set size (bytes)" << ", " << "Current working set size (bytes)" << ", " <<
				"Peak paged pool usage (bytes)" << ", " << "Current paged pool usage (bytes)" << ", " << "Peak nonpaged pool usage (bytes)" << ", " <<
				"Current nonpaged pool usage (bytes)" << " , " << "Commit Charge value (bytes)" << ", " << "Peak value of the Commit Charge (bytes)" << std::endl;

	for(QStringList::iterator it = testFiles.begin(); it != testFiles.end(); ++it)
	{
		std::string fileName = whereTestFiles.absolutePath().toStdString() + "\\" + (*it).toStdString();
		std::string params;
		Priority prior;
		if(ui.checkBoxOneForAll->isChecked())
		{
			params = filesParam[std::string("OneForAll")].prameters;
			prior = filesParam[std::string("OneForAll")].priority;
		}
		else
		{
			params = filesParam[(*it).toStdString()].prameters;
			prior = filesParam[(*it).toStdString()].priority;
		}
		for(int iter = 1; iter <= ui.spinBox->value(); ++iter)
		{
			res = ob.evaluateFile( fileName, params, prior);
			ofile << (*it).toStdString() << ", " << iter << ", " << res.CreationTime.wHour << ":" << res.CreationTime.wMinute << ":" << res.CreationTime.wSecond << ":" << 
				res.CreationTime.wMilliseconds  << ", " << res.ExitTime.wHour << ":" << res.ExitTime.wMinute << ":" << res.ExitTime.wSecond << ":" << 
				res.ExitTime.wMilliseconds << ", " << res.KernelTime.wHour << ":" << res.KernelTime.wMinute << ":" << res.KernelTime.wSecond << ":" << 
				res.KernelTime.wMilliseconds << ", " << res.UserTime.wHour << ":" << res.UserTime.wMinute << ":" << res.UserTime.wSecond << ":" << 
				res.UserTime.wMilliseconds << ", "  << res.LifeTime << ", " << res.CycleTime << ", " << res.CPUusage << ", " << 
				res.MemoryCounter.PageFaultCount << ", " << res.MemoryCounter.PeakWorkingSetSize << ", " << res.MemoryCounter.WorkingSetSize << ", " <<
				res.MemoryCounter.PeakPagefileUsage << ", " << res.MemoryCounter.QuotaPagedPoolUsage << ", " << res.MemoryCounter.QuotaPeakNonPagedPoolUsage << 
				", " << res.MemoryCounter.QuotaNonPagedPoolUsage << " , " << res.MemoryCounter.PagefileUsage << ", " << res.MemoryCounter.PeakPagefileUsage 
				<< std::endl;
			
		}
	}
	QMessageBox::information(this, "Information", "All Done!");
}

void speedTestGUI::bindFilesParams()
{
	QTreeWidgetItemIterator it(ui.treeWidget);
	if(ui.checkBoxOneForAll->isChecked())
	{
		//Find Field 'One For All'
		while ((*it)->text(0) != tr("One For All"))
			++it;
		std::string strParams;
		ParamsOfProcs params;
		for(int i = 0; i < (*it)->childCount(); ++i)
			strParams.append((*it)->child(i)->text(1).toStdString() + "\n");
		params.prameters = strParams;
		params.priority = getPriorityFromTree(*it);
		filesParam.insert( std::pair<std::string,ParamsOfProcs>("OneForAll",params) );
	}else
	{
		while (*it)
		{
			if( !((*it)->parent()) && ((*it)->text(0) != tr("One For All")) )
			{
				std::string strParams;
				ParamsOfProcs params;
				for(int i = 0; i < (*it)->childCount(); ++i)
					strParams.append((*it)->child(i)->text(1).toStdString() + "\n");
				params.prameters = strParams;
				params.priority = getPriorityFromTree(*it);
				filesParam.insert( std::pair<std::string,ParamsOfProcs>((*it)->text(0).toStdString(),params) );
			}
			++it;
		}
	}
}

Priority speedTestGUI::getPriorityFromTree(QTreeWidgetItem* item)
{
	QTreeWidget* trw = ui.treeWidget;
	QComboBox* combo = (QComboBox*)trw->itemWidget(item,2);
	switch (combo->currentIndex()) //"Below Normal" << "Normal" << "Above Normal" << "High" << "Realtime"
	{
		case 0:
			return BelowNormal;
		case 1:
			return Normal;
		case 2:
			return AboveNormal;
		case 3:
			return High;
		case 4:
			return Realtime;
		default:
			return Normal;
	}
}

void speedTestGUI::checkBoxChanged(int state)
{
    if(state == 0)
    {
        setOneForAllUnable();
    } else if(state == 2)
    {
        setAllUnable();
    }
}

void speedTestGUI::setOneForAllUnable()
{
    setResetAll(false, true);
}

void speedTestGUI::setAllUnable()
{
    setResetAll(true, false);
}

void speedTestGUI::setResetAll(bool allDis, bool oneDis)
{
	QTreeWidget* trw = ui.treeWidget;
    QTreeWidgetItemIterator it(trw);
    while (*it)
    {
        QTreeWidgetItem* item = *it;
        QTreeWidgetItem* parent = item->parent();

        if(parent)
        {
            if(parent->text(0) != tr("One For All"))
                item->setDisabled(allDis);
            else
                item->setDisabled(oneDis);
        }
        else
        {
            if(item->text(0) != tr("One For All"))
			{
                item->setDisabled(allDis);
				trw->itemWidget(item,2)->setDisabled(allDis);
			}
            else 
			{
                item->setDisabled(oneDis);
				trw->itemWidget(item,2)->setDisabled(oneDis);
			}
        }

        ++it;
    }
}

speedTestGUI::~speedTestGUI()
{

}
