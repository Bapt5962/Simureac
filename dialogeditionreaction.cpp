#include "dialogeditionreaction.h"
#include "ui_dialogeditionreaction.h"

DialogEditionReaction::DialogEditionReaction(DonneeReaction copieReaction, QList<DonneeParticule*> particulesListe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditionReaction),
    copie(copieReaction)
{
    ui->setupUi(this);

    enSuppression = false;

    particules = particulesListe;

    //Construction de la zone "réaction"

    layoutReaction = new QVBoxLayout;
    ui->Reaction->setLayout(layoutReaction);
    tabs = new QTabWidget;
    layoutReaction->addWidget(tabs);
    ajouterParticule = new QPushButton("Ajouter une particule");
    layoutReaction->addWidget(ajouterParticule);

    tabReactif = new QScrollArea;
    tabs->addTab(tabReactif, "Réactifs");
    sousTabReactif = new QWidget;
    tabReactif->setWidget(sousTabReactif);
    tabReactif->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tabProduit = new QScrollArea;
    tabs->addTab(tabProduit, "Produits");
    sousTabProduit = new QWidget;
    tabProduit->setWidget(sousTabProduit);
    tabProduit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tabCatalyseur = new QScrollArea;
    tabs->addTab(tabCatalyseur, "Catalyseurs");
    sousTabCatalyseur = new QWidget;
    tabCatalyseur->setWidget(sousTabCatalyseur);
    tabCatalyseur->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(ajouterParticule, SIGNAL(clicked()), this, SLOT(ajouterParticuleClic()));

    //Entrée des valeurs de la zone "réaction"

    for (int l(0); l < copie.getReactifs().size(); l++) {
        ListeParticuleReaction* nouvelleParticule(new ListeParticuleReaction(false, particules, sousTabReactif));
        nouvelleParticule->setValeurs(copie.getProportionReactifs()[l], copie.getReactifs()[l]->getID());
        listeReactifs.append(nouvelleParticule);
        connect(nouvelleParticule, SIGNAL(supprime()), this, SLOT(supprimerParticuleClic()));
    }
    for (int l(0); l < copie.getProduits().size(); l++) {
        ListeParticuleReaction* nouvelleParticule(new ListeParticuleReaction(false, particules, sousTabProduit));
        nouvelleParticule->setValeurs(copie.getProportionProduits()[l], copie.getProduits()[l]->getID());
        listeProduits.append(nouvelleParticule);
        connect(nouvelleParticule, SIGNAL(supprime()), this, SLOT(supprimerParticuleClic()));
    }
    for (int l(0); l < copie.getCatalyseurs().size(); l++) {
        ListeParticuleReaction* nouvelleParticule(new ListeParticuleReaction(true, particules, sousTabCatalyseur));
        nouvelleParticule->setValeurs(copie.getEfficaciteCatalyseurs()[l], copie.getCatalyseurs()[l]->getID());
        listeCatalyseurs.append(nouvelleParticule);
        connect(nouvelleParticule, SIGNAL(supprime()), this, SLOT(supprimerParticuleClic()));
    }

    //Entrée des valeurs du reste de la réaction

    if(copie.getFacteurPreExp() != 200 || copie.getEnergieActivation() != 0)
    {
        ui->Cinetique->setChecked(true);
    }
    ui->CinetiqueA->setValue(copie.getFacteurPreExp());
    ui->CinetiqueEa->setValue(copie.getEnergieActivation());
    if(copie.getDeltaTemperature() != 0)
    {
        ui->Temperature->setChecked(true);
    }
    ui->TemperatureJ->setValue(copie.getDeltaTemperature());
    if(!(copie.getPHA() == 1 && copie.getPHB() == -99))
    {
        ui->pH->setChecked(true);
    }
    ui->pHA->setValue(copie.getPHA());
    ui->pHB->setValue(copie.getPHB());

    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(this, SIGNAL(accepted()), this, SLOT(renvoyerDonnees()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(refaireListe()));

    tabs->resize(438, 100);

    refaireListe();

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogEditionReaction::~DialogEditionReaction()
{
    enSuppression = true;
    qDeleteAll(listeReactifs);
    qDeleteAll(listeProduits);
    qDeleteAll(listeCatalyseurs);
    delete sousTabReactif;
    delete sousTabProduit;
    delete sousTabCatalyseur;
    delete tabReactif;
    delete tabProduit;
    delete tabCatalyseur;
    delete tabs;
    delete ajouterParticule;
    delete layoutReaction;
    delete ui;
}

void DialogEditionReaction::ajouterParticuleClic()
{
    ListeParticuleReaction* nouvelleParticule;
    switch(tabs->currentIndex())
    {
    case 0:
        nouvelleParticule = new ListeParticuleReaction(false, particules, sousTabReactif);
        listeReactifs.append(nouvelleParticule);
        copie.addReactif(1, particules[0]);
        break;
    case 1:
        nouvelleParticule = new ListeParticuleReaction(false, particules, sousTabProduit);
        listeProduits.append(nouvelleParticule);
        copie.addProduit(1, particules[0]);
        break;
    case 2:
        nouvelleParticule = new ListeParticuleReaction(true, particules, sousTabCatalyseur);
        listeCatalyseurs.append(nouvelleParticule);
        copie.addCatalyseur(0, particules[0]);
        break;
    }
    nouvelleParticule->setFixedSize(100, 100);
    nouvelleParticule->show();
    connect(nouvelleParticule, SIGNAL(supprime()), this, SLOT(supprimerParticuleClic()));
    refaireListe();
}

void DialogEditionReaction::supprimerParticuleClic()
{
    for (int l(listeReactifs.size() - 1); l >= 0; l--) {
        if(listeReactifs[l]->getASupprimer())
        {
            delete listeReactifs[l];
            listeReactifs.removeAt(l);
            copie.supprimerReactif(l);
        }
    }
    for (int l(listeProduits.size() - 1); l >= 0; l--) {
        if(listeProduits[l]->getASupprimer())
        {
            delete listeProduits[l];
            listeProduits.removeAt(l);
            copie.supprimerProduit(l);
        }
    }
    for (int l(listeCatalyseurs.size() - 1); l >= 0; l--) {
        if(listeCatalyseurs[l]->getASupprimer())
        {
            delete listeCatalyseurs[l];
            listeCatalyseurs.removeAt(l);
            copie.supprimerCatalyseur(l);
        }
    }
    refaireListe();
}

void DialogEditionReaction::on_Temperature_clicked()
{
    if(!ui->Temperature->isChecked())
    {
        ui->TemperatureJ->setValue(0);
    }
}

void DialogEditionReaction::on_pH_clicked()
{
    if(!ui->pH->isChecked())
    {
        ui->pHA->setValue(-1);
        ui->pHB->setValue(-99);
    }
}

void DialogEditionReaction::renvoyerDonnees()
{
    for (int l(0); l < listeReactifs.size(); l++) {
        copie.setProportionReactif(l, qRound(listeReactifs[l]->getNombre()));
        copie.setReactif(l, particules[listeReactifs[l]->getID()]);
    }
    for (int l(0); l < listeProduits.size(); l++) {
        copie.setProportionProduit(l, qRound(listeProduits[l]->getNombre()));
        copie.setProduit(l, particules[listeProduits[l]->getID()]);
    }
    for (int l(0); l < listeCatalyseurs.size(); l++) {
        copie.setEfficaciteCatalyseur(l, listeCatalyseurs[l]->getNombre());
        copie.setCatalyseur(l, particules[listeCatalyseurs[l]->getID()]);
    }
    if(ui->Cinetique->isChecked())
    {
        copie.setFacteurPreExp(ui->CinetiqueA->value());
        copie.setEnergieActivation(ui->CinetiqueEa->value());
    }
    else
    {
        copie.setFacteurPreExp(200);
        copie.setEnergieActivation(0);
    }
    copie.setDeltaTemperature(ui->TemperatureJ->value());
    copie.setPH(ui->pHA->value(), ui->pHB->value());
    emit sauvegarder(copie);
}

void DialogEditionReaction::refaireListe()
{
    if(enSuppression)
    {
        return;
    }
    int largeur(tabs->size().width());
    if(largeur < 430)
    {
        largeur = 430;
    }
    sousTabReactif->setFixedSize(largeur - 25, 50 * listeReactifs.size());
    for (int l(0); l < listeReactifs.size(); l++) {
        listeReactifs[l]->move(0, 50 * l);
        listeReactifs[l]->setFixedSize(sousTabReactif->size().width(), 50);
    }
    sousTabProduit->setFixedSize(largeur - 25, 50 * listeProduits.size());
    for (int l(0); l < listeProduits.size(); l++) {
        listeProduits[l]->move(0, 50 * l);
        listeProduits[l]->setFixedSize(sousTabProduit->size().width(), 50);
    }
    sousTabCatalyseur->setFixedSize(largeur - 25, 50 * listeCatalyseurs.size());
    for (int l(0); l < listeCatalyseurs.size(); l++) {
        listeCatalyseurs[l]->move(0, 50 * l);
        listeCatalyseurs[l]->setFixedSize(sousTabCatalyseur->size().width(), 50);
    }
}

void DialogEditionReaction::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    refaireListe();
}
