#ifndef SIMULATION_H
#define SIMULATION_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QApplication>
#include "general.h"
#include "particule.h"
#include "remplissage.h"
#include "particuleliste.h"
#include <QtMath>
#include "thermometre.h"
#include "phmetre.h"
#include "conductimetre.h"
#include "chronometre.h"
#include "colorimetre.h"
#include "ordinateur.h"

/*
 * [Simulation]
 *
 * Partie de gauche de MainWindow contenant la simulation
 * Essaye de tourner à 60 FPS
 *
 */

class Simulation : public QWidget
{
    Q_OBJECT
public:
    Simulation(QList<ParticuleListe*> *particulesListeT, General *generalT, QWidget *parent = nullptr);
    ~Simulation();
    void calculPH(double H3O = 0, double HO = 0);
    //double calculCoeffAleatoire(double p, double t);
    void refaireListeParticule();
    void nettoyer();

    //Instruments

    void utiliserChronometre(bool checked);
    void utiliserThermometre(bool checked);
    void utiliserPHMetre(bool checked);
    void utiliserConductimetre(bool checked);
    void utiliserColorimetre(bool checked);
    void utiliserOrdinateur(bool checked);

    //Set et Get pour les variables

    double getVolumeBecher() const;
    void setVolumeBecher(double value);

    double getVolumeBurette() const;
    void setVolumeBurette(double value);

    int getTypeBecher() const;
    void setTypeBecher(int value);

    int getTypeBurette() const;
    void setTypeBurette(int value);

    bool getPaused() const;
    void setPaused(bool value, bool bouton = false);

    void setConcentration(bool value);

    double getTemperatureBecher() const;
    void setTemperatureBecher(double value);

    QColor getCouleurArrierePlan() const;
    void setCouleurArrierePlan(const QColor &value);

    bool getFpsAEnvoyer() const;
    void setFpsAEnvoyer(bool value);

public slots:
    void loop(); //Fonction principale, appelée 1000 fois par secondes mais le bloc principal est exécuté à une fréquence dépendant du nombre de FPS (Par défaut : 60 fois par secondes)
    void donneeRemplissageRecu(Remplissage remplir);
    void donneeAnnuler();

signals:
    void chronoStart();
    void chronoReset();
    void chronoMode();
    void pause();

    void lagging(QString lag); //lag = message à afficher

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    General *general;
    QList<ParticuleListe*> *particulesListe;
    bool concentration; //Vis-à-vis de la liste de particules à droite de MainWindow

    //Pause
    bool paused;
    bool pauseBouton;
    bool pauseDialog;

    //Timers et FPS
    int compteFps; //Augmente de 1 à chaque frame, remis à 0 chaque secondes
    int fpsActuel; //compteFPS avant sa remise à 0
    int fpsCible;
    bool fpsAEnvoyer;

    double dernierElapsed; //Utile dans la détection des lag spikes

    int rateFps; //Nombre de fois d'affilée où (fpsActuel) < (fpsCible) / 2
    bool messageFps; //Le message de ralentissement est t-il affichée ?

    QElapsedTimer timerGlobal; //Compte le temps de simulation, pauses comprises
    QElapsedTimer timerPause; //Compte le temps que la simulation a été en pause
    long totalTempsPause; //Temps total passé en pause
    QElapsedTimer timerFrame; //Remis à 0 à chaque frame
    QElapsedTimer timerSecondes; //Remis à 0 toutes les secondes

    //Images de la simulations
    QLabel *becher;
    QLabel *burette;

    //Interactions burette-becher
    QSlider *debitBurette;

    //Quantités de liquides (en mL)
    double volumeBecher;
    double volumeBurette;
    double volumeInitialBurette;
    int typeBecher; //Type 1 : 100 mL ; Type 2 : 200 mL ; Type 4 : 400 mL
    int typeBurette; //Type 1 : 25 mL ; Type 2 : 50 mL

    //Particules
    QList<Particule*> particules;
    int nombreDansBurette;
    int nombreDansBecher;
    QList<int> degagementGazeux; //Nombre de particules gazeuses sortie du becher pour chaque type (y compris particules non gazeuses)

    //Couleurs
    QColor couleurArrierePlan;
    QColor couleurBecher;
    QColor couleurBurette;

    //Quadrillage de concentrations (pour éviter que toutes les particules se concentre au même endroit)
    int maxX;
    int maxY;
    int maxXmini;
    int maxYmini;
    QList<unsigned int> concentrationBecher[20][19]; //[x][y][typeParticule]
    QList<QPoint> concentrationMaxBecher; //[typeParticule]
    QVector<QVector<Particule*>> concentrationBecherParticule[15][15]; //[x][y][typeParticule][particule]
    QVector<int> concentrationBecherParticuleTaille[15][15]; //[x][y][typeParticule]
    bool concentrationBecherParticuleInitialise;
    QList<unsigned int> concentrationBurette[73]; //[y][typeParticule]
    QList<int> presenceSolide[172][163]; //[x][y][typeParticule]

    //Attirances entre réactifs
    void refaireReactifsAttires();
    QList<int> reactionsPossibles; //[Réaction]
    QList<int> reactionsPossiblesPrecedent; //reactionsPossibles lors de la frame précédente
    QList<QList<int>> reactifsAttires; //[typeParticule] attiré par [n]
    QList<int> coefficientAttiranceReactif; //[typeParticule]
    QList<int> coefficientAttiranceReactifCinetique; //[typeParticule]
    QList<bool> stopReactif[15][15]; //[x][y][typeParticule]

    double mouvementConcentration(int n); //(|n| < 1) -> 0 ; (0 < |n| < 22) -> (0.2 + (|n| - 1) / 50) * (|n| / n) ; (|n| > 21) -> 0.6 * (|n| / n)
    double vibrationConcentration(unsigned int n); //(n < 22) -> 1 + (n - 1) * 0.1 ; (n > 21) -> 3

    //Temps
    long frames; //1 frame = 1/50 secondes
    bool chronometrePresent;
    Chronometre *chronometre;

    //Température
    double temperatureBecher;
    double temperatureBurette;
    bool thermometrePresent;
    Thermometre *thermometre;

    //pH
    double pH;
    bool pHMetrePresent;
    PHMetre *pHMetre;

    //Conductivité
    bool conductimetrePresent;
    Conductimetre *conductimetre;

    //Absorbance
    bool colorimetrePresent;
    Colorimetre *colorimetre;

    //Ordinateur
    bool ordinateurPresent;
    QLabel *ecran;
    Ordinateur *ordinateur;
    double obtenirValeurPourOrdinateur(int type);
};

#endif // SIMULATION_H
