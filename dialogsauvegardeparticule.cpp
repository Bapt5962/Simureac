#include "dialogsauvegardeparticule.h"
#include "ui_dialogsauvegardeparticule.h"

DialogSauvegardeParticule::DialogSauvegardeParticule(General* generalU, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSauvegardeParticule)
{
    ui->setupUi(this);

    general = generalU;

    for (int p(0); p < general->nombreParticule(); p++) {
        ui->Choix->addItem(general->getParticule(p)->getNom());
    }

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogSauvegardeParticule::~DialogSauvegardeParticule()
{
    delete ui;
    emit dialogQuit(0);
}

void DialogSauvegardeParticule::on_Sauvegarder_clicked()
{
    int index(ui->Choix->currentIndex());
    DonneeParticule* particule(general->getParticule(index));
    particule->setNomDeFichier(ui->Nom->text());
    particule->setPossedeUnFichier(true);
    FileText sauvegarde;
    sauvegarde.write(particule->getNom());
    sauvegarde.write(particule->getFormule());
    sauvegarde.write(QString::number(particule->getCouleurParticule().red()));
    sauvegarde.write(QString::number(particule->getCouleurParticule().green()));
    sauvegarde.write(QString::number(particule->getCouleurParticule().blue()));
    sauvegarde.write(QString::number(particule->getEtat()));
    sauvegarde.write(QString::number(particule->getPHValeur()));
    sauvegarde.write(QString::number(particule->getRouge()));
    sauvegarde.write(QString::number(particule->getVert()));
    sauvegarde.write(QString::number(particule->getBleu()));
    sauvegarde.write(QString::number(particule->getConductivite()));
    if(sauvegarde.save("data/bibliotheque/particules/" + ui->Nom->text() + ".srpart"))
    {
        QMessageBox::information(this, "Sauvegarde", "Le fichier a bien été sauvegardé");
        close();
    }
    else
    {
        QMessageBox::critical(this, "Sauvegarde", "Le fichier n'a pas pu être sauvegardé");
    }
}

void DialogSauvegardeParticule::on_Annuler_clicked()
{
    close();
}

void DialogSauvegardeParticule::on_Choix_currentIndexChanged(int index)
{
    if(general->getParticule(index)->getPossedeUnFichier())
    {
        ui->Nom->setText(general->getParticule(index)->getNomDeFichier());
    }
    else
    {
        ui->Nom->setText(general->getParticule(index)->getNom());
    }
}

void DialogSauvegardeParticule::on_Nom_textChanged()
{
    bool nomValide(true);
    QString nom(ui->Nom->text());
    if(nom.size() == 0)
    {
        nomValide = false;
    }
    if(nom.startsWith('.') || nom.startsWith(' ') || nom.endsWith('.') || nom.endsWith(' '))
    {
        nomValide = false;
    }
    if(nom.contains('\\') || nom.contains('/') || nom.contains(':') || nom.contains('*') || nom.contains('?') || nom.contains('"') || nom.contains('<') || nom.contains('>') || nom.contains('|'))
    {
        nomValide = false;
    }
    if(!nomValide)
    {
        ui->Sauvegarder->setEnabled(false);
        return;
    }
    ui->Sauvegarder->setEnabled(true);
    if(FileText::exists("data/bibliotheque/particules/" + nom + ".srpart"))
    {
        ui->Sauvegarder->setText("Sauvegarder et remplacer");
    }
    else
    {
        ui->Sauvegarder->setText("Sauvegarder");
    }
}
