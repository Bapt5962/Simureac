#ifndef ORDINATEUR_H
#define ORDINATEUR_H

#include <QWidget>
#include <QList>
#include <QPointF>
#include <QPainter>
#include "ordinateurwidgetminmax.h"
#include <QtMath>
#include <QToolTip>
#include <QCursor>
#include <QTimer>

/*
 * [Ordinateur]
 *
 * Assimile des données de classes héritant d'Instrument
 * Affiche les données obtenus sur un graphique
 *
 * Grandeurs :
 *
 * 0 : Aucune
 * 1 : t (en s)
 * 2 : V (en mL)
 * 3 : Θ (en °C)
 * 4 : Θ (en °K)
 * 5 : pH
 * 6 : [H30+] (en mol.L⁻¹)
 * 7 : [HO-] (en mol.L⁻¹)
 * 8 : σ (en S.cm⁻¹)
 * 9 : A
 * 10: T (en %)
 *
 */

namespace Ui {
class Ordinateur;
}

class Ordinateur : public QWidget
{
    Q_OBJECT

public:
    explicit Ordinateur(QWidget *parent = nullptr);
    ~Ordinateur();

    void loop(double temps, double x = 0, double yG = 0, double yD = 0);
    int getGrandeurX() const;
    int getGrandeurYG() const;
    int getGrandeurYD() const;

    bool getEnAssimilation() const;

    double interpolation(double valeur, double min, double max);
    double inverseInterpolation(double valeur, double min, double max);

public slots:
    void on_Reset_clicked();
    void on_Assimiler_clicked();
    void on_Abscisse_clicked();
    void on_OrdonneeG_clicked();
    void on_OrdonneeD_clicked();
    void on_MinMax_clicked();
    void changementMinMax();
    void toolTipLoop();

signals:
    void assimiliationClic(bool);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::Ordinateur *ui;

    bool limitesAffiche;
    OrdinateurWidgetMinMax* limites;

    int grandeurX;
    int grandeurYG;
    int grandeurYD;
    QString affichage[11]; //Constantes, générées dans le constructeur
    QString unites[11]; //Constantes, générées dans le constructeur

    QPoint dernierePositionCurseur;
    int curseurFixe;

    bool enAssimilation;

    double tempsDepart;
    double tempsDernierPoint;

    bool echelleContrainteMin;
    bool echelleContrainteMax;
    double minX;
    double maxX;
    double minYG;
    double maxYG;
    double minYD;
    double maxYD;

    QList<QList<QPointF>> graphiques[2];
    QList<QPointF> ajouterPointsCoordonnees;
    QList<bool> ajouterPointsAxe; //true = gauche : false = droite
    QList<int> ajouterPointsCouleursID; //"Pointe" vers ajouterPointsCouleurs
    QList<QString> ajouterPointsCouleurs; //Constante, générée dans le constructeur
    int idCouleur[2];
};

#endif // ORDINATEUR_H
