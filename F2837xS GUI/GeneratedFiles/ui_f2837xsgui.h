/********************************************************************************
** Form generated from reading UI file 'f2837xsgui.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F2837XSGUI_H
#define UI_F2837XSGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_F2837xSGUIClass
{
public:
    QAction *actionView_Help;
    QAction *actionAbout;
    QAction *actionGpioInfo;
    QAction *actionEpwmInfo;
    QAction *actionUsbInfo;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *chartArea;
    QVBoxLayout *inputLayout;
    QPushButton *exitButton;
    QPushButton *clearMessabeBoxButton;
    QPushButton *testButton;
    QCheckBox *debugDataCheckBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuCheat_Sheets;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *F2837xSGUIClass)
    {
        if (F2837xSGUIClass->objectName().isEmpty())
            F2837xSGUIClass->setObjectName(QStringLiteral("F2837xSGUIClass"));
        F2837xSGUIClass->resize(1076, 730);
        actionView_Help = new QAction(F2837xSGUIClass);
        actionView_Help->setObjectName(QStringLiteral("actionView_Help"));
        actionAbout = new QAction(F2837xSGUIClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionGpioInfo = new QAction(F2837xSGUIClass);
        actionGpioInfo->setObjectName(QStringLiteral("actionGpioInfo"));
        actionEpwmInfo = new QAction(F2837xSGUIClass);
        actionEpwmInfo->setObjectName(QStringLiteral("actionEpwmInfo"));
        actionUsbInfo = new QAction(F2837xSGUIClass);
        actionUsbInfo->setObjectName(QStringLiteral("actionUsbInfo"));
        centralWidget = new QWidget(F2837xSGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        chartArea = new QVBoxLayout();
        chartArea->setSpacing(0);
        chartArea->setObjectName(QStringLiteral("chartArea"));

        gridLayout->addLayout(chartArea, 1, 0, 2, 1);

        inputLayout = new QVBoxLayout();
        inputLayout->setSpacing(6);
        inputLayout->setObjectName(QStringLiteral("inputLayout"));

        gridLayout->addLayout(inputLayout, 1, 3, 1, 4);

        exitButton = new QPushButton(centralWidget);
        exitButton->setObjectName(QStringLiteral("exitButton"));

        gridLayout->addWidget(exitButton, 2, 6, 1, 1);

        clearMessabeBoxButton = new QPushButton(centralWidget);
        clearMessabeBoxButton->setObjectName(QStringLiteral("clearMessabeBoxButton"));

        gridLayout->addWidget(clearMessabeBoxButton, 2, 4, 1, 1);

        testButton = new QPushButton(centralWidget);
        testButton->setObjectName(QStringLiteral("testButton"));

        gridLayout->addWidget(testButton, 2, 5, 1, 1);

        debugDataCheckBox = new QCheckBox(centralWidget);
        debugDataCheckBox->setObjectName(QStringLiteral("debugDataCheckBox"));

        gridLayout->addWidget(debugDataCheckBox, 2, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        F2837xSGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(F2837xSGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1076, 31));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuCheat_Sheets = new QMenu(menuHelp);
        menuCheat_Sheets->setObjectName(QStringLiteral("menuCheat_Sheets"));
        F2837xSGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(F2837xSGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        F2837xSGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(F2837xSGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        F2837xSGUIClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuHelp->addAction(actionView_Help);
        menuHelp->addAction(menuCheat_Sheets->menuAction());
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuCheat_Sheets->addAction(actionUsbInfo);
        menuCheat_Sheets->addAction(actionGpioInfo);
        menuCheat_Sheets->addAction(actionEpwmInfo);

        retranslateUi(F2837xSGUIClass);

        QMetaObject::connectSlotsByName(F2837xSGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *F2837xSGUIClass)
    {
        F2837xSGUIClass->setWindowTitle(QApplication::translate("F2837xSGUIClass", "F2837xSGUI", Q_NULLPTR));
        actionView_Help->setText(QApplication::translate("F2837xSGUIClass", "View Help", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("F2837xSGUIClass", "About", Q_NULLPTR));
        actionGpioInfo->setText(QApplication::translate("F2837xSGUIClass", "GPIO", Q_NULLPTR));
        actionEpwmInfo->setText(QApplication::translate("F2837xSGUIClass", "EPWM", Q_NULLPTR));
        actionUsbInfo->setText(QApplication::translate("F2837xSGUIClass", "USB Commands", Q_NULLPTR));
        exitButton->setText(QApplication::translate("F2837xSGUIClass", "Exit", Q_NULLPTR));
        clearMessabeBoxButton->setText(QApplication::translate("F2837xSGUIClass", "clear", Q_NULLPTR));
        testButton->setText(QApplication::translate("F2837xSGUIClass", "testButton", Q_NULLPTR));
        debugDataCheckBox->setText(QApplication::translate("F2837xSGUIClass", "Generate Debug Data", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("F2837xSGUIClass", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("F2837xSGUIClass", "Help", Q_NULLPTR));
        menuCheat_Sheets->setTitle(QApplication::translate("F2837xSGUIClass", "Cheat Sheets", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class F2837xSGUIClass: public Ui_F2837xSGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F2837XSGUI_H
