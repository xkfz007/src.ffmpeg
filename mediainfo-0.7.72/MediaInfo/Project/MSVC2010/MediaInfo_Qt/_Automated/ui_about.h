/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_6;
    QLabel *logo;
    QSpacerItem *verticalSpacer_2;
    QLabel *aboutText;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *logoMail;
    QVBoxLayout *buttons;
    QPushButton *website;
    QPushButton *checkversion;
    QPushButton *donate;
    QPushButton *mail;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QPushButton *okButton;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QStringLiteral("About"));
        About->resize(479, 381);
        verticalLayout = new QVBoxLayout(About);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        logo = new QLabel(About);
        logo->setObjectName(QStringLiteral("logo"));
        logo->setMaximumSize(QSize(128, 128));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/icon/icon.png")));
        logo->setScaledContents(true);
        logo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_6->addWidget(logo);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout_6);

        aboutText = new QLabel(About);
        aboutText->setObjectName(QStringLiteral("aboutText"));

        horizontalLayout->addWidget(aboutText);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        logoMail = new QLabel(About);
        logoMail->setObjectName(QStringLiteral("logoMail"));
        logoMail->setMaximumSize(QSize(64, 64));
        logoMail->setPixmap(QPixmap(QString::fromUtf8(":/icon/aboutmail.png")));
        logoMail->setScaledContents(true);
        logoMail->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(logoMail);

        buttons = new QVBoxLayout();
        buttons->setSpacing(0);
        buttons->setObjectName(QStringLiteral("buttons"));
        website = new QPushButton(About);
        website->setObjectName(QStringLiteral("website"));

        buttons->addWidget(website);

        checkversion = new QPushButton(About);
        checkversion->setObjectName(QStringLiteral("checkversion"));

        buttons->addWidget(checkversion);

        donate = new QPushButton(About);
        donate->setObjectName(QStringLiteral("donate"));

        buttons->addWidget(donate);

        mail = new QPushButton(About);
        mail->setObjectName(QStringLiteral("mail"));

        buttons->addWidget(mail);


        horizontalLayout_2->addLayout(buttons);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(About);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_3->addWidget(label_3);

        okButton = new QPushButton(About);
        okButton->setObjectName(QStringLiteral("okButton"));

        verticalLayout_3->addWidget(okButton);


        horizontalLayout_2->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About", 0));
        aboutText->setText(QApplication::translate("About", "MediaInfo 0.7.34\n"
"Copyright (C) 2002-2009 Jerome Martinez\n"
"\n"
"Supply very detailled information\n"
"about a multimedia file:\n"
"Matroska, OGG (including OGM)\n"
"MPEG1 (including VCD)\n"
"MPEG2 (including DVD and SVCD)\n"
"MPEG4 (including Itunes M4A)\n"
"Quicktime\n"
"RealMedia\n"
"WindowsMedia (including WMV, WMA)\n"
"Microsoft RIFF (including AVI, WAV)\n"
"Sound-only formats (AC3, DTS, AAC, AU, AIFF...)", 0));
        website->setText(QApplication::translate("About", "Go to website", 0));
        checkversion->setText(QApplication::translate("About", "Check for new version", 0));
        donate->setText(QApplication::translate("About", "Donate", 0));
        mail->setText(QApplication::translate("About", "Write mail to author", 0));
        label_3->setText(QApplication::translate("About", "Translator : Zen", 0));
        okButton->setText(QApplication::translate("About", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
