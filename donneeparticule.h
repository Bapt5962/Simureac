#ifndef DONNEEPARTICULE_H
#define DONNEEPARTICULE_H

#include <QString>
#include <QColor>

/*
 * [DonneeParticule]
 *
 * Donnée d'une particule
 *
 * Contient :
 * ID : Un ID unique
 * nom : Un nom (Purement visuelle)
 * formule : Une formule (Purement visuelle)
 * couleurParticule : Une couleur de particule (Purement visuelle)
 * etat : L'état de la particule (aqueux/solide/liquide/gazeux)
 * pHValeur : 1 pour H30+ ; -1 pour HO- ; 0 pour le reste
 * [ABSORBANCE PAS TERMINE]
 * conductivite : Conductivité de la particule en mS.m².mol⁻¹
 *
 */

class DonneeParticule
{
public:
    DonneeParticule(int IDa);
    ~DonneeParticule();
    void calculerCouleurFond();

    void setTout(DonneeParticule change);

    QString getNom() const;
    void setNom(const QString &value);

    QColor getCouleurParticule() const;
    void setCouleurParticule(const QColor &value);

    int getPHValeur() const;
    void setPHValeur(int value);

    QString getFormule() const;
    void setFormule(const QString &value);

    double getConductivite() const;
    void setConductivite(double value);

    double getRouge() const;
    void setRouge(double value);

    double getVert() const;
    void setVert(double value);

    double getBleu() const;
    void setBleu(double value);

    int getID() const;
    void setID(int value);

    int getEtat() const;
    void setEtat(int value);

    double getIntensiteCouleur() const;

    QString getNomDeFichier() const;
    void setNomDeFichier(const QString &value);

    bool getPossedeUnFichier() const;
    void setPossedeUnFichier(bool value);

private:
    int ID;
    QString nom;
    QString formule;
    QColor couleurParticule;
    int etat;
    /*
     * Etat :
     * 0 : Aqueux
     * 1 : Solide
     * 2 : Liquide
     * 3 : Gazeux
     */

    //pH
    int pHValeur;

    //Absorbtion (Valeurs double -> Coefficients : c(mol.L⁻¹) * k = a)
    double rouge; //633 nm
    double vert; //528 nm
    double bleu; //470 nm
    //QColor couleurFond;
    double intensiteCouleur;

    //Conductimètrie
    double conductivite; //mS.m².mol⁻¹

    //Fichiers
    QString nomDeFichier;
    bool possedeUnFichier;
};

#endif // DONNEEPARTICULE_H
