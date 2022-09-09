#include "simulation.h"

Simulation::Simulation(QList<ParticuleListe*> *particulesListeT, General *generalT, QWidget *parent) : QWidget(parent)
{
    general = generalT;
    particulesListe = particulesListeT;
    concentration = false;

    paused = false;
    pauseBouton = false;
    pauseDialog = false;

    fpsCible = 60;
    setFocusPolicy(Qt::ClickFocus);

    maxX = 20;
    maxY = 19;
    maxXmini = 15;
    maxYmini = 15;
    concentrationBecherParticuleInitialise = false;

    //Constructions des instruments

    chronometre = new Chronometre(general, this);
    chronometre->move(250, 10);
    chronometre->hide();
    connect(this, SIGNAL(chronoStart()), chronometre, SLOT(startClicked()));
    connect(this, SIGNAL(chronoReset()), chronometre, SLOT(resetClicked()));
    connect(this, SIGNAL(chronoMode()), chronometre, SLOT(modeClicked()));

    conductimetre = new Conductimetre(general, this);
    conductimetre->move(605, 578);
    conductimetre->hide();

    thermometre = new Thermometre(general, this);
    thermometre->move(570, 480);
    thermometre->hide();

    pHMetre = new PHMetre(general, this);
    pHMetre->move(120, 620);
    pHMetre->hide(); 

    colorimetre = new Colorimetre(general, this);
    colorimetre->move(200, 400);
    colorimetre->hide();

    ecran = new QLabel(this);
    QPixmap ecranImg("data/assets/ordinateur.png");
    ecran->setPixmap(ecranImg.scaled(ecranImg.width() / 3, ecranImg.height() / 3));
    ordinateur = new Ordinateur(ecran);
    ecran->move(570, 10);
    ecran->hide();

    connect(ordinateur, SIGNAL(assimiliationClic(bool)), chronometre, SLOT(bloquerBoutons(bool)));
    connect(ordinateur, SIGNAL(assimiliationClic(bool)), colorimetre, SLOT(bloquerBoutons(bool)));

    //Construction du becher

    becher = new QLabel(this);
    QPixmap becherImg("data/assets/becher100.png");
    becherImg = becherImg.scaled(becherImg.width() / 3, becherImg.height() / 3);
    becher->setPixmap(becherImg);
    becher->move(450, 700);
    becher->setToolTip("0 mL");

    //Construction de la burette

    burette = new QLabel(this);
    QPixmap buretteImg("data/assets/burette25.png");
    buretteImg = buretteImg.scaled(buretteImg.width() / 3, buretteImg.height() / 3);
    burette->setPixmap(buretteImg);
    burette->move(550, 0);
    burette->setToolTip("0 mL");

    //Slider pour le débit de la burette

    debitBurette = new QSlider(Qt::Vertical, this);
    debitBurette->setFixedSize(debitBurette->width() / 8, debitBurette->height() * 1.5);
    debitBurette->move(580, 695);
    debitBurette->setRange(0, 100);
    debitBurette->setValue(100); //Le débit est inversé : 100 -> min ; 0 -> max

    //Initialisation des variables

    frames = 0;
    compteFps = 0;
    fpsActuel = 0;
    rateFps = 0;
    messageFps = false;
    volumeBecher = 0;
    volumeBurette = 0;
    typeBecher = 1;
    typeBurette = 1;
    couleurBecher = QColor(192, 192, 200, 128);
    couleurBurette = QColor(192, 192, 200, 128);
    couleurArrierePlan = QColor(140, 140, 140);
    temperatureBecher = 25;
    temperatureBurette = 25;
    chronometrePresent = false;
    thermometrePresent = false;
    pHMetrePresent = false;
    conductimetrePresent = false;
    colorimetrePresent = false;
    fpsAEnvoyer = false;
    dernierElapsed = 1;

    totalTempsPause = 0;
    timerGlobal.start();
    timerFrame.start();
    timerSecondes.start();
    QTimer::singleShot(1, this, SLOT(loop()));
}

Simulation::~Simulation()
{
    delete becher;
    delete burette;
    delete debitBurette;
    delete chronometre;
    delete thermometre;
    delete pHMetre;
    delete conductimetre;
}

void Simulation::calculPH(double H3O, double HO)
{
    if(general->getPH())
    {
        double difference(HO - H3O);
        if(difference == 0)
        {
            pH = 7;
        }
        else if(difference > 0)
        {
            pH = -log10(difference);
        }
        else
        {
            pH = 14 + log10(-difference);
        }
    }
    else
    {
        pH = 7;
    }
}

/*double Simulation::calculCoeffAleatoire(double p, double t)
{
    //a(t) = p + t * (p * exp(-p)* (exp(-pt) * (exp(-pt) + 1) / 2)) + (-0.008 * (p - 0.5)² + 0.002) * t²
    double expPT(qExp(-p * t));
    return p + t * (p * qExp(-p) * (expPT * (expPT + 1) / 2)) + (-0.008 * (p - 0.5) * (p - 0.5) + 0.002) * t * t;
    *
     * Fonction approximativement égale à la suite v(n) pour :
     * v(n + 1) = v(n) + u(n + 1)
     * v(0) = p
     * u(n + 1) = u(n) - u(n)²
     * u(0) = p
     * p compris entre 0 et 1
     *
} La fonction v(n) était elle-même une approximation, changée pour p(t) = 1 - (1 / exp(60 * ln(-1 / (p - 1))))^((x + 1) / 60)*/

void Simulation::refaireListeParticule()
{
    for (int n(0); n < particules.size(); n++) {
        particules[n]->setCaracteristiques(general->getParticule(particules[n]->getIDCaracteristiques()));
    }
}

void Simulation::nettoyer()
{
    qDeleteAll(particules);
    particules.clear();
    volumeBecher = 0;
    volumeBurette = 0;
    temperatureBecher = 0;
    temperatureBurette = 0;
    reactionsPossiblesPrecedent.clear();
    degagementGazeux.clear();
    concentrationBecherParticuleInitialise = false;
}

void Simulation::utiliserChronometre(bool checked)
{
    chronometrePresent = checked;
    if(checked)
    {
        chronometre->show();
    }
    else
    {
        chronometre->hide();
    }
}

void Simulation::utiliserThermometre(bool checked)
{
    thermometrePresent = checked;
    if(checked)
    {
        thermometre->show();
    }
    else
    {
        thermometre->hide();
    }
}

void Simulation::utiliserPHMetre(bool checked)
{
    pHMetrePresent = checked;
    if(checked)
    {
        pHMetre->show();
    }
    else
    {
        pHMetre->hide();
    }
}

