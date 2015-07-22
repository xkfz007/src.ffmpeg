/********************************************************************************
** Form generated from reading UI file 'editsheet.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITSHEET_H
#define UI_EDITSHEET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditSheet
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QTableWidget *tableWidget;
    QVBoxLayout *vboxLayout;
    QPushButton *pushButton;
    QCheckBox *checkBoxAdapt;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EditSheet)
    {
        if (EditSheet->objectName().isEmpty())
            EditSheet->setObjectName(QStringLiteral("EditSheet"));
        EditSheet->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(EditSheet);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(EditSheet);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(EditSheet);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout_2->addLayout(horizontalLayout);

        tableWidget = new QTableWidget(EditSheet);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(tableWidget);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(0);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));

        verticalLayout_2->addLayout(vboxLayout);

        pushButton = new QPushButton(EditSheet);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout_2->addWidget(pushButton);

        checkBoxAdapt = new QCheckBox(EditSheet);
        checkBoxAdapt->setObjectName(QStringLiteral("checkBoxAdapt"));

        verticalLayout_2->addWidget(checkBoxAdapt);

        buttonBox = new QDialogButtonBox(EditSheet);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(EditSheet);
        QObject::connect(buttonBox, SIGNAL(accepted()), EditSheet, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EditSheet, SLOT(reject()));

        QMetaObject::connectSlotsByName(EditSheet);
    } // setupUi

    void retranslateUi(QDialog *EditSheet)
    {
        EditSheet->setWindowTitle(QApplication::translate("EditSheet", "Dialog", 0));
        label->setText(QApplication::translate("EditSheet", "Name :", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("EditSheet", "0", 0));
        pushButton->setText(QApplication::translate("EditSheet", "+", 0));
        checkBoxAdapt->setText(QApplication::translate("EditSheet", "Adapt columns to content", 0));
    } // retranslateUi

};

namespace Ui {
    class EditSheet: public Ui_EditSheet {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITSHEET_H
