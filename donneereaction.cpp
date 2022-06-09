#include "donneereaction.h"

DonneeReaction::DonneeReaction(int IDa)
{
    ID = IDa;
    facteurPreExp = 200;
    energieActivation = 0;
    deltaTemperature = 0;
    setPH(-999, 1, false);
    derniereTemperature = -300;
    derniereValeurPourT = 0;
}

void DonneeReaction::setTout(DonneeReaction change)
{
    facteurPreExp = change.getFacteurPreExp();
    energieActivation = change.getEnergieActivation();
    deltaTemperature = change.getDeltaTemperature();
    proportionReactifs = change.getProportionReactifs();
    reactifs = change.getReactifs();
    reactifsID = change.getReactifsID();
    proportionProduits = change.getProportionProduits();
    produits = change.getProduits();
    produitsID = change.getProduitsID();
    efficaciteCatalyseurs = change.getEfficaciteCatalyseurs();
    catalyseurs = change.getCatalyseurs();
    catalyseursID = change.getCatalyseursID();
    setPH(change.getPHCentre(), change.getPHAmplitude(), change.getVersAcide());
}

int DonneeReaction::getID() const
{
    return ID;
}

void DonneeReaction::setID(int value)
{
    ID = value;
}

QList<int> DonneeReaction::getProportionReactifs() const
{
    return proportionReactifs;
}

QList<DonneeParticule *> DonneeReaction::getReactifs() const
{
    return reactifs;
}

void DonneeReaction::setReactif(int index, DonneeParticule *particule)
{
    reactifs[index] = particule;
    reactifsID[index] = particule->getID();
}

void DonneeReaction::setProportionReactif(int index, int proportion)
{
    proportionReactifs[index] = proportion;
}

void DonneeReaction::supprimerReactif(int index)
{
    reactifs.removeAt(index);
    reactifsID.removeAt(index);
    proportionReactifs.removeAt(index);
}

QList<int> DonneeReaction::getProportionProduits() const
{
    return proportionProduits;
}

QList<DonneeParticule *> DonneeReaction::getProduits() const
{
    return produits;
}

void DonneeReaction::setProduit(int index, DonneeParticule *particule)
{
    produits[index] = particule;
    produitsID[index] = particule->getID();
}

void DonneeReaction::setProportionProduit(int index, int proportion)
{
    proportionProduits[index] = proportion;
}

void DonneeReaction::supprimerProduit(int index)
{
    produits.removeAt(index);
    produitsID.removeAt(index);
    proportionProduits.removeAt(index);
}

QList<double> DonneeReaction::getEfficaciteCatalyseurs() const
{
    return efficaciteCatalyseurs;
}

QList<DonneeParticule *> DonneeReaction::getCatalyseurs() const
{
    return catalyseurs;
}

void DonneeReaction::setCatalyseur(int index, DonneeParticule *particule)
{
    catalyseurs[index] = particule;
    catalyseursID[index] = particule->getID();
}

void DonneeReaction::setEfficaciteCatalyseur(int index, double efficacite)
{
    efficaciteCatalyseurs[index] = efficacite;
}

void DonneeReaction::supprimerCatalyseur(int index)
{
    catalyseurs.removeAt(index);
    catalyseursID.removeAt(index);
    efficaciteCatalyseurs.removeAt(index);
}

void DonneeReaction::addReactif(int proportion, DonneeParticule *particule)
{
    proportionReactifs.append(proportion);
    reactifs.append(particule);
    reactifsID.append(particule->getID());
}

void DonneeReaction::addProduit(int proportion, DonneeParticule *particule)
{
    proportionProduits.append(proportion);
    produits.append(particule);
    produitsID.append(particule->getID());
}

void DonneeReaction::addCatalyseur(double efficacite, DonneeParticule *particule)
{
    efficaciteCatalyseurs.append(efficacite);
    catalyseurs.append(particule);
    derniereTemperatureCatalyse.append(-300);
    derniereValeurPourTCatalyse.append(0);
    catalyseursID.append(particule->getID());
}

double DonneeReaction::getFacteurPreExp() const
{
    return facteurPreExp;
}

void DonneeReaction::setFacteurPreExp(double value)
{
    facteurPreExp = value;
}

double DonneeReaction::getEnergieActivation() const
{
    return energieActivation;
}

void DonneeReaction::setEnergieActivation(double value)
{
    energieActivation = value;
}

double DonneeReaction::getCinetiqueKPourT(double temperature, int catalyseur)
{
    if(catalyseur == -1)
    {
        if(derniereTemperature == temperature)
        {
            return derniereValeurPourT;
        }
        double returned(facteurPreExp * qExp(-energieActivation / (8.31446261815324 * (temperature + 273.15))));
        derniereTemperature = temperature;
        derniereValeurPourT = returned;
        return returned;
    }
    else
    {
        if(derniereTemperatureCatalyse[catalyseur] == temperature)
        {
            return derniereValeurPourTCatalyse[catalyseur];
        }
        double returned(facteurPreExp * qExp(-efficaciteCatalyseurs[catalyseur] / (8.31446261815324 * (temperature + 273.15))));
        derniereTemperatureCatalyse[catalyseur] = temperature;
        derniereValeurPourTCatalyse[catalyseur] = returned;
        return returned;
    }
}

