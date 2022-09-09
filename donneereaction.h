#ifndef DONNEEREACTION_H
#define DONNEEREACTION_H

#include <QList>
#include <QtMath>
#include "donneeparticule.h"

/*
 * [DonneeReaction]
 *
 * Donnée d'une réaction
 *
 * Contient :
 * ID : Un ID unique
 * proportionReactifs ET reactifs : Liste des réactifs et leurs proportions
 * proportionProduits ET produits : Liste des produits et leurs proportions
 * efficaciteCatalyseurs ET catalyseurs : Liste des catalyseurs et l'énergie d'activation attribuée en leur présence
 * facteurPreExp ET energieActivation : Constante de la cinétique
 * deltaTemperature : Changement de température par réaction, valeur en Joule (4.18 J -> +1°C pour 1 mL
 * pHMin ET pHMax : Borne du pH où la réaction peut se produire
 *
 */

class DonneeReaction
{
public:
    DonneeReaction(int IDa);

    void setTout(DonneeReaction change);

    int getID() const;
    void setID(int value);

    //Les sets ci-dessous ne delete pas la particule précédente
    QList<int> getProportionReactifs() const;
    QList<DonneeParticule *> getReactifs() const;
    void setReactif(int index, DonneeParticule* particule);
    void setProportionReactif(int index, int proportion);
    void supprimerReactif(int index);
    QList<int> getProportionProduits() const;
    QList<DonneeParticule *> getProduits() const;
    void setProduit(int index, DonneeParticule* particule);
    void setProportionProduit(int index, int proportion);
    void supprimerProduit(int index);
    QList<double> getEfficaciteCatalyseurs() const;
    QList<DonneeParticule *> getCatalyseurs() const;
    void setCatalyseur(int index, DonneeParticule* particule);
    void setEfficaciteCatalyseur(int index, double efficacite);
    void supprimerCatalyseur(int index);

    void addReactif(int proportion, DonneeParticule* particule);
    void addProduit(int proportion, DonneeParticule* particule);
    void addCatalyseur(double efficacite, DonneeParticule* particule);

    double getFacteurPreExp() const;
    void setFacteurPreExp(double value);
    double getEnergieActivation() const;
    void setEnergieActivation(double value);

    double getCinetiqueKPourT(double temperature, int catalyseur);

    double getDeltaTemperature() const;
    void setDeltaTemperature(double value);
    double getTemperatureParReaction(double volume) const;

    double getPH(double pH);
    void setPH(double pHAA, double pHBA);

    QString formaterEquation();
    QColor formaterCouleurProduits();

    double getPHA() const;
    double getPHB() const;

    QList<int> getReactifsID() const;
    QList<int> getProduitsID() const;
    QList<int> getCatalyseursID() const;

    void oublierLimiteExp();

private:
    int ID;

    //Particule

    QList<int> proportionReactifs;
    QList<DonneeParticule*> reactifs;
    QList<int> reactifsID;
    QList<int> proportionProduits;
    QList<DonneeParticule*> produits;
    QList<int> produitsID;
    QList<double> efficaciteCatalyseurs;
    QList<DonneeParticule*> catalyseurs;
    QList<int> catalyseursID;

    //Cinétique

    double facteurPreExp;
    double energieActivation;

    //Température

    double deltaTemperature; //En J/mol

    //Mémoire pour getAleatoirePourT() afin de limiter l'appel de qExp()
    double derniereTemperature;
    double derniereValeurPourT;
    QList<double> derniereTemperatureCatalyse;
    QList<double> derniereValeurPourTCatalyse;

    //pH

    double pHA;
    double pHB;

    //Mémoire pour getPH() afin de limiter l'appel de qExp()
    double dernierPH;
    double derniereValeurPourPH;

};

#endif // DONNEEREACTION_H
