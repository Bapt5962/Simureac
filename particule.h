#ifndef PARTICULE_H
#define PARTICULE_H

#include <QPoint>
#include "donneeparticule.h"

/*
 * [Particule]
 *
 * Coordonnées et caractéristiques d'une particule de l'animation
 * Le rendu est généré dans Simulation
 * Une Particule représente 1E-6 mol
 */

class Particule : public QPoint
{
public:
    Particule(double x, double y, DonneeParticule *particule);

    DonneeParticule *getCaracteristiques() const;
    void setCaracteristiques(DonneeParticule *value);

    double getXDouble() const;
    void setXDouble(double value);

    double getYDouble() const;
    void setYDouble(double value);

    bool aSupprimer() const;
    void planifierSupression();

    bool getAttirance() const;
    void setAttirance(bool value);

    float getVibrationSolide() const;
    void setVibrationSolide(float value);

    int getIDCaracteristiques() const;

private:
    //Coordonnées réelles
    double xDouble;
    double yDouble;

    bool attirance;

    int IDCaracteristiques;
    DonneeParticule *caracteristiques;

    bool supprimer;

    float vibrationSolide;
};

#endif // PARTICULE_H
