/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *QTVersionLabel;
    QLabel *xVersion;
    QLabel *Proj4VersionLabel;
    QLabel *GdalVersionLabel;
    QLabel *QTVersion;
    QLabel *osmVersion;
    QLabel *VersionLabel;
    QLabel *Version;
    QSpacerItem *horizontalSpacer;
    QLabel *pxIcon;
    QLabel *label;
    QLabel *label_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTextEdit *txtChangelog;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(581, 434);
        AboutDialog->setSizeGripEnabled(true);
        verticalLayout_3 = new QVBoxLayout(AboutDialog);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_3->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(20);
        gridLayout->setVerticalSpacing(-1);
        QTVersionLabel = new QLabel(AboutDialog);
        QTVersionLabel->setObjectName(QStringLiteral("QTVersionLabel"));
        QTVersionLabel->setStyleSheet(QStringLiteral("QLabel { font-size: x-small; font-style: italic; }"));

        gridLayout->addWidget(QTVersionLabel, 2, 0, 1, 1);

        xVersion = new QLabel(AboutDialog);
        xVersion->setObjectName(QStringLiteral("xVersion"));
        xVersion->setStyleSheet(QStringLiteral("QLabel { font-size:small; font-weight:bold; }"));
        xVersion->setText(QStringLiteral("%1"));

        gridLayout->addWidget(xVersion, 4, 1, 1, 1);

        Proj4VersionLabel = new QLabel(AboutDialog);
        Proj4VersionLabel->setObjectName(QStringLiteral("Proj4VersionLabel"));
        Proj4VersionLabel->setStyleSheet(QStringLiteral("QLabel { font-size: x-small; font-style: italic; }"));

        gridLayout->addWidget(Proj4VersionLabel, 3, 0, 1, 1);

        GdalVersionLabel = new QLabel(AboutDialog);
        GdalVersionLabel->setObjectName(QStringLiteral("GdalVersionLabel"));
        GdalVersionLabel->setStyleSheet(QStringLiteral("QLabel { font-size: x-small; font-style: italic; }"));

        gridLayout->addWidget(GdalVersionLabel, 4, 0, 1, 1);

        QTVersion = new QLabel(AboutDialog);
        QTVersion->setObjectName(QStringLiteral("QTVersion"));
        QTVersion->setStyleSheet(QStringLiteral("QLabel { font-size:small; font-weight:bold; }"));
        QTVersion->setText(QStringLiteral("%1 (%2)"));

        gridLayout->addWidget(QTVersion, 2, 1, 1, 1);

        osmVersion = new QLabel(AboutDialog);
        osmVersion->setObjectName(QStringLiteral("osmVersion"));
        osmVersion->setStyleSheet(QStringLiteral("QLabel { font-size:small; font-weight:bold; }"));
        osmVersion->setText(QStringLiteral("%1"));

        gridLayout->addWidget(osmVersion, 3, 1, 1, 1);

        VersionLabel = new QLabel(AboutDialog);
        VersionLabel->setObjectName(QStringLiteral("VersionLabel"));
        VersionLabel->setStyleSheet(QStringLiteral("QLabel { font-size: x-small; font-style: italic; }"));

        gridLayout->addWidget(VersionLabel, 1, 0, 1, 1);

        Version = new QLabel(AboutDialog);
        Version->setObjectName(QStringLiteral("Version"));
        Version->setStyleSheet(QStringLiteral("QLabel { font-size:small; font-weight:bold; }"));
        Version->setText(QStringLiteral("%1"));

        gridLayout->addWidget(Version, 1, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pxIcon = new QLabel(AboutDialog);
        pxIcon->setObjectName(QStringLiteral("pxIcon"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(pxIcon->sizePolicy().hasHeightForWidth());
        pxIcon->setSizePolicy(sizePolicy);
        pxIcon->setMinimumSize(QSize(100, 100));
        pxIcon->setText(QStringLiteral("TextLabel"));

        horizontalLayout->addWidget(pxIcon);


        verticalLayout_3->addLayout(horizontalLayout);

        label = new QLabel(AboutDialog);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        groupBox = new QGroupBox(AboutDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        txtChangelog = new QTextEdit(groupBox);
        txtChangelog->setObjectName(QStringLiteral("txtChangelog"));
        txtChangelog->setLineWrapMode(QTextEdit::NoWrap);
        txtChangelog->setReadOnly(true);
        txtChangelog->setTabStopWidth(4);

        verticalLayout->addWidget(txtChangelog);


        verticalLayout_3->addWidget(groupBox);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(AboutDialog);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);


        verticalLayout_3->addLayout(hboxLayout);


        retranslateUi(AboutDialog);
        QObject::connect(okButton, SIGNAL(clicked()), AboutDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About Simultra", nullptr));
        label_2->setText(QApplication::translate("AboutDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:600;\">Simultra </span><span style=\" font-size:11pt; font-weight:600; color:#000000;\">\342\200\224 A multiagent road traffic simulator in urban areas</span></p></body></html>", nullptr));
        QTVersionLabel->setText(QApplication::translate("AboutDialog", "Qt version", nullptr));
        Proj4VersionLabel->setText(QApplication::translate("AboutDialog", "OSM version", nullptr));
        GdalVersionLabel->setText(QApplication::translate("AboutDialog", "RadarImg version", nullptr));
        VersionLabel->setText(QApplication::translate("AboutDialog", "Simultra version", nullptr));
        label->setText(QApplication::translate("AboutDialog", "Copyright 2016-2018 Rafik Hadfi <rafik.hadfi@gmail.com>", nullptr));
        label_3->setText(QApplication::translate("AboutDialog", "This software is under the GNU General Public Licence version 3", nullptr));
        groupBox->setTitle(QString());
        okButton->setText(QApplication::translate("AboutDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
