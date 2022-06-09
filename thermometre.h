#ifndef THERMOMETRE_H
#define THERMOMETRE_H

#include "instrument.h"
#include <QLabel>
#include <QLCDNumber>

/*
 * [Thermometre]
 *
 * Hérite Instrument
 * valeurAffiche : température en °C
 *
 */

class Thermometre : public Instrument
{
    Q_OBJECT
public:
    Thermometre(General *generalT, QWidget *parent = nullptr);
    ~Thermometre();

    void loop(double valeur);
    //Valeur : Température en °C

private:
    QLabel *objet;
    QLCDNumber *affichage;
};

#endif // THERMOMETRE_H
