#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    general = new General;

    particules = new QList<ParticuleListe*>;
    particulesConteneur = new QWidget;
    ui->elements->setWidget(particulesConteneur);
    ui->elements->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    particulesConteneur->setFixedSize(198, 64);
    refaireListe();

    simulation = new Simulation(particules, general);
    ui->simulation->setWidget(simulation);
    simulation->setFixedSize(1700, 1000);
    connect(simulation, SIGNAL(lagging(QString)), this, SLOT(lagging(QString)));
    connect(simulation, SIGNAL(pause()), this, SLOT(mettreEnPause()));

    ui->simulation->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    preferences.load("data/preferences.txt");
    if(preferences[0].endsWith("Oui"))
    {
        ui->actionSombre->setChecked(true);
    }
    if(preferences[4].endsWith("Oui"))
    {
        ui->actionCompteurFPS->setChecked(true);
        simulation->setFpsAEnvoyer(true);
    }
    if(preferences[5].endsWith("Oui"))
    {
        ui->actionPleinEcran->setChecked(true);
        showFullScreen();
        simulation->setFixedSize(1700, 1000);
    }
    simulation->setCouleurArrierePlan(QColor(preferences[1].split(' ')[2].toInt(), preferences[2].split(' ')[2].toInt(), preferences[3].split(' ')[2].toInt()));

    messageStatusBar = new QLabel("");
    statusBar()->addPermanentWidget(messageStatusBar);
    messageStatusBar->show();

    ui->Logo->setPixmap(QPixmap("data/assets/logo.png"));

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

MainWindow::~MainWindow()
{
    delete general;
    delete simulation;
    qDeleteAll(*particules);
    delete particules;
    delete particulesConteneur;
    delete messageStatusBar;
    delete ui;
    exit(EXIT_SUCCESS); //Met [Critical error detected c0000374] sous le tapis
}

void MainWindow::refaireListe()
{
    int barreVisible(0);
    if(ui->elements->size().height() < particules->size() * 64)
    {
        barreVisible = 1;
    }
    while (particules->size() < general->nombreParticule()) {
        particules->append(new ParticuleListe(particules->size() * 64, particulesConteneur));
        particulesConteneur->setFixedSize(253 - 17 * barreVisible, particules->size() * 64);
        particules->at(particules->size() - 1)->show();
    }
    for(int p(0); p < particules->size(); p++)
    {
        if(ui->Formule->isChecked())
        {
            particules->at(p)->setFormule(general->getParticule(p)->getFormule());
        }
        else
        {
            particules->at(p)->setFormule(general->getParticule(p)->getNom());
        }
        particules->at(p)->setCouleur(general->getParticule(p)->getCouleurParticule());
    }
    ui->Remplissages->setDisabled(general->nombreParticule() == 0);
    tailleListe();
}

void MainWindow::tailleListe()
{
    int barreVisible(0);
    if(ui->elements->size().height() < particules->size() * 64)
    {
        barreVisible = 1;
    }
    for(int p(0); p < particules->size(); p++)
    {
        particules->at(p)->setFixedSize(253 - 17 * barreVisible, 64);
    }
}

void MainWindow::on_actionImporterParticule_triggered()
{
    simulation->setPaused(true);
    dialogImporterParticule = new DialogImporterParticule(general, this);
    dialogImporterParticule->setModal(true);
    dialogImporterParticule->show();
    connect(dialogImporterParticule, SIGNAL(dialogQuit(int)), this, SLOT(dialogQuit(int)));
}

void MainWindow::on_actionChargerSimulation_triggered()
{
    simulation->setPaused(true);
    dialogChargerSimulation = new DialogChargerSimulation(general, this);
    dialogChargerSimulation->setModal(true);
    dialogChargerSimulation->show();
    connect(dialogChargerSimulation, SIGNAL(dialogQuit(int, General*)), this, SLOT(dialogQuit(int, General*)));
}

void MainWindow::on_actionSauvegarderParticule_triggered()
{
    if(general->nombreParticule() == 0)
    {
        return;
    }
    simulation->setPaused(true);
    dialogSauvegardeParticule = new DialogSauvegardeParticule(general, this);
    dialogSauvegardeParticule->setModal(true);
    dialogSauvegardeParticule->show();
    connect(dialogSauvegardeParticule, SIGNAL(dialogQuit(int)), this, SLOT(dialogQuit(int)));
}

