#ifndef PHMETRE_H
#define PHMETRE_H

#include "instrument.h"
#include <QLabel>
#include <QLCDNumber>

/*
 * [PHMetre]
 *
 * Hérite Instrument
 * valeurAffiche : pH de la solution
 *
 * Calcul réalisé dans Simulation :
 * diff = [H30+] - [HO-]
 * diff > 0:
 * pH calculé sur la concentration de [H30+]
 * diff < 0:
 * pH calculé sur le pHO calculé sur la concentration de [HO-]
 *
 */

class PHMetre : public Instrument
{
    Q_OBJECT
public:
    PHMetre(General *generalT, QWidget *parent = nullptr);
    ~PHMetre();

    void loop(double valeur);
    //Valeur : pH

private:
    QLabel *objet;
    QLCDNumber *affichage;
};

#endif // PHMETRE_H
