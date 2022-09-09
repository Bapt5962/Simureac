#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QInputDialog>
#include <QColorDialog>
#include <QDesktopServices>
#include "simulation.h"
#include "general.h"
#include "dialogsimulation.h"
#include "dialogremplissage.h"
#include "particuleliste.h"
#include "filetext.h"
#include "dialogsauvegardeparticule.h"
#include "dialogimporterparticule.h"
#include "dialogsauvegardesimulation.h"
#include "dialogchargersimulation.h"
#include "dialogaide.h"
#include "dialogapropos.h"

#include <QDebug>

/*
 * [MainWindow]
 *
 * Fenêtre principal
 *
 * Contient :
 * La barre d'action en haut
 * La simulation (voir Simulation)
 * La liste des éléments (voir ParticuleListe)
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refaireListe();
    void tailleListe();

public slots:
    //Fichier
    void on_actionChargerSimulation_triggered();
    void on_actionImporterParticule_triggered();
    void on_actionSauvegarderParticule_triggered();
    void on_actionSauvegarderSimulation_triggered();
    //Edition
    void on_actionEditer_triggered();
    void on_actionFermer_triggered();
    //Instruments
    void on_actionChronometre_triggered(bool checked);
    void on_actionThermometre_triggered(bool checked);
    void on_actionPHMetre_triggered(bool checked);
    void on_actionConductimetre_triggered(bool checked);
    void on_actionColorimetre_triggered(bool checked);
    void on_actionOrdinateur_triggered(bool checked);
    //Affichage
    void on_actionSombre_triggered(bool checked);
    void on_actionFond_triggered();
    void on_actionCompteurFPS_triggered(bool checked);
    void on_actionPleinEcran_triggered(bool checked);
    //Aide
    void on_actionAide_triggered();
    void on_actionAPropos_triggered();
    void on_actionDonnations_triggered();
    //Dialog sans retour à MainWindow
    void dialogQuit(int type, General* nouveau = nullptr); //0 = Sauvegarde Particule ; 1 = Importer Particule ; 2 = Sauvegarde Simulation ; 3 = Importer Simulation ; 4 = Aide ; 5 = A propos
    //Fenêtre d'édition de simulation
    void donneeSimulationRecu(General simulationGet);
    void donneeSimulationAnnuler();
    //Boutons en dessous de la liste de particules
    void on_Pause_toggled(bool toggled);
    void on_Burette_clicked();
    void on_Becher_clicked();
    void on_Eau_clicked();
    void on_Temperature_clicked();
    //Boutons au dessus de la liste de particules
    void on_Concentration_toggled(bool toggled);
    void on_Formule_toggled(bool toggled);
    //Autre
    void lagging(QString lag);
    void mettreEnPause();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QLabel *messageStatusBar;

    General *general;
    Simulation *simulation;

    FileText preferences;

    QWidget* particulesConteneur;
    QList<ParticuleListe*> *particules;

    DialogSimulation *dialogSimulation;
    DialogRemplissage *dialogRemplissage;

    DialogSauvegardeParticule *dialogSauvegardeParticule;
    DialogImporterParticule *dialogImporterParticule;
    DialogSauvegardeSimulation *dialogSauvegardeSimulation;
    DialogChargerSimulation *dialogChargerSimulation;

    DialogAide *dialogAide;
    DialogAPropos *dialogAPropos;
};
#endif // MAINWINDOW_H
