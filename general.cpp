#include "general.h"

General::General()
{
    aleatoire = new QRandomGenerator(QDateTime::currentSecsSinceEpoch()); //Seed choisi en fonction de la date
    pH = false;
    changementRecent = true;
    indexH3O = -1;
    indexHO = -1;
    possedeUnFichier = false;
    nom = "";
    nomDeFichier = "";
    description = "";
}

void General::setTout(General change)
{
    possedeUnFichier = change.getPossedeUnFichier();
    nomDeFichier = change.getNomDeFichier();
    nom = change.getNom();
    description = change.getDescription();
    setPH(change.getPH());
    qDeleteAll(particules);
    particules.clear();
    qDeleteAll(reactions);
    reactions.clear();
    for (int IDp(0); IDp < change.nombreParticule(); IDp++) {
        particules.append(change.getParticule(IDp));
    }
    for (int IDp(0); IDp < change.nombreReaction(); IDp++) {
        reactions.append(change.getReaction(IDp));
        for (int IDr(0); IDr < reactions[IDp]->getReactifs().size(); IDr++) {
            reactions[IDp]->setReactif(IDr, particules[reactions[IDp]->getReactifsID()[IDr]]);
        }
        for (int IDr(0); IDr < reactions[IDp]->getProduits().size(); IDr++) {
            reactions[IDp]->setProduit(IDr, particules[reactions[IDp]->getProduitsID()[IDr]]);
        }
        for (int IDr(0); IDr < reactions[IDp]->getCatalyseurs().size(); IDr++) {
            reactions[IDp]->setCatalyseur(IDr, particules[reactions[IDp]->getCatalyseursID()[IDr]]);
        }
    }
    indexHO = change.getIndexHO();
    indexH3O = change.getIndexH3O();
    changementRecent = true;
}

General* General::creerCopie()
{
    General* returned(new General);
    returned->setPossedeUnFichier(possedeUnFichier);
    returned->setNomDeFichier(nomDeFichier);
    returned->setNom(nom);
    returned->setDescription(description);
    returned->setPH(pH);
    returned->setChangementRecent(true);
    QList<DonneeParticule*> nouvellesParticules;
    for (int IDp(0); IDp < nombreParticule(); IDp++) {
        returned->addParticule(*particules[IDp]);
        nouvellesParticules.append(returned->getParticule(IDp));
    }
    for (int IDp(0); IDp < nombreReaction(); IDp++) {
        returned->addReaction(*reactions[IDp]);
        for (int IDr(0); IDr < returned->getReaction(IDp)->getReactifs().size(); IDr++) {
            returned->getReaction(IDp)->setReactif(IDr, nouvellesParticules[returned->getReaction(IDp)->getReactifs()[IDr]->getID()]);
        }
        for (int IDr(0); IDr < returned->getReaction(IDp)->getProduits().size(); IDr++) {
            returned->getReaction(IDp)->setProduit(IDr, nouvellesParticules[returned->getReaction(IDp)->getProduits()[IDr]->getID()]);
        }
        for (int IDr(0); IDr < returned->getReaction(IDp)->getCatalyseurs().size(); IDr++) {
            returned->getReaction(IDp)->setCatalyseur(IDr, nouvellesParticules[returned->getReaction(IDp)->getCatalyseurs()[IDr]->getID()]);
        }
    }
    return returned;
}

void General::vider()
{
    qDeleteAll(particules);
    qDeleteAll(reactions);
}

bool General::getPH() const
{
    return pH;
}

void General::setPH(bool value)
{
    pH = value;
}

DonneeParticule* General::getParticule(int index) const
{
    return particules[index];
}

void General::setParticule(int index, const DonneeParticule &value)
{
    delete particules[index];
    particules[index] = new DonneeParticule(value);
    changementRecent = true;
    switch (value.getPHValeur()) {
    case -1:
        indexHO = index;
        break;
    case 1:
        indexH3O = index;
        break;
    }
}

void General::addParticule(const DonneeParticule &value)
{
    particules.append(new DonneeParticule(value));
    changementRecent = true;
    switch (value.getPHValeur()) {
    case -1:
        indexHO = particules.size() - 1;
        break;
    case 1:
        indexH3O = particules.size() - 1;
        break;
    }
}

int General::nombreParticule() const
{
    return particules.size();
}

DonneeReaction *General::getReaction(int index) const
{
    return reactions[index];
}

void General::setReaction(int index, const DonneeReaction &value)
{
    DonneeReaction* suppression(reactions[index]);
    reactions[index] = new DonneeReaction(value);
    changementRecent = true;
    delete suppression;
}

void General::addReaction(const DonneeReaction &value)
{
    reactions.append(new DonneeReaction(value));
    changementRecent = true;
}

int General::nombreReaction() const
{
    return reactions.size();
}

QRandomGenerator* General::getAleatoire() const
{
    return aleatoire;
}

bool General::getChangementRecent() const
{
    return changementRecent;
}

void General::setChangementRecent(bool value)
{
    changementRecent = value;
}

int General::getIndexH3O() const
{
    return indexH3O;
}

int General::getIndexHO() const
{
    return indexHO;
}

QList<DonneeParticule *> General::getParticules() const
{
    return particules;
}

QList<DonneeReaction *> General::getReactions() const
{
    return reactions;
}

QString General::getNomDeFichier() const
{
    return nomDeFichier;
}

void General::setNomDeFichier(const QString &value)
{
    nomDeFichier = value;
}

bool General::getPossedeUnFichier() const
{
    return possedeUnFichier;
}

void General::setPossedeUnFichier(bool value)
{
    possedeUnFichier = value;
}

QString General::getNom() const
{
    return nom;
}

void General::setNom(const QString &value)
{
    nom = value;
}

QString General::getDescription() const
{
    return description;
}

void General::setDescription(const QString &value)
{
    description = value;
}