void Simulation::utiliserConductimetre(bool checked)
{
    conductimetrePresent = checked;
    if(checked)
    {
        conductimetre->show();
    }
    else
    {
        conductimetre->hide();
    }
}

void Simulation::utiliserColorimetre(bool checked)
{
    colorimetrePresent = checked;
    if(checked)
    {
        colorimetre->show();
    }
    else
    {
        colorimetre->hide();
    }
}

void Simulation::utiliserOrdinateur(bool checked)
{
    ordinateurPresent = checked;
    if(checked)
    {
        ecran->show();
    }
    else
    {
        ecran->hide();
    }
}

void Simulation::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter dessin;
    dessin.begin(this);

    //Arrière-plan
    dessin.fillRect(0, 0, 2000, 1000, couleurArrierePlan);

    //Contenu du becher
    dessin.fillRect(470, (165 - ((volumeBecher / typeBecher) * 1.65)) + 766, 175, (volumeBecher / typeBecher) * 1.65, couleurBecher);

    //Contenu dynamique de la burette
    dessin.fillRect(555, (665 - ((volumeBurette / typeBurette) * 26.6)) + 30, 10, (volumeBurette / typeBurette) * 26.6, couleurBurette);
    //Contenu statique de la burette
    dessin.fillRect(555, 694, 10, 44, couleurBurette);
    QPainterPath basBurette;
    basBurette.moveTo(555, 738);
    basBurette.lineTo(558, 762);
    basBurette.lineTo(563, 762);
    basBurette.lineTo(565, 738);
    basBurette.lineTo(555, 738);
    dessin.fillPath(basBurette, couleurBurette);

    //Particules
    for (int n(0); n < particules.size(); n++) {
        if(particulesListe->at(particules[n]->getCaracteristiques()->getID())->getVisible())
        {
            dessin.setPen(particules[n]->getCaracteristiques()->getCouleurParticule());
            dessin.drawEllipse(particules[n]->x() - 2, particules[n]->y() - 2, 5, 5);
            dessin.fillRect(particules[n]->x() - 1, particules[n]->y() - 1, 4, 4, particules[n]->getCaracteristiques()->getCouleurParticule());
        }
    };
}

void Simulation::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_S:
        if(chronometrePresent)
            emit chronoStart();
        break;
    case Qt::Key_R:
        if(chronometrePresent)
            emit chronoReset();
        break;
    case Qt::Key_M:
        if(chronometrePresent)
            emit chronoMode();
        break;
    case Qt::Key_A:
        if(ordinateurPresent)
            ordinateur->on_Assimiler_clicked();
        break;
    case Qt::Key_Space:
        emit pause();
        break;
    case Qt::Key_Escape:
        int confirmaton(QMessageBox::question(this, "Quitter", "Quitter SimuRéac ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No));
        if(confirmaton == QMessageBox::Yes)
        {
            QApplication::quit();
        }
        break;
    }
}

bool Simulation::getFpsAEnvoyer() const
{
    return fpsAEnvoyer;
}

void Simulation::setFpsAEnvoyer(bool value)
{
    fpsAEnvoyer = value;
}

QColor Simulation::getCouleurArrierePlan() const
{
    return couleurArrierePlan;
}

void Simulation::setCouleurArrierePlan(const QColor &value)
{
    couleurArrierePlan = value;
}

void Simulation::setConcentration(bool value)
{
    concentration = value;
}

void Simulation::refaireReactifsAttires()
{
    reactifsAttires.clear();
    coefficientAttiranceReactif.clear();
    coefficientAttiranceReactifCinetique.clear();
    for(int p(0); p < general->nombreParticule(); p++)
    {
        reactifsAttires.append(QList<int>());
        coefficientAttiranceReactif.append(1);
        coefficientAttiranceReactifCinetique.append(1);
        int ajoutCoeff(1);
        for(int r(0); r < general->nombreReaction(); r++)
        {
            if(reactionsPossibles[r] > 0)
            {
                bool aAjouter(false);
                double coeffCinetique(0);
                QList<int> ajout;
                for(int t(0); t < general->getReaction(r)->getReactifs().size(); t++)
                {
                    if((!aAjouter) && general->getReaction(r)->getReactifs()[t]->getID() == general->getParticule(p)->getID())
                    {
                        aAjouter = true;
                        coeffCinetique += general->getReaction(r)->getCinetiqueKPourT(temperatureBecher, -1);
                    }
                    else
                    {
                        ajout.append(general->getReaction(r)->getReactifs()[t]->getID());
                        coefficientAttiranceReactif.last() += ajoutCoeff;
                        ajoutCoeff++;
                    }
                }
                coeffCinetique = (1 - pow(1 / qExp(coeffCinetique), 1.0/60.0)) * 1.44;
                coeffCinetique *= general->getReaction(r)->getPH(pH);
                coefficientAttiranceReactif.last() *= coeffCinetique;
                coefficientAttiranceReactifCinetique.last() *= coeffCinetique;
                if(aAjouter)
                {
                    for(int a(0); a < ajout.size(); a++)
                    {
                        if(reactifsAttires[p].indexOf(ajout[a]) == -1)
                        {
                            reactifsAttires[p].append(ajout[a]);
                        }
                    }
                }
            }
        }
    }
    for(int p(0); p < reactionsPossiblesPrecedent.size(); p++)
    {
        int deltaReactionPossible(reactionsPossibles[p] - reactionsPossiblesPrecedent[p]);
        if(deltaReactionPossible > 0)
        {
            for(int r(0); r < general->getReaction(p)->getReactifs().size(); r++)
            {
                int attiranceNecessaire(reactionsPossibles[p] * general->getReaction(p)->getProportionReactifs()[r]);
                for(int t(0); t < particules.size() && attiranceNecessaire > 0; t++)
                {
                    if(particules[t]->getYDouble() >= 763 && particules[t]->getCaracteristiques()->getID() == general->getReaction(p)->getReactifs()[r]->getID())
                    {
                        particules[t]->setAttirance(true);
                        attiranceNecessaire--;
                    }
                }
            }
        }
    }
}

bool Simulation::getPaused() const
{
    return paused;
}

void Simulation::setPaused(bool value, bool bouton)
{
    if(bouton)
    {
        pauseBouton = value;
    }
    else
    {
        pauseDialog = value;
    }
    bool anciennePause(paused);
    paused = pauseBouton || pauseDialog;
    if(paused != anciennePause)
    {
        if(paused)
        {
            timerPause.start();
        }
        else
        {
            totalTempsPause += timerPause.elapsed();
        }
    }
}

double Simulation::mouvementConcentration(int n)
{
    if(abs(n) < 1)
    {
        return 0;
    }
    else if(abs(n) > 11)
    {
        return 0.4 * (abs(n) / n);
    }
    else
    {
        return (0.2 + (abs(n) - 1) / 50) * (abs(n) / n);
    }
}