void MainWindow::on_actionSauvegarderSimulation_triggered()
{
    simulation->setPaused(true);
    dialogSauvegardeSimulation = new DialogSauvegardeSimulation(general, this);
    dialogSauvegardeSimulation->setModal(true);
    dialogSauvegardeSimulation->show();
    connect(dialogSauvegardeSimulation, SIGNAL(dialogQuit(int)), this, SLOT(dialogQuit(int)));
}

void MainWindow::on_actionEditer_triggered()
{
    simulation->setPaused(true);
    General* copieGeneral(general->creerCopie());
    dialogSimulation = new DialogSimulation(copieGeneral, this);
    connect(dialogSimulation, SIGNAL(sauvegarder(General)), this, SLOT(donneeSimulationRecu(General)));
    connect(dialogSimulation, SIGNAL(annuler()), this, SLOT(donneeSimulationAnnuler()));
    dialogSimulation->setModal(true);
    dialogSimulation->show();
}

void MainWindow::on_actionFermer_triggered()
{
    simulation->setPaused(true);
    if(QMessageBox::question(this, "Fermer", "Êtes vous sur ? Toutes données non sauvegardées seront perdues") == QMessageBox::Yes)
    {
        simulation->nettoyer();
        general->setTout(General());
        qDeleteAll(*particules);
        particules->clear();
        simulation->refaireListeParticule();
        refaireListe();
    }
    simulation->setPaused(false);
}

void MainWindow::on_actionChronometre_triggered(bool checked)
{
    simulation->utiliserChronometre(checked);
}

void MainWindow::on_actionThermometre_triggered(bool checked)
{
    simulation->utiliserThermometre(checked);
}

void MainWindow::on_actionPHMetre_triggered(bool checked)
{
    simulation->utiliserPHMetre(checked);
}

void MainWindow::on_actionConductimetre_triggered(bool checked)
{
    simulation->utiliserConductimetre(checked);
}

void MainWindow::on_actionColorimetre_triggered(bool checked)
{
    simulation->utiliserColorimetre(checked);
}

void MainWindow::on_actionOrdinateur_triggered(bool checked)
{
    simulation->utiliserOrdinateur(checked);
}

void MainWindow::on_actionSombre_triggered(bool checked)
{
    simulation->setPaused(true);
    if(checked)
    {
        preferences.replace(0, "Thème Sombre : Oui");
    }
    else
    {
        preferences.replace(0, "Thème Sombre : Non");
    }
    preferences.save();
    QMessageBox::information(this, "Thème", "Le thème sera effectif au prochain redémarrage");
    simulation->setPaused(false);
}

void MainWindow::on_actionFond_triggered()
{
    simulation->setPaused(true);
    QColor couleur(QColorDialog::getColor(QColor(140, 140, 140), this, "Couleur d'arrière-plan"));
    if(!couleur.isValid())
    {
        couleur = simulation->getCouleurArrierePlan();
    }
    simulation->setCouleurArrierePlan(couleur);
    preferences.replace(1, "Fond.R : " + QString::number(couleur.red()));
    preferences.replace(2, "Fond.G : " + QString::number(couleur.green()));
    preferences.replace(3, "Fond.B : " + QString::number(couleur.blue()));
    preferences.save();
    simulation->setPaused(false);
}

void MainWindow::on_actionCompteurFPS_triggered(bool checked)
{
    if(checked)
    {
        preferences.replace(4, "Afficher FPS : Oui");
    }
    else
    {
        preferences.replace(4, "Afficher FPS : Non");
    }
    preferences.save();
    simulation->setFpsAEnvoyer(checked);
}

void MainWindow::on_actionPleinEcran_triggered(bool checked)
{
    if(checked)
    {
        preferences.replace(5, "Plein écran : Oui");
        showFullScreen();
        simulation->setFixedSize(1700, 1000);
    }
    else
    {
        preferences.replace(5, "Plein écran : Non");
        showNormal();
    }
    preferences.save();
}

void MainWindow::on_actionAide_triggered()
{
    simulation->setPaused(true);
    dialogAide = new DialogAide(this);
    connect(dialogAide, SIGNAL(dialogQuit(int)), this, SLOT(dialogQuit(int)));
    dialogAide->setModal(true);
    dialogAide->show();
}

