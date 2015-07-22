/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionQuit;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionPrint;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *check_Y;
    QCheckBox *check_U;
    QCheckBox *check_V;
    QCheckBox *check_YDiff;
    QCheckBox *check_UDiff;
    QCheckBox *check_VDiff;
    QCheckBox *check_Diffs;
    QCheckBox *check_TOUT;
    QCheckBox *check_VREP;
    QCheckBox *check_RANG;
    QScrollBar *horizontalScrollBar;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(797, 513);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName(QStringLiteral("actionZoomIn"));
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName(QStringLiteral("actionZoomOut"));
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        check_Y = new QCheckBox(centralwidget);
        check_Y->setObjectName(QStringLiteral("check_Y"));
        check_Y->setChecked(true);

        horizontalLayout->addWidget(check_Y);

        check_U = new QCheckBox(centralwidget);
        check_U->setObjectName(QStringLiteral("check_U"));
        check_U->setChecked(true);

        horizontalLayout->addWidget(check_U);

        check_V = new QCheckBox(centralwidget);
        check_V->setObjectName(QStringLiteral("check_V"));
        check_V->setChecked(true);

        horizontalLayout->addWidget(check_V);

        check_YDiff = new QCheckBox(centralwidget);
        check_YDiff->setObjectName(QStringLiteral("check_YDiff"));

        horizontalLayout->addWidget(check_YDiff);

        check_UDiff = new QCheckBox(centralwidget);
        check_UDiff->setObjectName(QStringLiteral("check_UDiff"));

        horizontalLayout->addWidget(check_UDiff);

        check_VDiff = new QCheckBox(centralwidget);
        check_VDiff->setObjectName(QStringLiteral("check_VDiff"));
        check_VDiff->setChecked(false);

        horizontalLayout->addWidget(check_VDiff);

        check_Diffs = new QCheckBox(centralwidget);
        check_Diffs->setObjectName(QStringLiteral("check_Diffs"));
        check_Diffs->setChecked(true);

        horizontalLayout->addWidget(check_Diffs);

        check_TOUT = new QCheckBox(centralwidget);
        check_TOUT->setObjectName(QStringLiteral("check_TOUT"));
        check_TOUT->setChecked(true);

        horizontalLayout->addWidget(check_TOUT);

        check_VREP = new QCheckBox(centralwidget);
        check_VREP->setObjectName(QStringLiteral("check_VREP"));
        check_VREP->setChecked(true);

        horizontalLayout->addWidget(check_VREP);

        check_RANG = new QCheckBox(centralwidget);
        check_RANG->setObjectName(QStringLiteral("check_RANG"));
        check_RANG->setChecked(true);

        horizontalLayout->addWidget(check_RANG);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        horizontalScrollBar = new QScrollBar(centralwidget);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalScrollBar, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 797, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuView->addAction(actionZoomIn);
        menuView->addAction(actionZoomOut);
        toolBar->addAction(actionOpen);
        toolBar->addSeparator();
        toolBar->addAction(actionZoomIn);
        toolBar->addAction(actionZoomOut);
        toolBar->addSeparator();
        toolBar->addAction(actionPrint);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionZoomIn->setText(QApplication::translate("MainWindow", "Zoom in", 0));
        actionZoomOut->setText(QApplication::translate("MainWindow", "Zoom out", 0));
        actionPrint->setText(QApplication::translate("MainWindow", "Print", 0));
        check_Y->setText(QApplication::translate("MainWindow", "Y values", 0));
        check_U->setText(QApplication::translate("MainWindow", "U values", 0));
        check_V->setText(QApplication::translate("MainWindow", "V values", 0));
        check_YDiff->setText(QApplication::translate("MainWindow", "YDiff", 0));
        check_UDiff->setText(QApplication::translate("MainWindow", "UDiff", 0));
        check_VDiff->setText(QApplication::translate("MainWindow", "VDiff", 0));
        check_Diffs->setText(QApplication::translate("MainWindow", "Diffs", 0));
        check_TOUT->setText(QApplication::translate("MainWindow", "TOUT", 0));
        check_VREP->setText(QApplication::translate("MainWindow", "VREP", 0));
        check_RANG->setText(QApplication::translate("MainWindow", "RANG", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
