#include "instrument.h"

Instrument::Instrument(General *generalT, QWidget *parent) : QWidget(parent)
{
    general = generalT;
    valeurAffiche = 0;
}

double Instrument::getValeur() const
{
    return valeurAffiche;
}
