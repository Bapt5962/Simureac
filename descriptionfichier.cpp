#include "descriptionfichier.h"
#include "ui_descriptionfichier.h"

DescriptionFichier::DescriptionFichier(QString titre, QString description, QString chemin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DescriptionFichier)
{
    ui->setupUi(this);
    ui->Nom->setPlainText(titre);
    ui->Description->setPlainText(description);
    cheminFichier = chemin;
}

DescriptionFichier::~DescriptionFichier()
{
    delete ui;
}

void DescriptionFichier::cocher(bool checked)
{
    ui->Case->setChecked(checked);
}

bool DescriptionFichier::estCoche() const
{
    return ui->Case->isChecked();
}

bool DescriptionFichier::contient(QString recherche)
{
    return ui->Nom->toPlainText().toLower().contains(recherche) || ui->Description->toPlainText().toLower().contains(recherche);
}

void DescriptionFichier::on_Case_toggled()
{
    if(ui->Case->isChecked())
    {
        emit coche(cheminFichier);
    }
}

QString DescriptionFichier::getCheminFichier() const
{
    return cheminFichier;
}