double DonneeReaction::getDeltaTemperature() const
{
    return deltaTemperature;
}

void DonneeReaction::setDeltaTemperature(double value)
{
    deltaTemperature = value;
}

double DonneeReaction::getTemperatureParReaction(double volume) const
{
    return ((deltaTemperature * 1E-6) / 4.18) / volume;
}

double DonneeReaction::getPH(double pH) const
{
    double calcul(pHA * pH + pHB);
    if(calcul > 1)
    {
        return 1;
    }
    else if(calcul < 0)
    {
        return 0;
    }
    else
    {
        return calcul;
    }
}

void DonneeReaction::setPH(double pHCentreA, double pHAmplitudeA, bool versAcideA)
{
    pHCentre = pHCentreA;
    pHAmplitude = pHAmplitudeA;
    versAcide = versAcideA;
    if(versAcide)
    {
        pHA = -1 / (2 * pHAmplitude);
        pHB = -(pHA * (pHCentre + pHAmplitude));
    }
    else
    {
        pHA = 1 / (2 * pHAmplitude);
        pHB = -(pHA * (pHCentre - pHAmplitude));
    }
}

QString DonneeReaction::formaterEquation()
{
    if(reactifs.size() == 0)
    {
        return "Réaction invalide";
    }
    QString returned("");
    for (int r(0); r < reactifs.size(); r++) {
        int proportion(proportionReactifs[r]);
        if(proportion == 1)
        {
            returned.append(reactifs[r]->getFormule());
        }
        else
        {
            returned.append(QString::number(proportion) + reactifs[r]->getFormule());
        }
        if(r + 1 < reactifs.size())
        {
            returned.append(" + ");
        }
    }
    returned.append("   ->   ");
    if(produits.size() == 0)
    {
        returned.append("cH2O");
    }
    else
    {
        for (int p(0); p < produits.size(); p++) {
            int proportion(proportionProduits[p]);
            if(proportion == 1)
            {
                returned.append(produits[p]->getFormule());
            }
            else
            {
                returned.append(QString::number(proportion) + produits[p]->getFormule());
            }
            if(p + 1 < produits.size())
            {
                returned.append(" + ");
            }
        }
    }
    if(deltaTemperature != 0)
    {
        if(deltaTemperature > 0)
        {
            if(deltaTemperature >= 1000000)
            {
                returned += " + " + QString::number(deltaTemperature / 1000000) + "MJ";
            }
            else if(deltaTemperature >= 1000)
            {
                returned += " + " + QString::number(deltaTemperature / 1000) + "kJ";
            }
            else
            {
                returned += " + " + QString::number(deltaTemperature) + "J";
            }
        }
        else
        {
            if(deltaTemperature <= -1000000)
            {
                returned = QString::number(-deltaTemperature / 1000000) + "MJ + " + returned;
            }
            else if(deltaTemperature <= -1000)
            {
                returned = QString::number(-deltaTemperature / 1000) + "kJ + " + returned;
            }
            else
            {
                returned = QString::number(-deltaTemperature) + "J + " + returned;
            }
        }
    }
    return returned;
}

QColor DonneeReaction::formaterCouleurProduits()
{
    if(produits.size() == 0)
    {
        return QColor(128, 128, 128);
    }
    int rouge(0), vert(0), bleu(0);
    for (int p(0); p < produits.size(); p++) {
        rouge += produits[p]->getCouleurParticule().red();
        vert += produits[p]->getCouleurParticule().green();
        bleu += produits[p]->getCouleurParticule().blue();
    }
    return QColor(rouge / produits.size(), vert / produits.size(), bleu / produits.size());
}

double DonneeReaction::getPHCentre() const
{
    return pHCentre;
}

double DonneeReaction::getPHAmplitude() const
{
    return pHAmplitude;
}

bool DonneeReaction::getVersAcide() const
{
    return versAcide;
}

QList<int> DonneeReaction::getReactifsID() const
{
    return reactifsID;
}

QList<int> DonneeReaction::getProduitsID() const
{
    return produitsID;
}

QList<int> DonneeReaction::getCatalyseursID() const
{
    return catalyseursID;
}

void DonneeReaction::oublierLimiteExp()
{
    derniereTemperature = -300;
    derniereValeurPourT = 0;
    for (int r(0); r < derniereTemperatureCatalyse.size(); r++) {
        derniereTemperatureCatalyse[r] = -300;
        derniereValeurPourTCatalyse[r] = 0;
    }
}
