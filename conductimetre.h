#ifndef CONDUCTIMETRE_H
#define CONDUCTIMETRE_H

#include "instrument.h"
#include <QLabel>
#include <QLCDNumber>

/*
 * [Conductimètre]
 *
 * Hérite Instrument
 * valeurAffiche : Conductivité de la solution (S.cm-1)
 */

class Conductimetre : public Instrument
{
    Q_OBJECT
public:
    Conductimetre(General *generalT, QWidget *parent = nullptr);
    ~Conductimetre();

    void loop(double valeur);
    //Valeur : Conductivité en S.cm-1

private:
    QLabel *objet;
    QLCDNumber *affichage;

    int unite; //0 = normal ; 1 = mili ; 2 = micro
    QPixmap normal;
    QPixmap mili;
    QPixmap micro;
};

#endif // CONDUCTIMETRE_H
