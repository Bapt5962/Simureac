#include "dialogimporterparticule.h"
#include "ui_dialogimporterparticule.h"

DialogImporterParticule::DialogImporterParticule(General* generalU, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImporterParticule)
{
    ui->setupUi(this);

    enSuppression = false;

    general = generalU;

    liste = new QWidget;
    ui->Liste->setWidget(liste);

    QDir dossier("data/bibliotheque/particules");
    QStringList particules = dossier.entryList(QStringList() << "*.srpart",QDir::Files);
    for (int p(0); p < particules.size(); p++) {
        FileText chargementDescriptions;
        if(chargementDescriptions.load("data/bibliotheque/particules/" + particules[p]))
        {
            descriptions.append(new DescriptionFichier(chargementDescriptions[1], chargementDescriptions[0], particules[p], liste));
        }
    }

    refaireListe();

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogImporterParticule::~DialogImporterParticule()
{
    enSuppression = true;
    qDeleteAll(descriptions);
    delete liste;
    delete ui;
    emit dialogQuit(1);
}

void DialogImporterParticule::on_Importer_clicked()
{
    QString retour("");
    int nombreImporte(0);
    for (int l(0); l < descriptions.size(); l++) {
        if(descriptions[l]->estCoche())
        {
            nombreImporte++;
            FileText chargementParticule;
            if(chargementParticule.load("data/bibliotheque/particules/" + descriptions[l]->getCheminFichier()))
            {
                if(chargementParticule.size() >= 11)
                {
                    DonneeParticule importation(general->nombreParticule());
                    importation.setNom(chargementParticule[0]);
                    importation.setFormule(chargementParticule[1]);
                    importation.setCouleurParticule(QColor(chargementParticule[2].toInt(), chargementParticule[3].toInt(), chargementParticule[4].toInt()));
                    importation.setEtat(chargementParticule[5].toInt());
                    importation.setPHValeur(chargementParticule[6].toInt());
                    importation.setRouge(chargementParticule[7].toDouble());
                    importation.setVert(chargementParticule[8].toDouble());
                    importation.setBleu(chargementParticule[9].toDouble());
                    importation.setConductivite(chargementParticule[10].toDouble());
                    importation.setPossedeUnFichier(true);
                    importation.setNomDeFichier(descriptions[l]->getCheminFichier().chopped(7));
                    general->addParticule(importation);
                }
                else
                {
                    retour += "\n" + descriptions[l]->getCheminFichier() + " n'a pas pu être chargé (Données incomplètes)";
                }
            }
            else
            {
                retour += "\n" + descriptions[l]->getCheminFichier() + " n'a pas pu être chargé (Impossiblr d'ouvrir le fichier)";
            }
        }
    }
    if(nombreImporte != 0)
    {
        if(retour.isEmpty())
        {
            QMessageBox::information(this, "Chargement", "Toutes les particules ont été chargées");
        }
        else
        {
            retour.remove(0, 1);
            QMessageBox::critical(this, "Erreur(s)", retour);
        }
    }
    close();
}

void DialogImporterParticule::on_Annuler_clicked()
{
    close();
}

void DialogImporterParticule::on_Recherche_textEdited()
{
    refaireListe();
}

void DialogImporterParticule::refaireListe()
{
    if(enSuppression)
    {
        return;
    }
    int largeur(ui->Liste->size().width());
    if(largeur < 430)
    {
        largeur = 430;
    }
    liste->setFixedSize(largeur - 25, 50 * descriptions.size());
    int lAffiche(0);
    for (int l(0); l < descriptions.size(); l++) {
        if(descriptions[l]->contient(ui->Recherche->text().toLower()))
        {
            descriptions[l]->show();
            descriptions[l]->move(0, 50 * lAffiche);
            descriptions[l]->setFixedSize(liste->size().width(), 50);
            lAffiche++;
        }
        else
        {
            descriptions[l]->hide();
        }
    }
}

void DialogImporterParticule::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    refaireListe();
}
