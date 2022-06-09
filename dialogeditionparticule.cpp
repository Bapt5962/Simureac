#include "dialogeditionparticule.h"
#include "ui_dialogeditionparticule.h"

DialogEditionParticule::DialogEditionParticule(DonneeParticule copieParticule, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditionParticule),
    copie(copieParticule)
{
    ui->setupUi(this);

    ui->Nom->setText(copie.getNom());
    ui->Formule->setText(copie.getFormule());
    switch (copie.getEtat()) {
    case 0:
        ui->Aqueux->setChecked(true);
        break;
    case 1:
        ui->Solide->setChecked(true);
        break;
    case 2:
        ui->Liquide->setChecked(true);
        break;
    case 3:
        ui->Gazeux->setChecked(true);
        break;
    }
    if(copie.getConductivite() != 0)
    {
        ui->Conductivite->setChecked(true);
    }
    ui->ConductiviteValeur->setValue(copie.getConductivite());
    if(copie.getRouge() != 0 || copie.getVert() != 0 || copie.getBleu() != 0)
    {
        ui->Absorbance->setChecked(true);
    }
    ui->Rouge->setValue(copie.getRouge());
    ui->Vert->setValue(copie.getVert());
    ui->Bleu->setValue(copie.getBleu());

    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(this, SIGNAL(accepted()), this, SLOT(renvoyerDonnees()));

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogEditionParticule::~DialogEditionParticule()
{
    delete ui;
}

void DialogEditionParticule::on_Conductivite_clicked()
{
    if(!ui->Conductivite->isChecked())
    {
        ui->ConductiviteValeur->setValue(0);
    }
}

void DialogEditionParticule::on_Absorbance_clicked()
{
    if(!ui->Absorbance->isChecked())
    {
        ui->Rouge->setValue(0);
        ui->Vert->setValue(0);
        ui->Bleu->setValue(0);
    }
}

void DialogEditionParticule::on_Couleur_clicked()
{
    copie.setCouleurParticule(QColorDialog::getColor(copie.getCouleurParticule(), this, "Nouvelle couleur"));
}

void DialogEditionParticule::renvoyerDonnees()
{
    copie.setNom(ui->Nom->text());
    if(copie.getNom() == "ENDPARTICLES")
    {
        copie.setNom("ENDPARTICLES_");
    }
    copie.setFormule(ui->Formule->text());
    if(copie.getFormule() == "ENDPARTICLES")
    {
        copie.setFormule("ENDPARTICLES_");
    }
    if(ui->Aqueux->isChecked())
    {
        copie.setEtat(0);
    }
    if(ui->Solide->isChecked())
    {
        copie.setEtat(1);
    }
    if(ui->Liquide->isChecked())
    {
        copie.setEtat(2);
    }
    if(ui->Gazeux->isChecked())
    {
        copie.setEtat(3);
    }
    copie.setConductivite(ui->ConductiviteValeur->value());
    copie.setRouge(ui->Rouge->value());
    copie.setVert(ui->Vert->value());
    copie.setBleu(ui->Bleu->value());
    emit sauvegarder(copie);
}
