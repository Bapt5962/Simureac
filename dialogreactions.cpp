#include "dialogreactions.h"
#include "ui_dialogreactions.h"

DialogReactions::DialogReactions(General *generalT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReactions)
{
    ignorerPremierResize = true;
    ui->setupUi(this);
    generalCopie = generalT;
    contenuListe = new QWidget;
    ui->Liste->setWidget(contenuListe);

    if(generalCopie->nombreParticule() == 0)
    {
        ui->Ajout->setDisabled(true);
    }

    refaireListe();

    QResizeEvent eventArtificiel(size(), size());
    resizeEvent(&eventArtificiel);

    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(this, SIGNAL(accepted()), this, SLOT(renvoyerDonnees()));
    connect(this, SIGNAL(rejected()), this, SLOT(envoyerAnnuler()));

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogReactions::~DialogReactions()
{
    delete ui;
    qDeleteAll(reactions);
    delete contenuListe;
}

void DialogReactions::refaireListe()
{
    qDeleteAll(reactions);
    reactions.clear();
    for (int IDp(0); IDp < generalCopie->nombreReaction(); IDp++) {
        reactions.append(new ListeDialogEdition(IDp, generalCopie->getReaction(IDp)->formaterEquation(), generalCopie->getReaction(IDp)->formaterCouleurProduits()));
        reactions[reactions.size() - 1]->setParent(contenuListe);
        reactions[reactions.size() - 1]->move(0, IDp * 45);
        reactions[reactions.size() - 1]->resize(681, 45);
        reactions[reactions.size() - 1]->show();
        connect(reactions[reactions.size() - 1], SIGNAL(boutonClic(int)), this, SLOT(boutonClic(int))); //Quand le bouton "Editer" est pressé, appele la fonction boutonClic ci-dessous
    }
    QResizeEvent eventArtificiel(size(), size());
    resizeEvent(&eventArtificiel);
}

void DialogReactions::boutonClic(int ID)
{
    IDEdition = ID;
    DonneeReaction copie(ID);
    copie.setTout(*generalCopie->getReaction(ID));
    dialogEdition = new DialogEditionReaction(copie, generalCopie->getParticules(), this);
    connect(dialogEdition, SIGNAL(sauvegarder(DonneeReaction)), this, SLOT(donneeReactionRecu(DonneeReaction)));
    dialogEdition->setModal(true);
    dialogEdition->setWindowModality(Qt::WindowModal);
    dialogEdition->show();
}

void DialogReactions::on_Ajout_clicked()
{
    DonneeReaction nouveau(generalCopie->nombreReaction());
    generalCopie->addReaction(nouveau);
    refaireListe();
    QResizeEvent eventArtificiel(size(), size());
    resizeEvent(&eventArtificiel);
}

void DialogReactions::donneeReactionRecu(DonneeReaction reaction)
{
    generalCopie->setReaction(IDEdition, reaction);
    refaireListe();
}

void DialogReactions::renvoyerDonnees()
{
    emit sauvegarder(*generalCopie);
}

void DialogReactions::envoyerAnnuler()
{
    generalCopie->vider();
}

void DialogReactions::resizeEvent(QResizeEvent *event)
{
    if(ignorerPremierResize)
    {
        ignorerPremierResize = false;
        return;
    }
    int barreVisible(0);
    if(ui->Liste->size().height() < reactions.size() * 45)
    {
        barreVisible = 1; //Si la barre verticale est visible, barreVisible = 1 ; sinon, barreVisible = 0
    }
    QDialog::resizeEvent(event);
    for (int IDp(0); IDp < reactions.size(); IDp++) {
        reactions[IDp]->resize(ui->Liste->width() - (2 + barreVisible * 17), 45); //Change la largeur pour qu'elle soit le plus grand possible
    }
    contenuListe->setFixedSize(ui->Liste->width() - (2 + barreVisible * 17), reactions.size() * 45);
}
