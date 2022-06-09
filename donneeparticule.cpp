#include "donneeparticule.h"

DonneeParticule::DonneeParticule(int IDa)
{
    nom = "Erreur";
    formule = "Erreur";
    couleurParticule = QColor(0, 0, 0);
    etat = 0;
    //couleurFond = QColor(0, 0, 0);
    pHValeur = 0;
    conductivite = 0;
    rouge = 0;
    vert = 0;
    bleu = 0;
    intensiteCouleur = 0;
    ID = IDa;
    nomDeFichier = "";
    possedeUnFichier = false;
}

DonneeParticule::~DonneeParticule()
{
    ID = 0;
}

void DonneeParticule::calculerCouleurFond()
{
    if(rouge > vert && rouge > bleu)
    {
        intensiteCouleur = rouge;
    }
    if(vert > rouge && vert > bleu)
    {
        intensiteCouleur = vert;
    }
    if(bleu > rouge && bleu > vert)
    {
        intensiteCouleur = bleu;
    }
}

void DonneeParticule::setTout(DonneeParticule change)
{
    setPossedeUnFichier(change.getPossedeUnFichier());
    setNomDeFichier(change.getNomDeFichier());
    setNom(change.getNom());
    setCouleurParticule(change.getCouleurParticule());
    setEtat(change.getEtat());
    setPHValeur(change.getPHValeur());
    setFormule(change.getFormule());
    setConductivite(change.getConductivite());
    setRouge(change.getRouge());
    setVert(change.getVert());
    setBleu(change.getBleu());
}

QString DonneeParticule::getNom() const
{
    return nom;
}

void DonneeParticule::setNom(const QString &value)
{
    nom = value;
}

QColor DonneeParticule::getCouleurParticule() const
{
    return couleurParticule;
}

void DonneeParticule::setCouleurParticule(const QColor &value)
{
    couleurParticule = value;
}

int DonneeParticule::getPHValeur() const
{
    return pHValeur;
}

void DonneeParticule::setPHValeur(int value)
{
    pHValeur = value;
}

QString DonneeParticule::getFormule() const
{
    return formule;
}

void DonneeParticule::setFormule(const QString &value)
{
    formule = value;
}

double DonneeParticule::getConductivite() const
{
    return conductivite;
}

void DonneeParticule::setConductivite(double value)
{
    conductivite = value;
}

double DonneeParticule::getRouge() const
{
    return rouge;
}

void DonneeParticule::setRouge(double value)
{
    rouge = value;
    calculerCouleurFond();
}

double DonneeParticule::getVert() const
{
    return vert;
}

void DonneeParticule::setVert(double value)
{
    vert = value;
    calculerCouleurFond();
}

double DonneeParticule::getBleu() const
{
    return bleu;
}

void DonneeParticule::setBleu(double value)
{
    bleu = value;
    calculerCouleurFond();
}

int DonneeParticule::getID() const
{
    return ID;
}

void DonneeParticule::setID(int value)
{
    ID = value;
}

int DonneeParticule::getEtat() const
{
    return etat;
}

void DonneeParticule::setEtat(int value)
{
    etat = value;
}

double DonneeParticule::getIntensiteCouleur() const
{
    return intensiteCouleur;
}

QString DonneeParticule::getNomDeFichier() const
{
    return nomDeFichier;
}

void DonneeParticule::setNomDeFichier(const QString &value)
{
    nomDeFichier = value;
}

bool DonneeParticule::getPossedeUnFichier() const
{
    return possedeUnFichier;
}

void DonneeParticule::setPossedeUnFichier(bool value)
{
    possedeUnFichier = value;
}
