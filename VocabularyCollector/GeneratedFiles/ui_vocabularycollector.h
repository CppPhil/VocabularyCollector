/********************************************************************************
** Form generated from reading UI file 'vocabularycollector.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOCABULARYCOLLECTOR_H
#define UI_VOCABULARYCOLLECTOR_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VocabularyCollectorClass
{
public:
    QAction *actionSave_As;
    QAction *actionPrint_Now;
    QWidget *centralWidget;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QLineEdit *lineEditEnglish;
    QLineEdit *lineEditGerman;
    QLabel *labelEnglish;
    QLabel *labelGerman;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VocabularyCollectorClass)
    {
        if (VocabularyCollectorClass->objectName().isEmpty())
            VocabularyCollectorClass->setObjectName(QStringLiteral("VocabularyCollectorClass"));
        VocabularyCollectorClass->resize(683, 450);
        actionSave_As = new QAction(VocabularyCollectorClass);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionPrint_Now = new QAction(VocabularyCollectorClass);
        actionPrint_Now->setObjectName(QStringLiteral("actionPrint_Now"));
        centralWidget = new QWidget(VocabularyCollectorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(20, 70, 641, 321));
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(2);
        tableWidget->horizontalHeader()->setDefaultSectionSize(300);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(200, 30, 75, 23));
        pushButton->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        lineEditEnglish = new QLineEdit(centralWidget);
        lineEditEnglish->setObjectName(QStringLiteral("lineEditEnglish"));
        lineEditEnglish->setGeometry(QRect(20, 30, 81, 20));
        lineEditEnglish->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        lineEditGerman = new QLineEdit(centralWidget);
        lineEditGerman->setObjectName(QStringLiteral("lineEditGerman"));
        lineEditGerman->setGeometry(QRect(110, 30, 81, 20));
        labelEnglish = new QLabel(centralWidget);
        labelEnglish->setObjectName(QStringLiteral("labelEnglish"));
        labelEnglish->setGeometry(QRect(40, 10, 47, 13));
        labelEnglish->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        labelGerman = new QLabel(centralWidget);
        labelGerman->setObjectName(QStringLiteral("labelGerman"));
        labelGerman->setGeometry(QRect(130, 10, 47, 13));
        VocabularyCollectorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VocabularyCollectorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 683, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        VocabularyCollectorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VocabularyCollectorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VocabularyCollectorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VocabularyCollectorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VocabularyCollectorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionPrint_Now);

        retranslateUi(VocabularyCollectorClass);

        QMetaObject::connectSlotsByName(VocabularyCollectorClass);
    } // setupUi

    void retranslateUi(QMainWindow *VocabularyCollectorClass)
    {
        VocabularyCollectorClass->setWindowTitle(QApplication::translate("VocabularyCollectorClass", "VocabularyCollector", 0));
        actionSave_As->setText(QApplication::translate("VocabularyCollectorClass", "Save As...", 0));
        actionPrint_Now->setText(QApplication::translate("VocabularyCollectorClass", "Print Now", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("VocabularyCollectorClass", "English", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("VocabularyCollectorClass", "German", 0));
        pushButton->setText(QApplication::translate("VocabularyCollectorClass", "Add to list", 0));
        labelEnglish->setText(QApplication::translate("VocabularyCollectorClass", "English", 0));
        labelGerman->setText(QApplication::translate("VocabularyCollectorClass", "German", 0));
        menuFile->setTitle(QApplication::translate("VocabularyCollectorClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class VocabularyCollectorClass: public Ui_VocabularyCollectorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOCABULARYCOLLECTOR_H
