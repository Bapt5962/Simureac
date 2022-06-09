#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H

#include "instrument.h"
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>

/*
 * [Chronomètre]
 *
 * Hérite Instrument
 * valeurAffiche : Temps affiché (s)
 */

class Chronometre : public Instrument
{
    Q_OBJECT
public:
    Chronometre(General *generalT, QWidget *parent = nullptr);
    ~Chronometre();
    void afficherValeur();

    void loop(long valeur);
    //Valeur : Temps en ms

    void oublierTemps(double valeur); //Permet d'ignorer les lag spikes

public slots:
    void startClicked();
    void resetClicked();
    void modeClicked();
    void bloquerBoutons(bool bloquer); //Durant une assimilation par l'ordinateur

private:
    QLabel *objet;
    QLCDNumber *affichage;

    bool secondesSeul;
    bool enPause;
    bool pauseAvantAssimilation;
    double tempsMiseEnPause;

    double tempsActuel;
    double demarrerA;
    double ajouterTemps;

    QPushButton *start;
    QPushButton *reset;
    QPushButton *mode;
};

#endif // CHRONOMETRE_H
