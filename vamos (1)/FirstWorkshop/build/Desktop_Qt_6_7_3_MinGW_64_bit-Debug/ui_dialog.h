/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AffichageDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_cin;
    QLineEdit *lineEdit_cin;
    QLabel *label_prenom;
    QLineEdit *lineEdit_prenom;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;

    void setupUi(QDialog *AffichageDialog)
    {
        if (AffichageDialog->objectName().isEmpty())
            AffichageDialog->setObjectName("AffichageDialog");
        AffichageDialog->resize(450, 250);
        verticalLayout = new QVBoxLayout(AffichageDialog);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(AffichageDialog);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label_cin = new QLabel(groupBox);
        label_cin->setObjectName("label_cin");

        gridLayout->addWidget(label_cin, 0, 0, 1, 1);

        lineEdit_cin = new QLineEdit(groupBox);
        lineEdit_cin->setObjectName("lineEdit_cin");
        lineEdit_cin->setReadOnly(true);

        gridLayout->addWidget(lineEdit_cin, 0, 1, 1, 1);

        label_prenom = new QLabel(groupBox);
        label_prenom->setObjectName("label_prenom");

        gridLayout->addWidget(label_prenom, 1, 0, 1, 1);

        lineEdit_prenom = new QLineEdit(groupBox);
        lineEdit_prenom->setObjectName("lineEdit_prenom");
        lineEdit_prenom->setReadOnly(true);

        gridLayout->addWidget(lineEdit_prenom, 1, 1, 1, 1);

        label_nom = new QLabel(groupBox);
        label_nom->setObjectName("label_nom");

        gridLayout->addWidget(label_nom, 2, 0, 1, 1);

        lineEdit_nom = new QLineEdit(groupBox);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setReadOnly(true);

        gridLayout->addWidget(lineEdit_nom, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBox);


        retranslateUi(AffichageDialog);

        QMetaObject::connectSlotsByName(AffichageDialog);
    } // setupUi

    void retranslateUi(QDialog *AffichageDialog)
    {
        AffichageDialog->setWindowTitle(QCoreApplication::translate("AffichageDialog", "Afficher un Client", nullptr));
        groupBox->setTitle(QCoreApplication::translate("AffichageDialog", "Afficher un client", nullptr));
        label_cin->setText(QCoreApplication::translate("AffichageDialog", "CIN", nullptr));
        label_prenom->setText(QCoreApplication::translate("AffichageDialog", "Pr\303\251nom", nullptr));
        label_nom->setText(QCoreApplication::translate("AffichageDialog", "Nom", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AffichageDialog: public Ui_AffichageDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
