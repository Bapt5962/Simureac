#ifndef DIALOGSAUVEGARDESIMULATION_H
#define DIALOGSAUVEGARDESIMULATION_H

#include <QDialog>
#include <QMessageBox>
#include "general.h"
#include "filetext.h"

/*
 * [Dialog Sauvegarde Particule]
 *
 * Fenêtre de sauvegarde d'une simulation
 *
 * Structure d'un fichier Simulation
 *
 * String   : Nom
 * String   : Description
 * "ENDOFTHEDESCRIPTION"
 * DonneeParticule[]  (Fichiers externes : String)
 * "ENDPARTICLES"
 * DonneeReaction[]   (Intégrés : DonneeReaction)
 * "ENDREACTIONS"
 *
 * Structure d'une DonneeReaction
 *
 * Int[]    : Proportion_Reactif
 * "ENDLIST"
 * Int[]    : ID_Reactif
 * "ENDLIST"
 * Int[]    : Proportion_Produit
 * "ENDLIST"
 * Int[]    : ID_Produit
 * "ENDLIST"
 * Double[] : Efficacite_Catalyseur
 * "ENDLIST"
 * Int[]    : ID_Catalyseur
 * "ENDLIST"
 * Double   : A_Cinetique
 * Double   : Ea_Cinetique
 * Double   : Delta_Temperature
 * Double   : PH_Min
 * Double   : PH_Max
 * "ENDREACTION"
 *
 */

namespace Ui {
class DialogSauvegardeSimulation;
}

class DialogSauvegardeSimulation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSauvegardeSimulation(General* generalU, QWidget *parent = nullptr);
    ~DialogSauvegardeSimulation();

public slots:
    void on_Sauvegarder_clicked();
    void on_Annuler_clicked();
    void on_NomFichier_textChanged();

signals:
    void dialogQuit(int);

private:
    Ui::DialogSauvegardeSimulation *ui;

    General* general;
};

#endif // DIALOGSAUVEGARDESIMULATION_H
