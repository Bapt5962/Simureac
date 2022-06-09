#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QWidget>
#include "general.h"

/*
 * [Instrument]
 *
 * Un instrument non défini
 * valeurAffiche ET uniteAffiche sont utilisés par l'ordinateur
 *
 */

class Instrument : public QWidget
{
    Q_OBJECT
public:
    explicit Instrument(General *generalT, QWidget *parent = nullptr);

    double getValeur() const;

protected:
    General *general;
    double valeurAffiche;

};

#endif // INSTRUMENT_H
