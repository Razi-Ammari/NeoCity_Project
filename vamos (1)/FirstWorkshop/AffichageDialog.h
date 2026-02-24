#ifndef AFFICHAGEDIALOG_H
#define AFFICHAGEDIALOG_H

#include <QDialog>

namespace Ui {
class AffichageDialog;
}

class AffichageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AffichageDialog(QWidget *parent = nullptr);
    ~AffichageDialog();

    // Méthode pour définir les données à afficher
    void setData(const QString &cin, const QString &prenom, const QString &nom);

private:
    Ui::AffichageDialog *ui;
};

#endif // AFFICHAGEDIALOG_H