void MainWindow::on_actionAPropos_triggered()
{
    simulation->setPaused(true);
    dialogAPropos = new DialogAPropos(this);
    connect(dialogAPropos, SIGNAL(dialogQuit(int)), this, SLOT(dialogQuit(int)));
    dialogAPropos->setModal(true);
    dialogAPropos->show();
}

void MainWindow::on_actionDonnations_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/donate/?hosted_button_id=XXHVRJNEKC4KC"));
}

void MainWindow::dialogQuit(int type, General* nouveau)
{
    switch (type) {
    case 1:
        simulation->refaireListeParticule();
        refaireListe();
        break;
    case 3:
        if(nouveau != nullptr)
        {
            simulation->nettoyer();
            general->setTout(*nouveau);
            qDeleteAll(*particules);
            particules->clear();
            simulation->refaireListeParticule();
            refaireListe();
            delete nouveau;
        }
        break;
    }
    simulation->setPaused(false);
}

void MainWindow::donneeSimulationRecu(General simulationGet)
{
    general->setTout(simulationGet);
    simulation->refaireListeParticule();
    refaireListe();
    simulation->setPaused(false);
}

void MainWindow::donneeSimulationAnnuler()
{
    simulation->setPaused(false);
}

void MainWindow::on_Pause_toggled(bool toggled)
{
    simulation->setPaused(toggled, true);
}

void MainWindow::on_Burette_clicked()
{
    simulation->setPaused(true);
    dialogRemplissage = new DialogRemplissage(false, simulation->getTypeBurette(), general, this);
    connect(dialogRemplissage, SIGNAL(remplir(Remplissage)), simulation, SLOT(donneeRemplissageRecu(Remplissage)));
    connect(dialogRemplissage, SIGNAL(annuler()), simulation, SLOT(donneeAnnuler()));
    dialogRemplissage->setModal(true);
    dialogRemplissage->show();
}

void MainWindow::on_Becher_clicked()
{
    simulation->setPaused(true);
    dialogRemplissage = new DialogRemplissage(true, simulation->getTypeBecher(), general, this);
    connect(dialogRemplissage, SIGNAL(remplir(Remplissage)), simulation, SLOT(donneeRemplissageRecu(Remplissage)));
    connect(dialogRemplissage, SIGNAL(annuler()), simulation, SLOT(donneeAnnuler()));
    dialogRemplissage->setModal(true);
    dialogRemplissage->show();
}

void MainWindow::on_Eau_clicked()
{
    simulation->setPaused(true);
    bool ok;
    double volume(QInputDialog::getDouble(this, "Nouveau volume", "Nouveau volume de solution, seul quantité est conservée (en mL)", 50, 1, simulation->getTypeBecher() * 100, 2, &ok));
    if(ok)
    {
        simulation->setVolumeBecher(volume);
    }
    simulation->setPaused(false);
}

void MainWindow::on_Temperature_clicked()
{
    simulation->setPaused(true);
    bool ok;
    double temperature(QInputDialog::getDouble(this, "Nouvelle température", "Température de la solution dans le becher (en °C)", 25, -273.15, 10000, 2, &ok));
    if(ok)
    {
        simulation->setTemperatureBecher(temperature);
    }
    simulation->setPaused(false);
}

void MainWindow::on_Concentration_toggled(bool toggled)
{
    simulation->setConcentration(toggled);
}

void MainWindow::on_Formule_toggled(bool toggled)
{
    for(int p(0); p < particules->size(); p++)
    {
        if(toggled)
        {
            particules->at(p)->setFormule(general->getParticule(p)->getFormule());
        }
        else
        {
            particules->at(p)->setFormule(general->getParticule(p)->getNom());
        }
    }
}

void MainWindow::lagging(QString lag)
{
    messageStatusBar->setText(lag);
}

void MainWindow::mettreEnPause()
{
    ui->Pause->setChecked(!ui->Pause->isChecked());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(event->size().width() < 1550)
    {
        ui->simulation->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        ui->simulation->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    if(event->size().height() < 1017)
    {
        ui->simulation->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        simulation->setFixedSize(1700, 1000);
    }
    else
    {
        ui->simulation->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        simulation->setFixedSize(1700, 936);
    }
}
