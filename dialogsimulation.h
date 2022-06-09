#ifndef DIALOGSIMULATION_H
#define DIALOGSIMULATION_H

#include <QDialog>
#include <QMessageBox>
#include "general.h"
#include "dialogparticules.h"
#include "dialogreactions.h"

/*
 * [Dialog Simulation]
 *
 * Fenêtre principale d'édition des caractèristique de la simulation :
 *
 * Contient :
 * Le bouton d'activation du pH (voir Général)
 * Le bouton d'accès à la fenêtre d'édition des éléments (voir _)
 * Le bouton d'accès à la fenêtre d'édition des intéractions (voir _)
 */

namespace Ui {
class DialogSimulation;
}

class DialogSimulation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSimulation(General *generalT, QWidget *parent = nullptr);
    ~DialogSimulation();

public slots:
    void on_pH_clicked();
    void on_Particules_clicked();
    void on_Interactions_clicked();
    void renvoyerDonnees();
    void donneeRecu(General simulation);
    void envoyerAnnuler();

signals:
    void sauvegarder(General);
    void annuler();

private:
    Ui::DialogSimulation *ui;

    General *generalCopie;

    DialogParticules *dialogParticules;

    DialogReactions *dialogReactions;
};

#endif // DIALOGSIMULATION_H
