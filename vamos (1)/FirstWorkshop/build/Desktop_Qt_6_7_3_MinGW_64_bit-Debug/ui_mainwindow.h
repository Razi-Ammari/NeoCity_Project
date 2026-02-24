/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_cin;
    QLineEdit *lineEdit_cin;
    QLabel *label_prenom;
    QLineEdit *lineEdit_prenom;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_afficher;
    QPushButton *pushButton_quitter;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(500, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label_cin = new QLabel(groupBox);
        label_cin->setObjectName("label_cin");

        gridLayout->addWidget(label_cin, 0, 0, 1, 1);

        lineEdit_cin = new QLineEdit(groupBox);
        lineEdit_cin->setObjectName("lineEdit_cin");

        gridLayout->addWidget(lineEdit_cin, 0, 1, 1, 1);

        label_prenom = new QLabel(groupBox);
        label_prenom->setObjectName("label_prenom");

        gridLayout->addWidget(label_prenom, 1, 0, 1, 1);

        lineEdit_prenom = new QLineEdit(groupBox);
        lineEdit_prenom->setObjectName("lineEdit_prenom");

        gridLayout->addWidget(lineEdit_prenom, 1, 1, 1, 1);

        label_nom = new QLabel(groupBox);
        label_nom->setObjectName("label_nom");

        gridLayout->addWidget(label_nom, 2, 0, 1, 1);

        lineEdit_nom = new QLineEdit(groupBox);
        lineEdit_nom->setObjectName("lineEdit_nom");

        gridLayout->addWidget(lineEdit_nom, 2, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pushButton_afficher = new QPushButton(groupBox);
        pushButton_afficher->setObjectName("pushButton_afficher");

        verticalLayout->addWidget(pushButton_afficher);

        pushButton_quitter = new QPushButton(groupBox);
        pushButton_quitter->setObjectName("pushButton_quitter");

        verticalLayout->addWidget(pushButton_quitter);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 2, 3, 1);


        verticalLayout_2->addWidget(groupBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 500, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des clients", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Ajouter un Client", nullptr));
        label_cin->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        lineEdit_cin->setPlaceholderText(QCoreApplication::translate("MainWindow", "012532", nullptr));
        label_prenom->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        lineEdit_prenom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mohamed", nullptr));
        label_nom->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        lineEdit_nom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Jdidi", nullptr));
        pushButton_afficher->setText(QCoreApplication::translate("MainWindow", "Afficher", nullptr));
        pushButton_quitter->setText(QCoreApplication::translate("MainWindow", "Quitter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
