/********************************************************************************
** Form generated from reading UI file 'QtOpenGL.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTOPENGL_H
#define UI_QTOPENGL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QViewport.h"

QT_BEGIN_NAMESPACE

class Ui_QtOpenGLClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QViewport *viewport;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtOpenGLClass)
    {
        if (QtOpenGLClass->objectName().isEmpty())
            QtOpenGLClass->setObjectName(QString::fromUtf8("QtOpenGLClass"));
        QtOpenGLClass->resize(1200, 524);
        centralWidget = new QWidget(QtOpenGLClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        viewport = new QViewport(centralWidget);
        viewport->setObjectName(QString::fromUtf8("viewport"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(viewport->sizePolicy().hasHeightForWidth());
        viewport->setSizePolicy(sizePolicy);
        viewport->setMinimumSize(QSize(1000, 400));
        viewport->setMaximumSize(QSize(1000, 400));
        viewport->setAutoFillBackground(true);

        gridLayout->addWidget(viewport, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(200, 16777215));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 2, 0, 1, 1);


        gridLayout->addWidget(widget, 2, 1, 1, 1);

        QtOpenGLClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtOpenGLClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 21));
        QtOpenGLClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtOpenGLClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtOpenGLClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtOpenGLClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtOpenGLClass->setStatusBar(statusBar);

        retranslateUi(QtOpenGLClass);
        QObject::connect(pushButton, SIGNAL(released()), QtOpenGLClass, SLOT(onPushButton()));

        QMetaObject::connectSlotsByName(QtOpenGLClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtOpenGLClass)
    {
        QtOpenGLClass->setWindowTitle(QApplication::translate("QtOpenGLClass", "QtOpenGL", nullptr));
        pushButton->setText(QApplication::translate("QtOpenGLClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtOpenGLClass: public Ui_QtOpenGLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTOPENGL_H