double Simulation::vibrationConcentration(unsigned int n)
{
    if(n == 0) //n est un unsigned int, donc si n = 0, alors 1 + (n - 1) * 0.1 = 1 + 2^32 * 0.1
    {
        return 1;
    }
    else if(n > 21)
    {
        return 3;
    }
    else
    {
        return 1 + (n - 1) * 0.1;
    }
}

double Simulation::getTemperatureBecher() const
{
    return temperatureBecher;
}

void Simulation::setTemperatureBecher(double value)
{
    temperatureBecher = value;
}

int Simulation::getTypeBurette() const
{
    return typeBurette;
}

void Simulation::setTypeBurette(int value)
{
    if(!(value == 1 || value == 2))
    {
        return;
    }
    typeBurette = value;
    QPixmap buretteImg("data/assets/burette" + QString::number(value * 25) + ".png");
    buretteImg = buretteImg.scaled(buretteImg.width() / 3, buretteImg.height() / 3);
    burette->setPixmap(buretteImg);
}

int Simulation::getTypeBecher() const
{
    return typeBecher;
}

void Simulation::setTypeBecher(int value)
{
    if(!(value == 1 || value == 2 || value == 4))
    {
        return;
    }
    typeBecher = value;
    QPixmap becherImg("data/assets/becher" + QString::number(value * 100) + ".png");
    becherImg = becherImg.scaled(becherImg.width() / 3, becherImg.height() / 3);
    becher->setPixmap(becherImg);
}

double Simulation::getVolumeBurette() const
{
    return volumeBurette;
}

void Simulation::setVolumeBurette(double value)
{
    volumeBurette = value;
}

