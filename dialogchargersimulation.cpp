#include "dialogchargersimulation.h"
#include "ui_dialogchargersimulation.h"

DialogChargerSimulation::DialogChargerSimulation(General* generalU, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChargerSimulation)
{
    ui->setupUi(this);

    enSuppression = false;

    general = generalU;
    importation = nullptr;

    liste = new QWidget;
    ui->Liste->setWidget(liste);

    QDir dossier("data/bibliotheque/simulations");
    QStringList simulations = dossier.entryList(QStringList() << "*.srsim",QDir::Files);
    for (int p(0); p < simulations.size(); p++) {
        FileText chargementDescriptions;
        if(chargementDescriptions.load("data/bibliotheque/simulations/" + simulations[p]))
        {
            QString description;
            int lecture(1);
            while (chargementDescriptions[lecture] != "ENDOFTHEDESCRIPTION" && lecture < chargementDescriptions.size() - 1) {
                description += chargementDescriptions[lecture] + "\n";
                lecture++;
            }
            description.chop(1);
            if(lecture >= chargementDescriptions.size() - 1)
            {
                descriptions.append(new DescriptionFichier(chargementDescriptions[0], "Fichier corrompu", simulations[p], liste));
                descriptions.last()->setDisabled(true);
            }
            else
            {
                descriptions.append(new DescriptionFichier(chargementDescriptions[0], description, simulations[p], liste));
                connect(descriptions.last(), SIGNAL(coche(QString)), this, SLOT(simulationCoche(QString)));
            }
        }
    }

    refaireListe();

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogChargerSimulation::~DialogChargerSimulation()
{
    enSuppression = true;
    qDeleteAll(descriptions);
    delete liste;
    delete ui;
    emit dialogQuit(3, importation);
}

void DialogChargerSimulation::on_Importer_clicked()
{
    QString chemin("");
    for (int l(0); l < descriptions.size(); l++) {
        if(descriptions[l]->estCoche())
        {
            chemin = descriptions[l]->getCheminFichier();
        }
    }
    if(chemin.isEmpty())
    {
        close();
        return;
    }
    FileText charger;
    if(!charger.load("data/bibliotheque/simulations/" + chemin))
    {
        QMessageBox::critical(this, "Erreur", "la simulation n'a pas pu être chargé (Impossiblr d'ouvrir le fichier)");
        close();
        return;
    }
    importation = new General;
    importation->setPossedeUnFichier(true);
    importation->setNomDeFichier(chemin.chopped(6));
    importation->setNom(charger[0]);
    QString description;
    int lecture(1);
    while (charger[lecture] != "ENDOFTHEDESCRIPTION" && lecture < charger.size() - 1) {
        description += charger[lecture] + "\n";
        lecture++;
    }
    importation->setDescription(description);
    lecture++;
    QStringList particulesACharger;
    while (charger[lecture] != "ENDPARTICLES" && lecture < charger.size() - 1) {
        particulesACharger.append(charger[lecture]);
        lecture++;
    }
    QString retourParticule("");
    for (int p(0); p < particulesACharger.size(); p++) {
        FileText chargementParticule;
        if(chargementParticule.load("data/bibliotheque/particules/" + particulesACharger[p] + ".srpart"))
        {
            if(chargementParticule.size() >= 11)
            {
                DonneeParticule importationP(importation->nombreParticule());
                importationP.setNom(chargementParticule[0]);
                importationP.setFormule(chargementParticule[1]);
                importationP.setCouleurParticule(QColor(chargementParticule[2].toInt(), chargementParticule[3].toInt(), chargementParticule[4].toInt()));
                importationP.setEtat(chargementParticule[5].toInt());
                importationP.setPHValeur(chargementParticule[6].toInt());
                importationP.setRouge(chargementParticule[7].toDouble());
                importationP.setVert(chargementParticule[8].toDouble());
                importationP.setBleu(chargementParticule[9].toDouble());
                importationP.setConductivite(chargementParticule[10].toDouble());
                importationP.setPossedeUnFichier(true);
                importationP.setNomDeFichier(particulesACharger[p]);
                importation->addParticule(importationP);
                if(importationP.getPHValeur() != 0)
                {
                    importation->setPH(true);
                }
            }
            else
            {
                retourParticule += "\n" + particulesACharger[p] + ".srpart n'a pas pu être chargé (Données incomplètes)";
            }
        }
        else
        {
            retourParticule += "\n" + particulesACharger[p] + ".srpart n'a pas pu être chargé (Impossiblr d'ouvrir le fichier)";
        }
    }
    if(!retourParticule.isEmpty())
    {
        retourParticule.remove(0, 1);
    }
    lecture++;
    QString retourReaction("");
    QList<QStringList> reactions;
    reactions.append(QStringList());
    while (charger[lecture] != "ENDREACTIONS" && lecture < charger.size() - 1) {
        if(charger[lecture] == "ENDREACTION")
        {
            reactions.append(QStringList());
        }
        else
        {
            reactions.last().append(charger[lecture]);
        }
        lecture++;
    }
    reactions.removeLast();
    for (int r(0); r < reactions.size(); r++) {
        if(reactions[r].size() < 12)
        {
            retourReaction += "\nLa réaction n°" + QString::number(r) + " n'a pas pu être chargée";
            continue;
        }
        int lectureReaction(0);
        DonneeReaction importationR(importation->nombreReaction());
        QStringList nombres, ids;
        //Reactifs
        while (reactions[r][lectureReaction] != "ENDLIST" && lectureReaction < reactions[r].size() - 1) {
            nombres.append(reactions[r][lectureReaction]);
            lectureReaction++;
        }
        lectureReaction++;
        while (reactions[r][lectureReaction] != "ENDLIST" && lectureReaction < reactions[r].size() - 1) {
            ids.append(reactions[r][lectureReaction]);
            lectureReaction++;
        }
        lectureReaction++;
        for (int p(0); p < nombres.size(); p++) {
            importationR.addReactif(nombres[p].toInt(), importation->getParticule(ids[p].toInt()));
        }
        nombres.clear();
        ids.clear();
        //Produits
        while (reactions[r][lectureReaction] != "ENDLIST" && lectureReaction < reactions[r].size() - 1) {
            nombres.append(reactions[r][lectureReaction]);
            lectureReaction++;
        }
        lectureReaction++;
        while (reactions[r][lectureReaction] != "ENDLIST" && lectureReaction < reactions[r].size() - 1) {
            ids.append(reactions[r][lectureReaction]);
            lectureReaction++;
        }
        lectureReaction++;
        for (int p(0); p < nombres.size(); p++) {
            importationR.addProduit(nombres[p].toInt(), importation->getParticule(ids[p].toInt()));
        }
        nombres.clear();
        ids.clear();
        //Catalyseurs
        while (reactions[r][lectureReaction] != "ENDLIST" && lectureReaction < reactions[r].size() - 1) {
            nombres.append(reactions[r][lectureReaction]);
            lectureReaction++;
        }
        lectureReaction++;
        while (reactions[r][lectureReaction] != "ENDLIST" && lectureReaction < reactions[r].size() - 1) {
            ids.append(reactions[r][lectureReaction]);
            lectureReaction++;
        }
        lectureReaction++;
        for (int p(0); p < nombres.size(); p++) {
            importationR.addCatalyseur(nombres[p].toDouble(), importation->getParticule(ids[p].toInt()));
        }
        //Autres valeurs
        importationR.setFacteurPreExp(reactions[r][lectureReaction].toDouble());
        lectureReaction++;
        importationR.setEnergieActivation(reactions[r][lectureReaction].toDouble());
        lectureReaction++;
        importationR.setDeltaTemperature(reactions[r][lectureReaction].toDouble());
        lectureReaction++;
        importationR.setPH(reactions[r][lectureReaction].toDouble(), reactions[r][lectureReaction + 1].toDouble());
        importation->addReaction(importationR);
    }
    close();
}

void DialogChargerSimulation::on_Annuler_clicked()
{
    close();
}

void DialogChargerSimulation::on_Recherche_textEdited()
{
    refaireListe();
}

void DialogChargerSimulation::simulationCoche(QString chemin)
{
    for (int l(0); l < descriptions.size(); l++) {
        if(descriptions[l]->getCheminFichier() != chemin)
        {
            descriptions[l]->cocher(false);
        }
    }
}

void DialogChargerSimulation::refaireListe()
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
    liste->setFixedSize(largeur - 25, 100 * descriptions.size());
    int lAffiche(0);
    for (int l(0); l < descriptions.size(); l++) {
        if(descriptions[l]->contient(ui->Recherche->text().toLower()))
        {
            descriptions[l]->show();
            descriptions[l]->move(0, 100 * lAffiche);
            descriptions[l]->setFixedSize(liste->size().width(), 100);
            lAffiche++;
        }
        else
        {
            descriptions[l]->hide();
        }
    }
}

void DialogChargerSimulation::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    refaireListe();
}
