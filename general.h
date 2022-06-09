#ifndef GENERAL_H
#define GENERAL_H

#include <QList>
#include <QRandomGenerator>
#include <QDateTime>
#include "donneeparticule.h"
#include "donneereaction.h"

/*
 * [General]
 *
 * Contient toutes les variables globales à tout les objets
 * Un seul objet de cette classe est créé dans MainWindow et son pointeur est transmis à tout les objets en ayant besoin
 */

class General
{
public:
    General();

    void setTout(General change);
    General* creerCopie();
    void vider(); //Supprime toutes les DonneeParticule et les DonneeReaction

    bool getPH() const;
    void setPH(bool value);

    DonneeParticule* getParticule(int index) const;
    void setParticule(int index, const DonneeParticule &value);
    void addParticule(const DonneeParticule &value);
    int nombreParticule() const;

    DonneeReaction* getReaction(int index) const;
    void setReaction(int index, const DonneeReaction &value);
    void addReaction(const DonneeReaction &value);
    int nombreReaction() const;

    QRandomGenerator* getAleatoire() const;

    bool getChangementRecent() const;
    void setChangementRecent(bool value);

    int getIndexH3O() const;

    int getIndexHO() const;

    QList<DonneeParticule *> getParticules() const;
    QList<DonneeReaction *> getReactions() const;

    QString getNomDeFichier() const;
    void setNomDeFichier(const QString &value);

    bool getPossedeUnFichier() const;
    void setPossedeUnFichier(bool value);

    QString getNom() const;
    void setNom(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

private:
    //Mode
    bool pH;
    int indexH3O;
    int indexHO;

    //Données
    QList<DonneeParticule*> particules;
    QList<DonneeReaction*> reactions;
    bool changementRecent;

    //Divers
    QRandomGenerator *aleatoire;

    //Fichiers
    QString nomDeFichier;
    bool possedeUnFichier;
    QString nom;
    QString description;
};

#endif // GENERAL_H
