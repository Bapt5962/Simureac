#include "particule.h"

Particule::Particule(double x, double y, DonneeParticule *particule) : QPoint(x, y)
{
    caracteristiques = particule;
    IDCaracteristiques = particule->getID();
    xDouble = x;
    yDouble = y;
    supprimer = false;
    attirance = false;
    vibrationSolide = 1;
}

DonneeParticule *Particule::getCaracteristiques() const
{
    return caracteristiques;
}

void Particule::setCaracteristiques(DonneeParticule *value)
{
    caracteristiques = value;
    IDCaracteristiques = value->getID();
}

double Particule::getXDouble() const
{
    return xDouble;
}

void Particule::setXDouble(double value)
{
    xDouble = value;
    setX(xDouble);
}

double Particule::getYDouble() const
{
    return yDouble;
}

void Particule::setYDouble(double value)
{
    yDouble = value;
    setY(yDouble);
}

bool Particule::aSupprimer() const
{
    return supprimer;
}

void Particule::planifierSupression()
{
    supprimer = true;
}

bool Particule::getAttirance() const
{
    return attirance;
}

void Particule::setAttirance(bool value)
{
    attirance = value;
}

float Particule::getVibrationSolide() const
{
    return vibrationSolide;
}

void Particule::setVibrationSolide(float value)
{
    vibrationSolide = value;
}

int Particule::getIDCaracteristiques() const
{
    return IDCaracteristiques;
}
