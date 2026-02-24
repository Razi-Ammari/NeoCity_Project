#include "affichagedialog.h"
#include "ui_affichagedialog.h"

AffichageDialog::AffichageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AffichageDialog)
{
    ui->setupUi(this);

    // Définir le titre de la fenêtre
    setWindowTitle("Afficher un Client");
}

AffichageDialog::~AffichageDialog()
{
    delete ui;
}

void AffichageDialog::setData(const QString &cin, const QString &prenom, const QString &nom)
{
    ui->lineEdit_cin->setText(cin);
    ui->lineEdit_prenom->setText(prenom);
    ui->lineEdit_nom->setText(nom);

    // Optionnel : rendre les champs en lecture seule
    ui->lineEdit_cin->setReadOnly(true);
    ui->lineEdit_prenom->setReadOnly(true);
    ui->lineEdit_nom->setReadOnly(true);
}