void Simulation::loop()
{
    if(timerSecondes.elapsed() >= 1000)
    {
        timerSecondes.start();
        fpsActuel = compteFps;
        compteFps = 0;
        QString envoi("");
        if(fpsAEnvoyer)
        {
            envoi += QString::number(fpsActuel) + " TPS";
        }
        if(!paused)
        {
            if(fpsActuel < fpsCible / 2)
            {
                rateFps++;
                if(rateFps > 3 && !messageFps)
                {
                    messageFps = true;
                }
                else if(rateFps <= 3 && messageFps)
                {
                    messageFps = false;
                }
            }
            else
            {
                rateFps = 0;
            }
        }
        if(messageFps)
        {
            if(fpsAEnvoyer)
            {
                envoi += " - Chute importante de la fréquence de la simulation detectée !";
            }
            else
            {
                envoi += "Chute importante de la fréquence de la simulation detectée !";
            }
        }
        emit lagging(envoi);
    }
    if(paused)
    {
        timerFrame.start();
        QTimer::singleShot(1, this, SLOT(loop()));
        return;
    }
    if(timerFrame.nsecsElapsed() < (1000.0 / fpsCible) * 1000000)
    {
        QTimer::singleShot(1, this, SLOT(loop()));
        return;
    }
    double elapsed((timerFrame.restart() * 60.0) / 1000.0); //elapsed systématquement sur du 60 FPS
    //Attention ! elapsed est un coeff ! elapsed = 1 pour FPS = 60 ; elapsed = 2 pour FPS = 30
    if(elapsed > dernierElapsed * 3 && frames >= 60) //Anti Lag-Spike
    {
        dernierElapsed = elapsed;
        chronometre->oublierTemps((elapsed * 1000.0) / 60.0);
        QTimer::singleShot(1, this, SLOT(loop()));
        return;
    }
    dernierElapsed = elapsed;
    frames++;
    compteFps++;

    const int limiteBuretteAncien((665 - ((volumeBurette / typeBurette) * 26.6)) + 30); //Ordonnée où s'arrêtait le volume de la burette avant le calcul via le débit
    //Débit burette
    if(debitBurette->value() != 100)
    {
        if(volumeBecher >= 100)
        {
            volumeBecher = 100;
            debitBurette->setValue(100);
        }
        else if(volumeBurette <= 0)
        {
            volumeBurette = 0;
            debitBurette->setValue(100);
        }
        else
        {
            double deplacement (((((100 - debitBurette->value()) / 100.0) / 60.0) * 2) * elapsed);
            temperatureBecher = (temperatureBecher * volumeBecher + temperatureBurette * deplacement) / (volumeBecher + deplacement);
            volumeBecher += deplacement;
            volumeBurette -= deplacement;
        }
    }

    becher->setToolTip(QString::number(qRound(volumeBecher * 10) / 10.0) + " mL");
    burette->setToolTip(QString::number(qRound(volumeBurette * 10) / 10.0) + " mL");

    //Nombre de particules dans chaque cases d'un quadrillage de maxX*maxY avec dimension des cases de 9*9 dans le becher
    for (int x(0); x < maxX; x++) {
        for (int y(0); y < maxY; y++) {
            concentrationBecher[x][y].clear();
            for (int n(0); n < general->nombreParticule(); n++) {
                concentrationBecher[x][y].append(0);
            }
        }
    }
    for (int n(0); n < particules.size(); n++) {
        if(particules[n]->getXDouble() >= 471 && particules[n]->getXDouble() <= 642 && particules[n]->getYDouble() >= 769 && particules[n]->getYDouble() <= 931)
        {
            concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()]++;
        }
    }

    QList<int> nombreParticulesParType; //Utile pour "Réactions possibles avec le contenu du becher"
    //Position dans le quadrillage de la concentration maximale d'une particule
    concentrationMaxBecher.clear();
    for (int n(0); n < general->nombreParticule(); n++) {
        nombreParticulesParType.append(0);
        QList<QPoint> maxPoints;
        unsigned int maxValue(0);
        for (int x(0); x < maxX; x++) {
            for (int y(0); y < maxY; y++) {
                if(concentrationBecher[x][y][n] > maxValue)
                {
                    maxValue = concentrationBecher[x][y][n];
                    maxPoints.clear();
                    maxPoints.append(QPoint(x, y));
                }
                else if(concentrationBecher[x][y][n] == maxValue)
                {
                    maxPoints.append(QPoint(x, y));
                }
                nombreParticulesParType.last() += concentrationBecher[x][y][n];
            }
        }
        if(maxValue == 0)
        {
            concentrationMaxBecher.append(QPoint(-1, -1));
        }
        else
        {
            concentrationMaxBecher.append(maxPoints[general->getAleatoire()->generateDouble() * maxPoints.size()]);
        }
    }

    //Réactions possibles avec le contenu du becher
    reactionsPossibles.clear();
    for (int r(0); r < general->nombreReaction(); r++) {
        int possibleMax(1000000000);
        for (int p(0); p < general->getReaction(r)->getProportionReactifs().size() && possibleMax; p++) {
            int avancementMax(nombreParticulesParType[general->getReaction(r)->getReactifs()[p]->getID()] / general->getReaction(r)->getProportionReactifs()[p]);
            if(avancementMax < possibleMax)
            {
                possibleMax = avancementMax;
            }
        }
        reactionsPossibles.append(possibleMax);
    }
    if(reactionsPossiblesPrecedent != reactionsPossibles || general->getChangementRecent())
    {
        refaireReactifsAttires();
        reactionsPossiblesPrecedent = reactionsPossibles;
        general->setChangementRecent(false);
    }

    if(concentrationBecherParticuleInitialise)
    {
        //Zones du becher où les réactifs doivent ralentir pour une prochaine réaction
        for (int x(0); x < maxXmini; x++) {
            for (int y(0); y < maxYmini; y++) {
                stopReactif[x][y].clear();
                for (int p(0); p < general->nombreParticule(); p++) {
                    bool attirance(false);
                    for (int r(0); r < reactifsAttires[p].size(); r++) {
                        if(p != reactifsAttires[p][r])
                        {
                            if(concentrationBecherParticule[x][y][reactifsAttires[p][r]].size() > 0)
                            {
                                attirance = true;
                            }
                        }
                    }
                    stopReactif[x][y].append(attirance);
                }
            }
        }
    }

    //Nombre de particules dans chaque cases d'une succession verticale de 67 cases avec dimension des cases de hauteur 9 dans la burette
    for (int y(0); y < 73; y++) {
        concentrationBurette[y].clear();
        for (int n(0); n < general->nombreParticule(); n++) {
            concentrationBurette[y].append(0);
        }
    }
    for (int n(0); n < particules.size(); n++) {
        if(particules[n]->getYDouble() >= 101 && particules[n]->getYDouble() <= 763)
        {
            concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()]++;
        }
    }

    //Nombre de particules solides à chaque coordonnées du becher
    for (int x(0); x < 172; x++) {
        for (int y(0); y < 163; y++) {
            presenceSolide[x][y].clear();
            for (int n(0); n < general->nombreParticule(); n++) {
                presenceSolide[x][y].append(0);
            }
        }
    }
    for (int n(0); n < particules.size(); n++) {
        if(particules[n]->getCaracteristiques()->getEtat() == 1 && particules[n]->getXDouble() >= 471 && particules[n]->getXDouble() <= 642 && particules[n]->getYDouble() >= 769 && particules[n]->getYDouble() <= 931)
        {
            presenceSolide[qRound(particules[n]->getXDouble()) - 471][qRound(particules[n]->getYDouble()) - 769][particules[n]->getCaracteristiques()->getID()]++;
        }
    }

    //Limite supérieur du becher et de la burette
    const int limiteBecher((165 - ((volumeBecher / typeBecher) * 1.65)) + 766); //Ordonnée où s'arrête le volume du becher
    const int limiteBurette((665 - ((volumeBurette / typeBurette) * 26.6)) + 30); //Ordonnée où s'arrête le volume de la burette
    const int maxSolide(nombreDansBecher / 14018.0 + 1); //(172 + 163) / 2 = 14018

    //Déplacement particules
    double coeffTemperature;
    if(temperatureBecher <= 0)
    {
        coeffTemperature = 0;
    }
    else
    {
        coeffTemperature = log10((temperatureBecher + 2.5) / 2.75);
    }
    const double aleatoireMax(4);
    for (int n(0); n < particules.size(); n++) {
        if(particules[n]->getCaracteristiques()->getEtat() == 3) //Particules gazeuses (Toujours dans le becher)
        {
            particules[n]->setXDouble(particules[n]->getXDouble() + (general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * coeffTemperature * elapsed);
            if(particules[n]->getXDouble() < 471) //Limite gauche
            {
                particules[n]->setXDouble(471);
            }
            if(particules[n]->getXDouble() > 642) //Limite droite
            {
                particules[n]->setXDouble(642);
            }
            particules[n]->setYDouble(particules[n]->getYDouble() + (general->getAleatoire()->generateDouble() * (aleatoireMax * 2) * -1) * coeffTemperature * elapsed);
            if(particules[n]->getYDouble() > 931) //Limite basse
            {
                particules[n]->setYDouble(931);
            }
            if(particules[n]->getYDouble() < limiteBecher + 3) //Limite haute (max = 766 + 3 = 769)
            {
                particules[n]->setYDouble(limiteBecher + 3);
            }
        }
        else if(particules[n]->getCaracteristiques()->getEtat() == 1) //Particules solides (Toujours dans le becher)
        {
            const int quadrillageX(qRound(particules[n]->getXDouble()) - 471), quadrillageY(qRound(particules[n]->getYDouble()) - 769);
            double deplacementX(((general->getAleatoire()->generateDouble() * ((aleatoireMax / particules[n]->getVibrationSolide()) * 2) - (aleatoireMax / particules[n]->getVibrationSolide())) * vibrationConcentration(concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()])));
            deplacementX *= -0.111 * particules[n]->getVibrationSolide() + 2.111; //f(1) = 2 ; f(10) = 1
            double deplacementY(0);
            if(quadrillageY < 162)
            {
                if(presenceSolide[quadrillageX][quadrillageY + 1][particules[n]->getCaracteristiques()->getID()] < maxSolide)
                {
                    deplacementY += (aleatoireMax / 10) * 0.6;
                    if(quadrillageY > 0)
                    {
                        if(presenceSolide[quadrillageX][quadrillageY - 1][particules[n]->getCaracteristiques()->getID()] < maxSolide)
                        {
                            deplacementY += ((general->getAleatoire()->generateDouble() * ((aleatoireMax / particules[n]->getVibrationSolide()) * 2) - (aleatoireMax / particules[n]->getVibrationSolide())) * vibrationConcentration(concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()]));
                        }
                    }
                }
            }
            if(quadrillageX < 171 && deplacementX > 0)
            {
                if(!(presenceSolide[quadrillageX + 1][quadrillageY][particules[n]->getCaracteristiques()->getID()] < maxSolide))
                {
                    deplacementX = 0;
                }
            }
            if(quadrillageX > 0 && deplacementX < 0)
            {
                if(!(presenceSolide[quadrillageX - 1][quadrillageY][particules[n]->getCaracteristiques()->getID()] < maxSolide))
                {
                    deplacementX = 0;
                }
            }
            particules[n]->setXDouble(particules[n]->getXDouble() + deplacementX * coeffTemperature * elapsed);
            if(particules[n]->getXDouble() < 471) //Limite gauche
            {
                particules[n]->setXDouble(471);
            }
            if(particules[n]->getXDouble() > 642) //Limite droite
            {
                particules[n]->setXDouble(642);
            }
            particules[n]->setYDouble(particules[n]->getYDouble() + deplacementY * coeffTemperature * elapsed);
            if(particules[n]->getYDouble() > 931) //Limite basse
            {
                particules[n]->setYDouble(931);
            }
            if(particules[n]->getYDouble() < limiteBecher + 3) //Limite haute (max = 766 + 3 = 769)
            {
                particules[n]->setYDouble(limiteBecher + 3);
            }
            if(particules[n]->getVibrationSolide() < 10)
            {
                particules[n]->setVibrationSolide(particules[n]->getVibrationSolide() + 0.01 * elapsed);
            }
        }
        else if(particules[n]->y() < 695) //Burette au dessus de 0
        {
            if(particules[n]->getYDouble() < 101)
            {
                particules[n]->setYDouble(101);
            }
            particules[n]->setXDouble(particules[n]->getXDouble() + elapsed * ((general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * vibrationConcentration(concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()])));
            if(particules[n]->getXDouble() < 556) //Limite gauche
            {
                particules[n]->setXDouble(556);
            }
            if(particules[n]->getXDouble() > 563) //Limite droite
            {
                particules[n]->setXDouble(563);
            }
            /*particules[n]->setYDouble(particules[n]->getYDouble() + ((general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * vibrationConcentration(concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()])));
            if(particules[n]->getYDouble() > 762) //Limite basse
            {
                particules[n]->setYDouble(762);
            }
            if(particules[n]->getYDouble() < limiteBurette + 71) //Limite haute (max = 30 + 71 = 101)
            {
                particules[n]->setYDouble(limiteBurette + 71);
            }
            if(qRound((particules[n]->getYDouble() - 101) / 9) > 0)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() - mouvementConcentration((concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9) - 1][particules[n]->getCaracteristiques()->getID()])));
            }
            if(qRound((particules[n]->getYDouble() - 101) / 9) < 73)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() + mouvementConcentration((concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9) + 1][particules[n]->getCaracteristiques()->getID()])));
            }*/
            particules[n]->setYDouble(particules[n]->getYDouble() + (limiteBurette - limiteBuretteAncien));
        }
        else if(particules[n]->y() < 738) //Burette entre 0 et bout triangulaire
        {
            particules[n]->setXDouble(particules[n]->getXDouble() + elapsed * (general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax));
            if(particules[n]->getXDouble() < 556) //Limite gauche
            {
                particules[n]->setXDouble(556);
            }
            if(particules[n]->getXDouble() > 563) //Limite droite
            {
                particules[n]->setXDouble(563);
            }
            /*particules[n]->setYDouble(particules[n]->getYDouble() + ((general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * vibrationConcentration(concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()])));
            if(particules[n]->getYDouble() > 762) //Limite basse
            {
                particules[n]->setYDouble(762);
            }
            if(particules[n]->getYDouble() < limiteBurette + 71) //Limite haute (max = 30 + 71 = 101)
            {
                particules[n]->setYDouble(limiteBurette + 71);
            }
            if(qRound((particules[n]->getYDouble() - 101) / 9) > 0)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() - mouvementConcentration((concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9) - 1][particules[n]->getCaracteristiques()->getID()])));
            }
            if(qRound((particules[n]->getYDouble() - 101) / 9) < 73)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() + mouvementConcentration((concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9) + 1][particules[n]->getCaracteristiques()->getID()])));
            }*/
            particules[n]->setYDouble(particules[n]->getYDouble() + (limiteBurette - limiteBuretteAncien));
        }
        else if(particules[n]->y() <= 762) //Burette bout triangulaire
        {
            particules[n]->setXDouble(particules[n]->getXDouble() + elapsed * (general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax));
            if(particules[n]->getXDouble() < (0.125 * particules[n]->getYDouble() + 463.75) + 2) //Limite gauche
            {
                particules[n]->setXDouble((0.125 * particules[n]->getYDouble() + 463.75) + 2);
            }
            if(particules[n]->getXDouble() > ((-0.125) * particules[n]->getYDouble() + 655.25) - 1) //Limite droite
            {
                particules[n]->setXDouble(((-0.125) * particules[n]->getYDouble() + 655.25) - 1);
            }
            /*particules[n]->setYDouble(particules[n]->getYDouble() + ((general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * vibrationConcentration(concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()])));
            if(particules[n]->getYDouble() > 762) //Limite basse
            {
                particules[n]->setYDouble(762);
            }
            if(particules[n]->getYDouble() < limiteBurette + 71) //Limite haute (max = 30 + 71 = 101)
            {
                particules[n]->setYDouble(limiteBurette + 71);
            }
            if(qRound((particules[n]->getYDouble() - 101) / 9) > 0)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() - mouvementConcentration((concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9) - 1][particules[n]->getCaracteristiques()->getID()])));
            }
            if(qRound((particules[n]->getYDouble() - 101) / 9) < 73)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() + mouvementConcentration((concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBurette[qRound((particules[n]->getYDouble() - 101) / 9) + 1][particules[n]->getCaracteristiques()->getID()])));
            }*/
            particules[n]->setYDouble(particules[n]->getYDouble() + (limiteBurette - limiteBuretteAncien));
            if(!(particules[n]->y() < 762))
            {
                nombreDansBurette--;
            }
        }
        else if(particules[n]->y() < limiteBecher) //Entre burette et becher
        {
            particules[n]->setYDouble(particules[n]->getYDouble() + elapsed * ((particules[n]->getYDouble() - 761) / 2));
            if(!(particules[n]->y() < limiteBecher))
            {
                nombreDansBecher++;
            }
            if(particules[n]->y() > limiteBecher) //Limite basse
            {
                particules[n]->setYDouble(limiteBecher);
            }
        }
        else //Becher
        {
            QPoint vecteurAttirance(0, 0);
            if(particules[n]->getAttirance())
            {
                for (int d(0); d < reactifsAttires[particules[n]->getCaracteristiques()->getID()].size(); d++) {
                    QPoint pointVerif(concentrationMaxBecher[reactifsAttires[particules[n]->getCaracteristiques()->getID()][d]]);
                    if(pointVerif != QPoint(-1, -1))
                    {
                        if(((pointVerif.x() * 9) + 471) / 18 > particules[n]->x() / 18)
                        {
                            vecteurAttirance.setX(vecteurAttirance.x() + 1 * coefficientAttiranceReactif[particules[n]->getCaracteristiques()->getID()]);
                        }
                        else if(((pointVerif.x() * 9) + 471) / 18 < particules[n]->x() / 18)
                        {
                            vecteurAttirance.setX(vecteurAttirance.x() - 1 * coefficientAttiranceReactif[particules[n]->getCaracteristiques()->getID()]);
                        }
                        if(((pointVerif.y() * 9) + 769) / 18 > particules[n]->y() / 18)
                        {
                            vecteurAttirance.setY(vecteurAttirance.y() + 1 * coefficientAttiranceReactif[particules[n]->getCaracteristiques()->getID()]);
                        }
                        else if(((pointVerif.y() * 9) + 769) / 18 < particules[n]->y() / 18)
                        {
                            vecteurAttirance.setY(vecteurAttirance.y() - 1 * coefficientAttiranceReactif[particules[n]->getCaracteristiques()->getID()]);
                        }
                    }
                }
            }
            double stop(1);
            if(stopReactif[qRound((particules[n]->getXDouble() - 471) / 12)][qRound((particules[n]->getYDouble() - 769) / 12)][particules[n]->getCaracteristiques()->getID()])
            {
                stop = 1 - (0.75 * coefficientAttiranceReactifCinetique[particules[n]->getCaracteristiques()->getID()]);
            }
            particules[n]->setXDouble(particules[n]->getXDouble() + elapsed * stop * (((general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * vibrationConcentration(concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()])) + (vecteurAttirance.x() * 0.1)) * coeffTemperature);
            if(particules[n]->getXDouble() < 471) //Limite gauche
            {
                particules[n]->setXDouble(471);
            }
            if(particules[n]->getXDouble() > 642) //Limite droite
            {
                particules[n]->setXDouble(642);
            }
            particules[n]->setYDouble(particules[n]->getYDouble() + elapsed * stop * (((general->getAleatoire()->generateDouble() * (aleatoireMax * 2) - aleatoireMax) * vibrationConcentration(concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()])) + (vecteurAttirance.y() * 0.1)) * coeffTemperature);
            if(particules[n]->getYDouble() > 931) //Limite basse
            {
                particules[n]->setYDouble(931);
            }
            if(particules[n]->getYDouble() < limiteBecher + 3) //Limite haute (max = 766 + 3 = 769)
            {
                particules[n]->setYDouble(limiteBecher + 3);
            }
            if(qRound((particules[n]->getYDouble() - 769) / 9) > 0)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() - mouvementConcentration((concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9) - 1][particules[n]->getCaracteristiques()->getID()])) * coeffTemperature);
            }
            if(qRound((particules[n]->getYDouble() - 769) / 9) < maxY - 1)
            {
                particules[n]->setYDouble(particules[n]->getYDouble() + mouvementConcentration((concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9) + 1][particules[n]->getCaracteristiques()->getID()])) * coeffTemperature);
            }
            if(qRound((particules[n]->getXDouble() - 471) / 9) > 0)
            {
                particules[n]->setXDouble(particules[n]->getXDouble() - mouvementConcentration((concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9) - 1][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()])) * coeffTemperature);
            }
            if(qRound((particules[n]->getXDouble() - 471) / 9) < maxX - 1)
            {
                particules[n]->setXDouble(particules[n]->getXDouble() + mouvementConcentration((concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9)][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()] - concentrationBecher[qRound((particules[n]->getXDouble() - 471) / 9) + 1][qRound((particules[n]->getYDouble() - 769) / 9)][particules[n]->getCaracteristiques()->getID()])) * coeffTemperature);
            }
        }
    };

    //Particules dans chaque cases d'un quadrillage de maxXmini*maxYmini avec dimension des cases de 9*9 dans le becher
    for (int x(0); x < maxXmini; x++) {
        for (int y(0); y < maxYmini; y++) {
            concentrationBecherParticule[x][y].clear();
            concentrationBecherParticuleTaille[x][y].clear();
            for (int n(0); n < general->nombreParticule(); n++) {
                concentrationBecherParticule[x][y].append(QVector<Particule*>());
                concentrationBecherParticuleTaille[x][y].append(QVector<int>(general->nombreParticule(), 0));
            }
        }
    }
    for (int n(0); n < particules.size(); n++) {
        if(particules[n]->getXDouble() >= 471 && particules[n]->getXDouble() <= 642 && particules[n]->getYDouble() >= 769 && particules[n]->getYDouble() <= 931)
        {
            concentrationBecherParticule[qRound((particules[n]->getXDouble() - 471) / 12)][qRound((particules[n]->getYDouble() - 769) / 12)][particules[n]->getCaracteristiques()->getID()].append(particules[n]);
            concentrationBecherParticuleTaille[qRound((particules[n]->getXDouble() - 471) / 12)][qRound((particules[n]->getYDouble() - 769) / 12)][particules[n]->getCaracteristiques()->getID()]++;
        }
    }
    concentrationBecherParticuleInitialise = true;

    //Réactions entre particules
    double deltaTemperatureBecher(0); //Modification de la température du becher à la fin de toutes les réactions
    QList<int> ordreReaction;
    ordreReaction.append(0);
    if(general->nombreReaction() > 1)
    {
        for (int r(1); r < general->nombreReaction(); r++) {
            ordreReaction.insert(general->getAleatoire()->bounded(0, ordreReaction.size()), r);
        }
    }

    for (int r(0); r < general->nombreReaction(); r++) {
        if(general->getReaction(ordreReaction[r])->getReactifs().size() == 0)
        {
            continue;
        }
        if((general->getReaction(ordreReaction[r])->getPH(pH) == 0) || temperatureBecher < 0)
        {
            continue;
        }
        QList<int> proportionsReactifs(general->getReaction(ordreReaction[r])->getProportionReactifs());
        QList<DonneeParticule*> reactifs(general->getReaction(ordreReaction[r])->getReactifs());
        for (int x(0); x < maxXmini; x++) {
            for (int y(0); y < maxYmini; y++) {
                QList<int> disponibles; //Réactifs disponibles pour la réaction
                int plusPetiteValeur(1000000000); //Nombre de réactions pouvant être effectués
                for (int d(0); d < reactifs.size(); d++) {
                    int valeur(concentrationBecherParticuleTaille[x][y][reactifs[d]->getID()] / proportionsReactifs[d]);
                    disponibles.append(valeur);
                    if(valeur < plusPetiteValeur)
                    {
                        plusPetiteValeur = valeur;
                    }
                }
                double cinetiqueUtilise(general->getReaction(ordreReaction[r])->getCinetiqueKPourT(temperatureBecher, -1));
                for (int k(0); k < general->getReaction(ordreReaction[r])->getCatalyseurs().size(); k++) {
                    if(concentrationBecherParticuleTaille[x][y][general->getReaction(ordreReaction[r])->getCatalyseurs()[k]->getID()] > 0)
                    {
                        if(general->getReaction(ordreReaction[r])->getCinetiqueKPourT(temperatureBecher, k) > cinetiqueUtilise)
                        {
                            cinetiqueUtilise = general->getReaction(ordreReaction[r])->getCinetiqueKPourT(temperatureBecher, k);
                        }
                    }
                }
                cinetiqueUtilise = 1 - pow(1 / qExp(cinetiqueUtilise), elapsed / 60.0) * 1.44; //p(t)
                const int plusPetiteValeurCopie(plusPetiteValeur);
                for (int c(0); c < plusPetiteValeurCopie; c++) {
                    if(cinetiqueUtilise * general->getReaction(ordreReaction[r])->getPH(pH) <= general->getAleatoire()->generateDouble())
                    {
                        plusPetiteValeur--;
                    }
                }
                if(plusPetiteValeur != 1000000000 && plusPetiteValeur != 0)
                {
                    for (int d(0); d < reactifs.size(); d++) {
                        for (int n(0); n < plusPetiteValeur * proportionsReactifs[d]; n++) {
                            concentrationBecherParticule[x][y][reactifs[d]->getID()][n]->planifierSupression();
                            concentrationBecherParticuleTaille[x][y][reactifs[d]->getID()]--;
                        }
                    }
                    QList<int> proportionsProduits(general->getReaction(ordreReaction[r])->getProportionProduits());
                    QList<DonneeParticule*> produits(general->getReaction(ordreReaction[r])->getProduits());
                    for (int d(0); d < produits.size(); d++) {
                        for (int n(0); n < plusPetiteValeur * proportionsProduits[d]; n++) {
                            particules.append(new Particule((x * 12) + 471, (y * 12) + 769, produits[d]));
                        }
                    }
                    deltaTemperatureBecher += general->getReaction(ordreReaction[r])->getTemperatureParReaction(volumeBecher);
                }
            }
        }
    }
    temperatureBecher += deltaTemperatureBecher;

    //Extension de la liste dégagementGazeux si nécessaire
    while (degagementGazeux.size() < general->nombreParticule())
    {
        degagementGazeux.append(0);
    }

    //Supression des particules à supprimer, comptage des particules dans le becher
    int compteParticuleBecher[general->nombreParticule()];
    int compteParticuleBurette[general->nombreParticule()];
    for (int p(0); p < general->nombreParticule(); p++) {
        compteParticuleBecher[p] = 0;
        compteParticuleBurette[p] = 0;
    }
    for (int n(particules.size() - 1); n >= 0; n--) {
        if(particules[n]->getCaracteristiques()->getEtat() == 3 && particules[n]->y() == limiteBecher + 3)
        {
            particules[n]->planifierSupression();
            degagementGazeux[particules[n]->getCaracteristiques()->getID()]++;
        }
        if(particules[n]->aSupprimer())
        {
            delete particules[n];
            particules.removeAt(n);
        }
        else if(particules[n]->getYDouble() > 763)
        {
            compteParticuleBecher[particules[n]->getCaracteristiques()->getID()]++;
        }
        else
        {
            compteParticuleBurette[particules[n]->getCaracteristiques()->getID()]++;
        }
    }
    double absorbanceTotalRougeBe(0);
    double absorbanceTotalVertBe(0);
    double absorbanceTotalBleuBe(0);
    double absorbanceTotalRougeBu(0);
    double absorbanceTotalVertBu(0);
    double absorbanceTotalBleuBu(0);
    for (int p(0); p < general->nombreParticule(); p++) {
        if(volumeBecher == 0)
        {
            particulesListe->at(p)->setValeur("[Becher Vide]");
        }
        else
        {
            double quantiteBe(compteParticuleBecher[p] / 1000000.0);
            double concentrationBe(quantiteBe / (volumeBecher / 1000.0));
            if(!(general->getParticule(p)->getIntensiteCouleur() == 0 || quantiteBe == 0))
            {
                double absorbanceRougeBe(general->getParticule(p)->getRouge() * concentrationBe);
                absorbanceTotalRougeBe += absorbanceRougeBe;
                double absorbanceVertBe(general->getParticule(p)->getVert() * concentrationBe);
                absorbanceTotalVertBe += absorbanceVertBe;
                double absorbanceBleuBe(general->getParticule(p)->getBleu() * concentrationBe);
                absorbanceTotalBleuBe += absorbanceBleuBe;
            }

            QString ajout("");
            if(general->getParticule(p)->getEtat() == 3)
            {
                ajout = " (" + QString::number(degagementGazeux[p] / 1000000.0, 'g', 4) + " mol dégagée)";
            }
            if(concentration)
            {
                particulesListe->at(p)->setValeur(QString::number(concentrationBe, 'g', 4) + " mol.L⁻¹" + ajout);
            }
            else
            {
                particulesListe->at(p)->setValeur(QString::number(quantiteBe, 'g', 4) + " mol" + ajout);
            }
        }
        if(volumeBurette != 0)
        {
            double quantiteBu(compteParticuleBurette[p] / 1000000.0);
            if(!(general->getParticule(p)->getIntensiteCouleur() == 0 || quantiteBu == 0))
            {
                double concentrationBu(quantiteBu / (volumeBecher / 1000.0));
                double absorbanceRougeBu(general->getParticule(p)->getRouge() * concentrationBu);
                absorbanceTotalRougeBu += absorbanceRougeBu;
                double absorbanceVertBu(general->getParticule(p)->getVert() * concentrationBu);
                absorbanceTotalVertBu += absorbanceVertBu;
                double absorbanceBleuBu(general->getParticule(p)->getBleu() * concentrationBu);
                absorbanceTotalBleuBu += absorbanceBleuBu;
            }
        }
    }

    //Couleur solutions
    couleurBecher = QColor((pow(10, 2 - absorbanceTotalRougeBe) * 1.92), (pow(10, 2 - absorbanceTotalVertBe) * 1.92), (pow(10, 2 - absorbanceTotalBleuBe) * 2.00));
    double alphaBe(128 + (absorbanceTotalRougeBe + absorbanceTotalVertBe + absorbanceTotalBleuBe) * 42.0);
    if(alphaBe > 255)
    {
        couleurBecher.setAlpha(255);
    }
    else
    {
        couleurBecher.setAlpha(alphaBe);
    }
    couleurBurette = QColor((pow(10, 2 - absorbanceTotalRougeBu) * 1.92), (pow(10, 2 - absorbanceTotalVertBu) * 1.92), (pow(10, 2 - absorbanceTotalBleuBu) * 2.00));
    double alphaBu(128 + (absorbanceTotalRougeBu + absorbanceTotalVertBu + absorbanceTotalBleuBu) * 42.0);
    if(alphaBu > 255)
    {
        couleurBurette.setAlpha(255);
    }
    else
    {
        couleurBurette.setAlpha(alphaBu);
    }

    //Chronomètre
    if(chronometrePresent || ordinateurPresent)
    {
        chronometre->loop(timerGlobal.elapsed() - totalTempsPause);
    }

    //Thermomètre
    if(thermometrePresent)
    {
        thermometre->loop(temperatureBecher);
    }

    //pHMetre
    if(general->getPH() && volumeBecher != 0)
    {
        calculPH((compteParticuleBecher[general->getIndexH3O()] / 1000000.0) / (volumeBecher / 1000.0), (compteParticuleBecher[general->getIndexHO()] / 1000000.0) / (volumeBecher / 1000.0));
    }
    else
    {
        calculPH();
    }
    if(pHMetrePresent)
    {
        pHMetre->loop(pH);
    }

    //Conductimètre
    if(conductimetrePresent)
    {
        double conductivite(0);
        if(volumeBecher != 0)
        {
            for (int p(0); p < general->nombreParticule(); p++) {
                conductivite += (general->getParticule(p)->getConductivite() / 1000) * ((compteParticuleBecher[p] / 1000000.0) / volumeBecher) * 1000000;
            }
        }
        conductimetre->loop(conductivite);
    }

    //Colorimètre
    if(colorimetrePresent)
    {
        if(volumeBecher == 0)
        {
            colorimetre->loop(0, 0, 0);
        }
        else
        {
            double absorbanceRouge(0);
            double absorbanceVert(0);
            double absorbanceBleu(0);
            for (int p(0); p < general->nombreParticule(); p++) {
                double quantite(compteParticuleBecher[p] / 1000000.0);
                double concentration(quantite / (volumeBecher / 1000.0));
                absorbanceRouge += general->getParticule(p)->getRouge() * concentration;
                absorbanceVert += general->getParticule(p)->getVert() * concentration;
                absorbanceBleu += general->getParticule(p)->getBleu() * concentration;
            }
            colorimetre->loop(absorbanceRouge, absorbanceVert, absorbanceBleu);
        }
    }

    //Ordinateur
    if(ordinateurPresent)
    {
        if(ordinateur->getEnAssimilation())
        {
            ordinateur->loop(chronometre->getValeur(), obtenirValeurPourOrdinateur(ordinateur->getGrandeurX()), obtenirValeurPourOrdinateur(ordinateur->getGrandeurYG()), obtenirValeurPourOrdinateur(ordinateur->getGrandeurYD()));
        }
        else
        {
            ordinateur->loop(chronometre->getValeur());
        }
    }

    repaint();
    QTimer::singleShot(1, this, SLOT(loop()));
}

