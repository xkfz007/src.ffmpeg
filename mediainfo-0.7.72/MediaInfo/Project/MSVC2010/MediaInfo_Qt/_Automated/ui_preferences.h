/********************************************************************************
** Form generated from reading UI file 'preferences.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCES_H
#define UI_PREFERENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Preferences
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QStackedWidget *stackedWidget;
    QWidget *Setup;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *comboBox_language;
    QLabel *label_2;
    QComboBox *comboBox_defaultview;
    QCheckBox *checkForNewVersion;
    QCheckBox *shellExtension;
    QCheckBox *shellInfoTip;
    QWidget *Advanced;
    QCheckBox *showToolbar;
    QCheckBox *showMenu;
    QCheckBox *closeAllBeforeOpen;
    QCheckBox *rememberToolBarPosition;
    QCheckBox *rememberGeometry;
    QWidget *Sheet;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QComboBox *comboBoxSheet;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_deleteSheet;
    QPushButton *pushButton_editSheet;
    QPushButton *pushButton_newSheet;
    QWidget *TreeText;
    QVBoxLayout *verticalLayout_4;
    QLabel *treeTextLabel;
    QComboBox *treeTextComboBox;
    QHBoxLayout *treeTextLayout;
    QPushButton *pushButton_deleteTreeText;
    QPushButton *pushButton_editTreeText;
    QPushButton *pushButton_newTreeText;
    QWidget *Custom;
    QVBoxLayout *verticalLayout_5;
    QLabel *customLabel;
    QComboBox *customComboBox;
    QHBoxLayout *customLayout;
    QPushButton *pushButton_deleteCustom;
    QPushButton *pushButton_editCustom;
    QPushButton *pushButton_newCustom;
    QWidget *page;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_6;
    QComboBox *comboBox_5;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_9;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Preferences)
    {
        if (Preferences->objectName().isEmpty())
            Preferences->setObjectName(QStringLiteral("Preferences"));
        Preferences->resize(437, 215);
        Preferences->setModal(true);
        verticalLayout = new QVBoxLayout(Preferences);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeWidget = new QTreeWidget(Preferences);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->header()->setVisible(false);

        horizontalLayout->addWidget(treeWidget);

        stackedWidget = new QStackedWidget(Preferences);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        Setup = new QWidget();
        Setup->setObjectName(QStringLiteral("Setup"));
        gridLayout = new QGridLayout(Setup);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(Setup);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboBox_language = new QComboBox(Setup);
        comboBox_language->setObjectName(QStringLiteral("comboBox_language"));

        gridLayout->addWidget(comboBox_language, 0, 1, 1, 1);

        label_2 = new QLabel(Setup);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        comboBox_defaultview = new QComboBox(Setup);
        comboBox_defaultview->setObjectName(QStringLiteral("comboBox_defaultview"));

        gridLayout->addWidget(comboBox_defaultview, 1, 1, 1, 1);

        checkForNewVersion = new QCheckBox(Setup);
        checkForNewVersion->setObjectName(QStringLiteral("checkForNewVersion"));

        gridLayout->addWidget(checkForNewVersion, 2, 0, 1, 1);

        shellExtension = new QCheckBox(Setup);
        shellExtension->setObjectName(QStringLiteral("shellExtension"));

        gridLayout->addWidget(shellExtension, 3, 0, 1, 1);

        shellInfoTip = new QCheckBox(Setup);
        shellInfoTip->setObjectName(QStringLiteral("shellInfoTip"));

        gridLayout->addWidget(shellInfoTip, 4, 0, 1, 1);

        stackedWidget->addWidget(Setup);
        Advanced = new QWidget();
        Advanced->setObjectName(QStringLiteral("Advanced"));
        showToolbar = new QCheckBox(Advanced);
        showToolbar->setObjectName(QStringLiteral("showToolbar"));
        showToolbar->setGeometry(QRect(9, 10, 271, 22));
        showMenu = new QCheckBox(Advanced);
        showMenu->setObjectName(QStringLiteral("showMenu"));
        showMenu->setGeometry(QRect(9, 68, 271, 22));
        closeAllBeforeOpen = new QCheckBox(Advanced);
        closeAllBeforeOpen->setObjectName(QStringLiteral("closeAllBeforeOpen"));
        closeAllBeforeOpen->setGeometry(QRect(9, 126, 271, 22));
        rememberToolBarPosition = new QCheckBox(Advanced);
        rememberToolBarPosition->setObjectName(QStringLiteral("rememberToolBarPosition"));
        rememberToolBarPosition->setGeometry(QRect(50, 40, 231, 22));
        rememberGeometry = new QCheckBox(Advanced);
        rememberGeometry->setObjectName(QStringLiteral("rememberGeometry"));
        rememberGeometry->setGeometry(QRect(9, 97, 271, 22));
        stackedWidget->addWidget(Advanced);
        Sheet = new QWidget();
        Sheet->setObjectName(QStringLiteral("Sheet"));
        verticalLayout_2 = new QVBoxLayout(Sheet);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_3 = new QLabel(Sheet);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        comboBoxSheet = new QComboBox(Sheet);
        comboBoxSheet->setObjectName(QStringLiteral("comboBoxSheet"));

        verticalLayout_2->addWidget(comboBoxSheet);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_deleteSheet = new QPushButton(Sheet);
        pushButton_deleteSheet->setObjectName(QStringLiteral("pushButton_deleteSheet"));

        horizontalLayout_2->addWidget(pushButton_deleteSheet);

        pushButton_editSheet = new QPushButton(Sheet);
        pushButton_editSheet->setObjectName(QStringLiteral("pushButton_editSheet"));

        horizontalLayout_2->addWidget(pushButton_editSheet);

        pushButton_newSheet = new QPushButton(Sheet);
        pushButton_newSheet->setObjectName(QStringLiteral("pushButton_newSheet"));

        horizontalLayout_2->addWidget(pushButton_newSheet);


        verticalLayout_2->addLayout(horizontalLayout_2);

        stackedWidget->addWidget(Sheet);
        TreeText = new QWidget();
        TreeText->setObjectName(QStringLiteral("TreeText"));
        verticalLayout_4 = new QVBoxLayout(TreeText);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        treeTextLabel = new QLabel(TreeText);
        treeTextLabel->setObjectName(QStringLiteral("treeTextLabel"));

        verticalLayout_4->addWidget(treeTextLabel);

        treeTextComboBox = new QComboBox(TreeText);
        treeTextComboBox->setObjectName(QStringLiteral("treeTextComboBox"));

        verticalLayout_4->addWidget(treeTextComboBox);

        treeTextLayout = new QHBoxLayout();
        treeTextLayout->setObjectName(QStringLiteral("treeTextLayout"));
        pushButton_deleteTreeText = new QPushButton(TreeText);
        pushButton_deleteTreeText->setObjectName(QStringLiteral("pushButton_deleteTreeText"));

        treeTextLayout->addWidget(pushButton_deleteTreeText);

        pushButton_editTreeText = new QPushButton(TreeText);
        pushButton_editTreeText->setObjectName(QStringLiteral("pushButton_editTreeText"));

        treeTextLayout->addWidget(pushButton_editTreeText);

        pushButton_newTreeText = new QPushButton(TreeText);
        pushButton_newTreeText->setObjectName(QStringLiteral("pushButton_newTreeText"));

        treeTextLayout->addWidget(pushButton_newTreeText);


        verticalLayout_4->addLayout(treeTextLayout);

        stackedWidget->addWidget(TreeText);
        Custom = new QWidget();
        Custom->setObjectName(QStringLiteral("Custom"));
        verticalLayout_5 = new QVBoxLayout(Custom);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        customLabel = new QLabel(Custom);
        customLabel->setObjectName(QStringLiteral("customLabel"));

        verticalLayout_5->addWidget(customLabel);

        customComboBox = new QComboBox(Custom);
        customComboBox->setObjectName(QStringLiteral("customComboBox"));

        verticalLayout_5->addWidget(customComboBox);

        customLayout = new QHBoxLayout();
        customLayout->setObjectName(QStringLiteral("customLayout"));
        pushButton_deleteCustom = new QPushButton(Custom);
        pushButton_deleteCustom->setObjectName(QStringLiteral("pushButton_deleteCustom"));

        customLayout->addWidget(pushButton_deleteCustom);

        pushButton_editCustom = new QPushButton(Custom);
        pushButton_editCustom->setObjectName(QStringLiteral("pushButton_editCustom"));

        customLayout->addWidget(pushButton_editCustom);

        pushButton_newCustom = new QPushButton(Custom);
        pushButton_newCustom->setObjectName(QStringLiteral("pushButton_newCustom"));

        customLayout->addWidget(pushButton_newCustom);


        verticalLayout_5->addLayout(customLayout);

        stackedWidget->addWidget(Custom);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout_6 = new QVBoxLayout(page);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label_6 = new QLabel(page);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_6->addWidget(label_6);

        comboBox_5 = new QComboBox(page);
        comboBox_5->setObjectName(QStringLiteral("comboBox_5"));

        verticalLayout_6->addWidget(comboBox_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_9 = new QPushButton(page);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        horizontalLayout_4->addWidget(pushButton_9);

        pushButton_8 = new QPushButton(page);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        horizontalLayout_4->addWidget(pushButton_8);

        pushButton_7 = new QPushButton(page);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout_4->addWidget(pushButton_7);


        verticalLayout_6->addLayout(horizontalLayout_4);

        stackedWidget->addWidget(page);

        horizontalLayout->addWidget(stackedWidget);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(Preferences);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Preferences);
        QObject::connect(buttonBox, SIGNAL(accepted()), Preferences, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Preferences, SLOT(reject()));
        QObject::connect(showToolbar, SIGNAL(toggled(bool)), rememberToolBarPosition, SLOT(setEnabled(bool)));
        QObject::connect(showToolbar, SIGNAL(toggled(bool)), showMenu, SLOT(setEnabled(bool)));
        QObject::connect(showMenu, SIGNAL(toggled(bool)), showToolbar, SLOT(setEnabled(bool)));

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Preferences);
    } // setupUi

    void retranslateUi(QDialog *Preferences)
    {
        Preferences->setWindowTitle(QApplication::translate("Preferences", "Preferences", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("Preferences", "widget index", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("Preferences", "name", 0));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(1, QApplication::translate("Preferences", "0", 0));
        ___qtreewidgetitem1->setText(0, QApplication::translate("Preferences", "Setup", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(1, QApplication::translate("Preferences", "1", 0));
        ___qtreewidgetitem2->setText(0, QApplication::translate("Preferences", "Advanced", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("Preferences", "Customize", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem3->child(0);
        ___qtreewidgetitem4->setText(1, QApplication::translate("Preferences", "2", 0));
        ___qtreewidgetitem4->setText(0, QApplication::translate("Preferences", "Sheet", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem3->child(1);
        ___qtreewidgetitem5->setText(1, QApplication::translate("Preferences", "3", 0));
        ___qtreewidgetitem5->setText(0, QApplication::translate("Preferences", "Tree & Text", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem3->child(2);
        ___qtreewidgetitem6->setText(1, QApplication::translate("Preferences", "4", 0));
        ___qtreewidgetitem6->setText(0, QApplication::translate("Preferences", "Custom", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem3->child(3);
        ___qtreewidgetitem7->setText(1, QApplication::translate("Preferences", "5", 0));
        ___qtreewidgetitem7->setText(0, QApplication::translate("Preferences", "Language", 0));
        treeWidget->setSortingEnabled(__sortingEnabled);

        label->setText(QApplication::translate("Preferences", "Language", 0));
        comboBox_language->clear();
        comboBox_language->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Fran\303\247ais", 0)
         << QApplication::translate("Preferences", "English", 0)
        );
        label_2->setText(QApplication::translate("Preferences", "Default View", 0));
        checkForNewVersion->setText(QApplication::translate("Preferences", "Check for new versions", 0));
        shellExtension->setText(QApplication::translate("Preferences", "Shell extension", 0));
        shellInfoTip->setText(QApplication::translate("Preferences", "Shell InfoTip", 0));
        showToolbar->setText(QApplication::translate("Preferences", "Show toolbar", 0));
        showMenu->setText(QApplication::translate("Preferences", "Show menu", 0));
        closeAllBeforeOpen->setText(QApplication::translate("Preferences", "Close all before open", 0));
        rememberToolBarPosition->setText(QApplication::translate("Preferences", "Remember its position", 0));
        rememberGeometry->setText(QApplication::translate("Preferences", "Remember Window Geometry", 0));
        label_3->setText(QApplication::translate("Preferences", "Choose your desired custom sheet", 0));
        pushButton_deleteSheet->setText(QApplication::translate("Preferences", "Delete", 0));
        pushButton_editSheet->setText(QApplication::translate("Preferences", "Edit", 0));
        pushButton_newSheet->setText(QApplication::translate("Preferences", "New", 0));
        treeTextLabel->setText(QApplication::translate("Preferences", "Select your desired configuration", 0));
        pushButton_deleteTreeText->setText(QApplication::translate("Preferences", "Delete", 0));
        pushButton_editTreeText->setText(QApplication::translate("Preferences", "Edit", 0));
        pushButton_newTreeText->setText(QApplication::translate("Preferences", "New", 0));
        customLabel->setText(QApplication::translate("Preferences", "Choose your desired custom text", 0));
        pushButton_deleteCustom->setText(QApplication::translate("Preferences", "Delete", 0));
        pushButton_editCustom->setText(QApplication::translate("Preferences", "Edit", 0));
        pushButton_newCustom->setText(QApplication::translate("Preferences", "New", 0));
        label_6->setText(QApplication::translate("Preferences", "Choose your desired language", 0));
        pushButton_9->setText(QApplication::translate("Preferences", "Delete", 0));
        pushButton_8->setText(QApplication::translate("Preferences", "Edit", 0));
        pushButton_7->setText(QApplication::translate("Preferences", "New", 0));
    } // retranslateUi

};

namespace Ui {
    class Preferences: public Ui_Preferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCES_H
