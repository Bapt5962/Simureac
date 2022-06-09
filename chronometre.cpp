#include "chronometre.h"

Chronometre::Chronometre(General *generalT, QWidget *parent) : Instrument(generalT, parent)
{
    objet = new QLabel(this);
    QPixmap img("data/assets/chronometre.png");
    img = img.scaled(img.width() / 3, img.height() / 3);
    objet->setPixmap(img);
    affichage = new QLCDNumber(this);
    affichage->setDigitCount(16);
    affichage->setSmallDecimalPoint(true);
    affichage->display("----------------");
    affichage->move(25, 30);
    affichage->setPalette(QPalette(QColor(0, 0, 0)));
    resize(img.width(), img.height());
    secondesSeul = true;
    enPause = false;
    pauseAvantAssimilation = false;
    demarrerA = 0;
    ajouterTemps = 0;

    start = new QPushButton("[S]tart", this);
    start->move(195, 8);
    start->setFixedSize(65, 23);
    connect(start, SIGNAL(clicked()), this, SLOT(startClicked()));
    reset = new QPushButton("[R]eset", this);
    reset->move(195, 30);
    reset->setFixedSize(65, 23);
    connect(reset, SIGNAL(clicked()), this, SLOT(resetClicked()));
    mode = new QPushButton("[M]ode", this);
    mode->move(195, 52);
    mode->setFixedSize(65, 23);
    connect(mode, SIGNAL(clicked()), this, SLOT(modeClicked()));
}

Chronometre::~Chronometre()
{
    delete objet;
    delete affichage;
    delete start;
    delete reset;
    delete mode;
    exit(EXIT_SUCCESS);
    /*  Situation : Le logiciel débute sont arrêt à la demande de l'utilisateur
     *  Problème  : Le logiciel crash quand il s'éteint
     *  Solution  : Eteindre le logiciel avant que celui-ci ne crash en s'éteignant
     */
}

void Chronometre::afficherValeur()
{
    double secondes(valeurAffiche);
    if(secondesSeul)
    {
        QString display(QString::number(secondes));
        if(display.indexOf('.') == -1) {
            display.append(".000");
        }
        else if(display.indexOf('.') == display.size() - 2) {
            display.append("00");
        }
        else if(display.indexOf('.') == display.size() - 3) {
            display.append("0");
        }
        else if(display.indexOf('.') < display.size() - 4) {
            display.resize(display.indexOf('.') + 4);
        }
        if(display.size() > 17)
        {
            display = "Error";
        }
        affichage->display(display);
    }
    else
    {
        double milisecondes(secondes - floor(secondes));
        long milisecondesInt = qRound(milisecondes * 1000);
        long secondesInt = floor(secondes);
        long minutes = secondesInt / 60;
        secondesInt = secondesInt % 60;
        long heures = minutes / 60;
        minutes = minutes % 60;
        if(heures >= 1000000)
        {
            affichage->display("Error");
        }
        else
        {
            QString display;
            display += QString::number(heures) + ":";
            if(minutes < 10)
            {
                display += "0" + QString::number(minutes) + ":";
            }
            else
            {
                display += QString::number(minutes) + ":";
            }
            if(secondesInt < 10)
            {
                display += "0" + QString::number(secondesInt) + ".";
            }
            else
            {
                display += QString::number(secondesInt) + ".";
            }
            if(milisecondesInt < 10)
            {
                display += "00" + QString::number(milisecondesInt);
            }
            else if(milisecondesInt < 100)
            {
                display += "0" + QString::number(milisecondesInt);
            }
            else
            {
                display += QString::number(milisecondesInt);
            }
            affichage->display(display);
        }
    }
}

void Chronometre::loop(long valeur)
{
    double secondes(valeur / 1000.0);
    tempsActuel = secondes;
    if(enPause)
    {
        return;
    }
    valeurAffiche = (secondes - demarrerA) + ajouterTemps;
    afficherValeur();
}

void Chronometre::oublierTemps(double valeur)
{
    ajouterTemps -= valeur / 1000.0;
}

void Chronometre::startClicked()
{
    if(enPause)
    {
        enPause = false;
        demarrerA = tempsActuel;
        ajouterTemps = tempsMiseEnPause;
    }
    else
    {
        enPause = true;
        tempsMiseEnPause = (tempsActuel - demarrerA) + ajouterTemps;
    }
}

void Chronometre::resetClicked()
{
    demarrerA = tempsActuel;
    ajouterTemps = 0;
    valeurAffiche = 0;
    tempsMiseEnPause = 0;
    afficherValeur();
}

void Chronometre::modeClicked()
{
    secondesSeul = !secondesSeul;
    afficherValeur();
}

void Chronometre::bloquerBoutons(bool bloquer)
{
    start->setDisabled(bloquer);
    reset->setDisabled(bloquer);
    if(bloquer)
    {
        if(enPause)
        {
            startClicked();
            pauseAvantAssimilation = true;
        }
    }
    else
    {
        if(pauseAvantAssimilation)
        {
            startClicked();
            pauseAvantAssimilation = false;
        }
    }
}