void Simulation::donneeRemplissageRecu(Remplissage remplir)
{
    if(remplir.getVerrerie()) // Becher
    {
        typeBecher = remplir.getTypeFormat();
        QPixmap becherImg("data/assets/becher" + QString::number(typeBecher * 100) + ".png");
        becherImg = becherImg.scaled(becherImg.width() / 3, becherImg.height() / 3);
        becher->setPixmap(becherImg);
        volumeBecher = remplir.getRemplir();
        temperatureBecher = remplir.getTemperature();
        for (int n(particules.size() - 1); n >= 0; n--) {
            if(particules[n]->getYDouble() >= 763)
            {
                delete particules[n];
                particules.removeAt(n);
            }
        }
        const int limiteBecher((165 - ((volumeBecher / typeBecher) * 1.65)) + 766); //Ordonnée où s'arrête le volume du becher
        for (int n(0); n < remplir.getElements().size(); n++) {
            int ajoutTotal(1000000 * (remplir.getRemplir() / 1000) * remplir.getConcentrations()[n]);
            for (int p(0); p < ajoutTotal; p++) {
                particules.append(new Particule(general->getAleatoire()->bounded(471, 643), general->getAleatoire()->bounded(limiteBecher + 3, 932), general->getParticule(remplir.getElements()[n])));
            }
            nombreDansBecher = ajoutTotal;
        }
        degagementGazeux.clear();
    }
    else //Burette
    {
        typeBurette = remplir.getTypeFormat();
        QPixmap buretteImg("data/assets/burette" + QString::number(typeBurette * 25) + ".png");
        buretteImg = buretteImg.scaled(buretteImg.width() / 3, buretteImg.height() / 3);
        burette->setPixmap(buretteImg);
        volumeBurette = remplir.getRemplir();
        volumeInitialBurette = remplir.getRemplir();
        temperatureBurette = remplir.getTemperature();
        for (int n(particules.size() - 1); n >= 0; n--) {
            if(particules[n]->getYDouble() < 763)
            {
                delete particules[n];
                particules.removeAt(n);
            }
        }
        const int limiteBurette((665 - ((volumeBurette / typeBurette) * 26.6)) + 30); //Ordonnée où s'arrête le volume de la burette
        for (int n(0); n < remplir.getElements().size(); n++) {
            int ajoutTotal(1000000 * (remplir.getRemplir() / 1000) * remplir.getConcentrations()[n]);
            double progressionY(limiteBurette + 71);
            const double etape((762 - (limiteBurette + 71)) / (ajoutTotal + 1.0));
            for (int p(0); p < ajoutTotal; p++) {
                progressionY += etape;
                double yAdd(progressionY + ((general->getAleatoire()->generateDouble() * 16) - 8));
                if(yAdd < limiteBurette + 71)
                {
                    yAdd = limiteBurette + 71;
                }
                if(yAdd > 762)
                {
                    yAdd = 762;
                }
                particules.append(new Particule(general->getAleatoire()->bounded(556, 564), yAdd, general->getParticule(remplir.getElements()[n])));
            }
            nombreDansBurette = ajoutTotal;
        }
    }
    setPaused(false);
}

void Simulation::donneeAnnuler()
{
    setPaused(false);
}

double Simulation::getVolumeBecher() const
{
    return volumeBecher;
}

void Simulation::setVolumeBecher(double value)
{
    volumeBecher = value;
}

double Simulation::obtenirValeurPourOrdinateur(int type)
{
    switch (type) {
    default:
        return 0;
        break;
    case 1:
        return chronometre->getValeur();
        break;
    case 2:
        return volumeInitialBurette - volumeBurette;
        break;
    case 3:
        return thermometre->getValeur();
        break;
    case 4:
        return thermometre->getValeur() + 273.15;
        break;
    case 5:
        return pHMetre->getValeur();
        break;
    case 6:
        return pow(10, -pHMetre->getValeur());
        break;
    case 7:
        return pow(10, -(14 - pHMetre->getValeur()));
        break;
    case 8:
        return conductimetre->getValeur();
        break;
    case 9:
        return colorimetre->getValeur();
        break;
    case 10:
        return pow(10, 2 - colorimetre->getValeur());
        break;
    }
}
