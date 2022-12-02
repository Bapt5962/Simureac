#include "dialogsimulation.h"
#include "ui_dialogsimulation.h"

DialogSimulation::DialogSimulation(General *generalT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSimulation)
{
    ui->setupUi(this);
    generalCopie = generalT;
    if(generalCopie->getPH())
    {
        ui->pH->setDisabled(true);
    }

    connect(this, SIGNAL(accepted()), this, SLOT(renvoyerDonnees()));
    connect(this, SIGNAL(rejected()), this, SLOT(envoyerAnnuler()));

    this->setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogSimulation::~DialogSimulation()
{
    delete ui;
}

void DialogSimulation::on_pH_clicked()
{
    int confirmaton(QMessageBox::question(this, "Action irréversible", "Activer le mode pH ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No));
    if(confirmaton == QMessageBox::Yes)
    {
        generalCopie->setPH(true);
        ui->pH->setDisabled(true);

        int tailleParticules(generalCopie->nombreParticule());

        DonneeParticule oxonium(tailleParticules);
        oxonium.setNom("Ion Hydroxonium");
        oxonium.setFormule("H3O+");
        oxonium.setCouleurParticule(QColor(196, 98, 98));
        oxonium.setConductivite(34.97);
        oxonium.setPHValeur(-1);
        generalCopie->addParticule(oxonium);

        DonneeParticule hydroxyde(tailleParticules + 1);
        hydroxyde.setNom("Ion Hydroxyde");
        hydroxyde.setFormule("HO-");
        hydroxyde.setCouleurParticule(QColor(98, 98, 196));
        hydroxyde.setConductivite(19.85);
        hydroxyde.setPHValeur(1);
        generalCopie->addParticule(hydroxyde);

        DonneeReaction reaction(generalCopie->nombreReaction());
        reaction.addReactif(1, generalCopie->getParticule(tailleParticules));
        reaction.addReactif(1, generalCopie->getParticule(tailleParticules + 1));
        generalCopie->addReaction(reaction);
    }
}

void DialogSimulation::on_Particules_clicked()
{
    dialogParticules = new DialogParticules(generalCopie->creerCopie(), this);
    connect(dialogParticules, SIGNAL(sauvegarder(General)), this, SLOT(donneeRecu(General)));
    dialogParticules->setModal(true);
    dialogParticules->setWindowModality(Qt::WindowModal);
    dialogParticules->show();
}

void DialogSimulation::on_Interactions_clicked()
{
    dialogReactions = new DialogReactions(generalCopie->creerCopie(), this);
    connect(dialogReactions, SIGNAL(sauvegarder(General)), this, SLOT(donneeRecu(General)));
    dialogReactions->setModal(true);
    dialogReactions->setWindowModality(Qt::WindowModal);
    dialogReactions->show();
}

void DialogSimulation::donneeRecu(General simulation)
{
    generalCopie->setTout(simulation);
}

void DialogSimulation::envoyerAnnuler()
{
    generalCopie->vider();
    emit annuler();
}


void DialogSimulation::renvoyerDonnees()
{
    for (int r(0); r < generalCopie->nombreReaction(); r++) {
        generalCopie->getReaction(r)->oublierLimiteExp();
    }
    emit sauvegarder(*generalCopie);
}
