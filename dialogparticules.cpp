#include "dialogparticules.h"
#include "ui_dialogparticules.h"

DialogParticules::DialogParticules(General *generalT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogParticules)
{
    ignorerPremierResize = true;
    ui->setupUi(this);
    generalCopie = generalT;
    contenuListe = new QWidget;
    ui->Liste->setWidget(contenuListe);

    refaireListe();

    QResizeEvent eventArtificiel(size(), size());
    resizeEvent(&eventArtificiel);

    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(this, SIGNAL(accepted()), this, SLOT(renvoyerDonnees()));
    connect(this, SIGNAL(rejected()), this, SLOT(envoyerAnnuler()));

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogParticules::~DialogParticules()
{
    delete ui;
    qDeleteAll(particules);
    delete contenuListe;
}

void DialogParticules::refaireListe()
{
    qDeleteAll(particules);
    particules.clear();
    for (int IDp(0); IDp < generalCopie->nombreParticule(); IDp++) {
        particules.append(new ListeDialogEdition(IDp, generalCopie->getParticule(IDp)->getNom(), generalCopie->getParticule(IDp)->getCouleurParticule()));
        particules[particules.size() - 1]->setParent(contenuListe);
        particules[particules.size() - 1]->move(0, IDp * 45);
        particules[particules.size() - 1]->resize(681, 45);
        particules[particules.size() - 1]->show();
        connect(particules[particules.size() - 1], SIGNAL(boutonClic(int)), this, SLOT(boutonClic(int))); //Quand le bouton "Editer" est pressé, appele la fonction boutonClic ci-dessous
    }
    QResizeEvent eventArtificiel(size(), size());
    resizeEvent(&eventArtificiel);
}

void DialogParticules::boutonClic(int ID)
{
    IDEdition = ID;
    DonneeParticule copie(ID);
    copie.setTout(*generalCopie->getParticule(ID));
    dialogEdition = new DialogEditionParticule(copie, this);
    connect(dialogEdition, SIGNAL(sauvegarder(DonneeParticule)), this, SLOT(donneeParticuleRecu(DonneeParticule)));
    dialogEdition->setModal(true);
    dialogEdition->show();
}

void DialogParticules::on_Ajout_clicked()
{
    DonneeParticule nouveau(generalCopie->nombreParticule());
    nouveau.setNom("Nouvelle particule");
    nouveau.setFormule("Formule");
    nouveau.setPHValeur(0);
    nouveau.setCouleurParticule(QColor(64, 64, 64));
    generalCopie->addParticule(nouveau);
    refaireListe();
    QResizeEvent eventArtificiel(size(), size());
    resizeEvent(&eventArtificiel);
}

void DialogParticules::donneeParticuleRecu(DonneeParticule particule)
{
    generalCopie->setParticule(IDEdition, particule);
    refaireListe();
}

void DialogParticules::renvoyerDonnees()
{
    emit sauvegarder(*generalCopie);
}

void DialogParticules::envoyerAnnuler()
{
    generalCopie->vider();
}

void DialogParticules::resizeEvent(QResizeEvent *event)
{
    if(ignorerPremierResize)
    {
        ignorerPremierResize = false;
        return;
    }
    int barreVisible(0);
    if(ui->Liste->size().height() < particules.size() * 45)
    {
        barreVisible = 1; //Si la barre verticale est visible, barreVisible = 1 ; sinon, barreVisible = 0
    }
    QDialog::resizeEvent(event);
    for (int IDp(0); IDp < particules.size(); IDp++) {
        particules[IDp]->resize(ui->Liste->width() - (2 + barreVisible * 17), 45); //Change la largeur pour qu'elle soit le plus grand possible
    }
    contenuListe->setFixedSize(ui->Liste->width() - (2 + barreVisible * 17), particules.size() * 45);
}
