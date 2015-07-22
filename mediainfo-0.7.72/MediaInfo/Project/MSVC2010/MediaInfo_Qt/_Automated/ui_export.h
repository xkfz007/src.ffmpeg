/********************************************************************************
** Form generated from reading UI file 'export.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORT_H
#define UI_EXPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Export
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *formatBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBoxMode;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBoxConfig;
    QCheckBox *checkBoxAdvanced;
    QGroupBox *filenameBox;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QCheckBox *appendToFile;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Export)
    {
        if (Export->objectName().isEmpty())
            Export->setObjectName(QStringLiteral("Export"));
        Export->resize(400, 300);
        verticalLayout = new QVBoxLayout(Export);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formatBox = new QGroupBox(Export);
        formatBox->setObjectName(QStringLiteral("formatBox"));
        verticalLayout_2 = new QVBoxLayout(formatBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(formatBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBoxMode = new QComboBox(formatBox);
        comboBoxMode->setObjectName(QStringLiteral("comboBoxMode"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBoxMode->sizePolicy().hasHeightForWidth());
        comboBoxMode->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboBoxMode);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(formatBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBoxConfig = new QComboBox(formatBox);
        comboBoxConfig->setObjectName(QStringLiteral("comboBoxConfig"));
        sizePolicy.setHeightForWidth(comboBoxConfig->sizePolicy().hasHeightForWidth());
        comboBoxConfig->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(comboBoxConfig);


        verticalLayout_2->addLayout(horizontalLayout_2);

        checkBoxAdvanced = new QCheckBox(formatBox);
        checkBoxAdvanced->setObjectName(QStringLiteral("checkBoxAdvanced"));

        verticalLayout_2->addWidget(checkBoxAdvanced);


        verticalLayout->addWidget(formatBox);

        filenameBox = new QGroupBox(Export);
        filenameBox->setObjectName(QStringLiteral("filenameBox"));
        gridLayout = new QGridLayout(filenameBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lineEdit = new QLineEdit(filenameBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

        toolButton = new QToolButton(filenameBox);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        gridLayout->addWidget(toolButton, 1, 1, 1, 1);

        appendToFile = new QCheckBox(filenameBox);
        appendToFile->setObjectName(QStringLiteral("appendToFile"));

        gridLayout->addWidget(appendToFile, 0, 0, 1, 1);


        verticalLayout->addWidget(filenameBox);

        buttonBox = new QDialogButtonBox(Export);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Export);
        QObject::connect(buttonBox, SIGNAL(accepted()), Export, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Export, SLOT(reject()));

        QMetaObject::connectSlotsByName(Export);
    } // setupUi

    void retranslateUi(QDialog *Export)
    {
        Export->setWindowTitle(QApplication::translate("Export", "Export", 0));
        formatBox->setTitle(QApplication::translate("Export", "Choose your desired export format", 0));
        label->setText(QApplication::translate("Export", "Format:", 0));
        label_2->setText(QApplication::translate("Export", "Profile:", 0));
        checkBoxAdvanced->setText(QApplication::translate("Export", "Advanced mode", 0));
        filenameBox->setTitle(QApplication::translate("Export", "Choose your desired filename", 0));
        toolButton->setText(QApplication::translate("Export", "...", 0));
        appendToFile->setText(QApplication::translate("Export", "Append to the existing file", 0));
    } // retranslateUi

};

namespace Ui {
    class Export: public Ui_Export {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORT_H
