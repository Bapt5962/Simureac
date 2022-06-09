#include "remplissage.h"

Remplissage::Remplissage()
{

}

int Remplissage::getTypeFormat() const
{
    return typeFormat;
}

void Remplissage::setTypeFormat(int value)
{
    typeFormat = value;
}

double Remplissage::getRemplir() const
{
    return remplir;
}

void Remplissage::setRemplir(double value)
{
    remplir = value;
}

QList<int> Remplissage::getElements() const
{
    return elements;
}

void Remplissage::setElements(const QList<int> &value)
{
    elements = value;
}

QList<double> Remplissage::getConcentrations() const
{
    return concentrations;
}

void Remplissage::setConcentrations(const QList<double> &value)
{
    concentrations = value;
}

bool Remplissage::getVerrerie() const
{
    return verrerie;
}

void Remplissage::setVerrerie(bool value)
{
    verrerie = value;
}

double Remplissage::getTemperature() const
{
    return temperature;
}

void Remplissage::setTemperature(double value)
{
    temperature = value;
}
