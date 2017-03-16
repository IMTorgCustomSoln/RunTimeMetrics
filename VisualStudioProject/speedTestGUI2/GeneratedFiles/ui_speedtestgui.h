/********************************************************************************
** Form generated from reading UI file 'speedtestgui.ui'
**
** Created: Tue 25. Sep 16:04:05 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPEEDTESTGUI_H
#define UI_SPEEDTESTGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QSplitter *splitter_4;
    QLabel *label;
    QSplitter *splitter;
    QLineEdit *lineEditAddressPathsStatistFiles;
    QPushButton *pushButtonStatistFilesDir;
    QSplitter *splitter_5;
    QLabel *label_2;
    QSplitter *splitter_3;
    QLineEdit *lineEditAddressPathsTestFiles;
    QPushButton *pushButtonTestDir;
    QPushButton *pushButtonTestFiles;
    QSplitter *splitter_2;
    QSpinBox *spinBox;
    QLabel *label_3;
    QPushButton *pushButtonEvaluate;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QCheckBox *checkBoxOneForAll;
    QTreeWidget *treeWidget;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QCheckBox *checkBoxReassign;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(435, 433);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        Dialog->setMaximumSize(QSize(1000000, 1000000));
        gridLayout_3 = new QGridLayout(Dialog);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        splitter_4 = new QSplitter(Dialog);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        label = new QLabel(splitter_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setTextFormat(Qt::AutoText);
        splitter_4->addWidget(label);
        splitter = new QSplitter(splitter_4);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        lineEditAddressPathsStatistFiles = new QLineEdit(splitter);
        lineEditAddressPathsStatistFiles->setObjectName(QString::fromUtf8("lineEditAddressPathsStatistFiles"));
        splitter->addWidget(lineEditAddressPathsStatistFiles);
        pushButtonStatistFilesDir = new QPushButton(splitter);
        pushButtonStatistFilesDir->setObjectName(QString::fromUtf8("pushButtonStatistFilesDir"));
        pushButtonStatistFilesDir->setMinimumSize(QSize(75, 23));
        splitter->addWidget(pushButtonStatistFilesDir);
        splitter_4->addWidget(splitter);

        gridLayout_2->addWidget(splitter_4, 0, 0, 1, 1);

        splitter_5 = new QSplitter(Dialog);
        splitter_5->setObjectName(QString::fromUtf8("splitter_5"));
        splitter_5->setOrientation(Qt::Vertical);
        label_2 = new QLabel(splitter_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setTextFormat(Qt::AutoText);
        splitter_5->addWidget(label_2);
        splitter_3 = new QSplitter(splitter_5);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        lineEditAddressPathsTestFiles = new QLineEdit(splitter_3);
        lineEditAddressPathsTestFiles->setObjectName(QString::fromUtf8("lineEditAddressPathsTestFiles"));
        splitter_3->addWidget(lineEditAddressPathsTestFiles);
        pushButtonTestDir = new QPushButton(splitter_3);
        pushButtonTestDir->setObjectName(QString::fromUtf8("pushButtonTestDir"));
        splitter_3->addWidget(pushButtonTestDir);
        pushButtonTestFiles = new QPushButton(splitter_3);
        pushButtonTestFiles->setObjectName(QString::fromUtf8("pushButtonTestFiles"));
        splitter_3->addWidget(pushButtonTestFiles);
        splitter_5->addWidget(splitter_3);

        gridLayout_2->addWidget(splitter_5, 1, 0, 1, 1);

        splitter_2 = new QSplitter(Dialog);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        spinBox = new QSpinBox(splitter_2);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximumSize(QSize(61, 23));
        spinBox->setAccelerated(true);
        spinBox->setMinimum(1);
        spinBox->setMaximum(10000);
        spinBox->setValue(1);
        splitter_2->addWidget(spinBox);
        label_3 = new QLabel(splitter_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        splitter_2->addWidget(label_3);
        pushButtonEvaluate = new QPushButton(splitter_2);
        pushButtonEvaluate->setObjectName(QString::fromUtf8("pushButtonEvaluate"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButtonEvaluate->sizePolicy().hasHeightForWidth());
        pushButtonEvaluate->setSizePolicy(sizePolicy1);
        pushButtonEvaluate->setMaximumSize(QSize(78, 16777215));
        splitter_2->addWidget(pushButtonEvaluate);

        gridLayout_2->addWidget(splitter_2, 2, 0, 1, 1);

        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(404, 277));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBoxOneForAll = new QCheckBox(groupBox);
        checkBoxOneForAll->setObjectName(QString::fromUtf8("checkBoxOneForAll"));

        gridLayout->addWidget(checkBoxOneForAll, 0, 2, 1, 1);

        treeWidget = new QTreeWidget(groupBox);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        gridLayout->addWidget(treeWidget, 1, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(229, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 0, 1, 1);

        pushButtonAdd = new QPushButton(groupBox);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));

        gridLayout->addWidget(pushButtonAdd, 2, 1, 1, 1);

        pushButtonDelete = new QPushButton(groupBox);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));

        gridLayout->addWidget(pushButtonDelete, 2, 2, 1, 1);

        checkBoxReassign = new QCheckBox(groupBox);
        checkBoxReassign->setObjectName(QString::fromUtf8("checkBoxReassign"));

        gridLayout->addWidget(checkBoxReassign, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 3, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "Where would you like to store statistic files?", 0, QApplication::UnicodeUTF8));
        pushButtonStatistFilesDir->setText(QApplication::translate("Dialog", "Choose", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "Choose directory/files to test", 0, QApplication::UnicodeUTF8));
        pushButtonTestDir->setText(QApplication::translate("Dialog", "Chose Dir", 0, QApplication::UnicodeUTF8));
        pushButtonTestFiles->setText(QApplication::translate("Dialog", "Choose File(s)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "Iterations", 0, QApplication::UnicodeUTF8));
        pushButtonEvaluate->setText(QApplication::translate("Dialog", "Evaluate", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Dialog", "Set Parameters", 0, QApplication::UnicodeUTF8));
        checkBoxOneForAll->setText(QApplication::translate("Dialog", "One For All", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("Dialog", "Parameter", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("Dialog", "File", 0, QApplication::UnicodeUTF8));
        pushButtonAdd->setText(QApplication::translate("Dialog", "Add", 0, QApplication::UnicodeUTF8));
        pushButtonDelete->setText(QApplication::translate("Dialog", "Delete", 0, QApplication::UnicodeUTF8));
        checkBoxReassign->setText(QApplication::translate("Dialog", "Re-Assigning", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPEEDTESTGUI_H
