/********************************************************************************
** Form generated from reading UI file 'editconfigtreetext.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITCONFIGTREETEXT_H
#define UI_EDITCONFIGTREETEXT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_editConfigTreeText
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *lineEdit;
    QToolBox *toolBox;
    QWidget *page;
    QWidget *page_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *editConfigTreeText)
    {
        if (editConfigTreeText->objectName().isEmpty())
            editConfigTreeText->setObjectName(QStringLiteral("editConfigTreeText"));
        editConfigTreeText->resize(417, 604);
        verticalLayout = new QVBoxLayout(editConfigTreeText);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        scrollArea = new QScrollArea(editConfigTreeText);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 397, 551));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lineEdit = new QLineEdit(scrollAreaWidgetContents);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        verticalLayout_2->addWidget(lineEdit);

        toolBox = new QToolBox(scrollAreaWidgetContents);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 379, 438));
        toolBox->addItem(page, QStringLiteral("Page 1"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 98, 28));
        toolBox->addItem(page_2, QStringLiteral("Page 2"));

        verticalLayout_2->addWidget(toolBox);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        buttonBox = new QDialogButtonBox(editConfigTreeText);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(editConfigTreeText);
        QObject::connect(buttonBox, SIGNAL(accepted()), editConfigTreeText, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), editConfigTreeText, SLOT(reject()));

        QMetaObject::connectSlotsByName(editConfigTreeText);
    } // setupUi

    void retranslateUi(QDialog *editConfigTreeText)
    {
        editConfigTreeText->setWindowTitle(QApplication::translate("editConfigTreeText", "Dialog", 0));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("editConfigTreeText", "Page 1", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("editConfigTreeText", "Page 2", 0));
    } // retranslateUi

};

namespace Ui {
    class editConfigTreeText: public Ui_editConfigTreeText {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITCONFIGTREETEXT_H
