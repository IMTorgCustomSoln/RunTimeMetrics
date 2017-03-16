#ifndef SPEEDTESTGUI_H
#define SPEEDTESTGUI_H

#include <QtGui/QDialog>
#include <QDialog>
#include <QDir>
#include <QTreeWidget>
#include "ui_speedtestgui.h"
#include "PriorityClass.h"

class speedTestGUI : public QDialog
{
	Q_OBJECT

public:
	speedTestGUI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~speedTestGUI();

	public slots:
    void pushButtonStatistFilesDirClicked();
    void pushButtonTestDirClicked();
    void pushButtonTestFilesClicked();
    void addItem();
    void deleteItem();
    void itemDoubleClicked(QTreeWidgetItem*,int);
    void evaluateClicked();
    void checkBoxChanged(int);

private:
    bool setDirectory(QDir& dirToSet);
    void setExecFilesLineEdit();
    void addFilesToTree();
    void addKeysToFile(QTreeWidgetItem*, QString);
    void setOneForAllUnable();
    void setAllUnable();
    void setResetAll(bool, bool);
	void bindFilesParams();
	QComboBox* getNewComboBox();
	Priority getPriorityFromTree(QTreeWidgetItem*);
    
private:
    QStringList testFiles;
    QDir whereStatistFiles;
    QDir whereTestFiles;

	std::map<std::string, ParamsOfProcs> filesParam;

	Ui::Dialog ui;
};

#endif // SPEEDTESTGUI_H
