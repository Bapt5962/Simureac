#ifndef DIALOGSAUVEGARDEPARTICULE_H
#define DIALOGSAUVEGARDEPARTICULE_H

#include <QDialog>
#include <QMessageBox>
#include "general.h"
#include "filetext.h"

/*
 * [Dialog Sauvegarder Particule]
 *
 * Fenêtre de sauvegarde d'une particule
 *
 * Structure d'un fichier Particule
 *
 * Int      : ID
 * String   : Nom
 * String   : Formule
 * Int      : P_Rouge
 * Int      : P_Vert
 * Int      : P_Bleu
 * Int      : Etat
 * Int      : PH
 * Double   : A_Rouge
 * Double   : A_Vert
 * Double   : A_Bleu
 * Double   : Conductivite
 *
 */

namespace Ui {
class DialogSauvegardeParticule;
}

class DialogSauvegardeParticule : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSauvegardeParticule(General* generalU, QWidget *parent = nullptr);
    ~DialogSauvegardeParticule();

public slots:
    void on_Sauvegarder_clicked();
    void on_Annuler_clicked();
    void on_Choix_currentIndexChanged(int index);
    void on_Nom_textChanged();

signals:
    void dialogQuit(int);

private:
    Ui::DialogSauvegardeParticule *ui;

    General* general;
};

#endif // DIALOGSAUVEGARDEPARTICULE_H
