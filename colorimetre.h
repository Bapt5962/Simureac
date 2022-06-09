#ifndef COLORIMETRE_H
#define COLORIMETRE_H

#include "instrument.h"
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>

/*
 * [Colorimètre]
 *
 * Hérite Instrument
 * valeurAffiche : Absorbance mesurée pour la couleur choisie
 */

class Colorimetre : public Instrument
{
    Q_OBJECT
public:
    Colorimetre(General *generalT, QWidget *parent = nullptr);
    ~Colorimetre();
    void afficherValeur();

    void loop(double rouge, double vert, double bleu);
    //Valeur : Absorbances

public slots:
    void rougeClicked();
    void vertClicked();
    void bleuClicked();
    void modeClicked();
    void bloquerBoutons(bool bloquer); //Durant une assimilation par l'ordinateur

private:
    QLabel *objet;
    QLCDNumber *affichage;

    double absorbances[3];

    bool modeAbsorbance;
    QPixmap absorbanceImg;
    QPixmap transmitanceImg;

    int couleurUtilise;
    QPushButton *rougeBouton;
    QPushButton *vertBouton;
    QPushButton *bleuBouton;
    QPushButton *mode;
};

#endif // COLORIMETRE_H
