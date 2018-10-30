/********************************************************************************
** Form generated from reading UI file 'CompositeTileSourceConfigurationWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPOSITETILESOURCECONFIGURATIONWIDGET_H
#define UI_COMPOSITETILESOURCECONFIGURATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CompositeTileSourceConfigurationWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *addSourceButton;
    QToolButton *removeSourceButton;
    QToolButton *moveDownButton;
    QToolButton *moveUpButton;
    QSlider *opacitySlider;
    QSpacerItem *horizontalSpacer;
    QListView *listView;

    void setupUi(QWidget *CompositeTileSourceConfigurationWidget)
    {
        if (CompositeTileSourceConfigurationWidget->objectName().isEmpty())
            CompositeTileSourceConfigurationWidget->setObjectName(QLatin1String("CompositeTileSourceConfigurationWidget"));
        CompositeTileSourceConfigurationWidget->resize(393, 343);
        verticalLayout = new QVBoxLayout(CompositeTileSourceConfigurationWidget);
        verticalLayout->setObjectName(QLatin1String("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QLatin1String("horizontalLayout"));
        addSourceButton = new QToolButton(CompositeTileSourceConfigurationWidget);
        addSourceButton->setObjectName(QLatin1String("addSourceButton"));
        QIcon icon;
        icon.addFile(QLatin1String(":/icons/images/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addSourceButton->setIcon(icon);
        addSourceButton->setPopupMode(QToolButton::InstantPopup);

        horizontalLayout->addWidget(addSourceButton);

        removeSourceButton = new QToolButton(CompositeTileSourceConfigurationWidget);
        removeSourceButton->setObjectName(QLatin1String("removeSourceButton"));
        removeSourceButton->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QLatin1String(":/icons/images/editdelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        removeSourceButton->setIcon(icon1);

        horizontalLayout->addWidget(removeSourceButton);

        moveDownButton = new QToolButton(CompositeTileSourceConfigurationWidget);
        moveDownButton->setObjectName(QLatin1String("moveDownButton"));
        moveDownButton->setEnabled(false);
        moveDownButton->setArrowType(Qt::DownArrow);

        horizontalLayout->addWidget(moveDownButton);

        moveUpButton = new QToolButton(CompositeTileSourceConfigurationWidget);
        moveUpButton->setObjectName(QLatin1String("moveUpButton"));
        moveUpButton->setEnabled(false);
        moveUpButton->setArrowType(Qt::UpArrow);

        horizontalLayout->addWidget(moveUpButton);

        opacitySlider = new QSlider(CompositeTileSourceConfigurationWidget);
        opacitySlider->setObjectName(QLatin1String("opacitySlider"));
        opacitySlider->setEnabled(false);
        opacitySlider->setMaximum(100);
        opacitySlider->setTracking(false);
        opacitySlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(opacitySlider);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        listView = new QListView(CompositeTileSourceConfigurationWidget);
        listView->setObjectName(QLatin1String("listView"));
        listView->setFocusPolicy(Qt::ClickFocus);
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setDragDropMode(QAbstractItemView::InternalMove);
        listView->setUniformItemSizes(true);

        verticalLayout->addWidget(listView);


        retranslateUi(CompositeTileSourceConfigurationWidget);

        QMetaObject::connectSlotsByName(CompositeTileSourceConfigurationWidget);
    } // setupUi

    void retranslateUi(QWidget *CompositeTileSourceConfigurationWidget)
    {
        CompositeTileSourceConfigurationWidget->setWindowTitle(QApplication::translate("CompositeTileSourceConfigurationWidget", "Form", nullptr));
        addSourceButton->setText(QApplication::translate("CompositeTileSourceConfigurationWidget", "+", nullptr));
        removeSourceButton->setText(QApplication::translate("CompositeTileSourceConfigurationWidget", "X", nullptr));
        moveDownButton->setText(QApplication::translate("CompositeTileSourceConfigurationWidget", "D", nullptr));
        moveUpButton->setText(QApplication::translate("CompositeTileSourceConfigurationWidget", "U", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CompositeTileSourceConfigurationWidget: public Ui_CompositeTileSourceConfigurationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPOSITETILESOURCECONFIGURATIONWIDGET_H
