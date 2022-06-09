#ifndef DIALOGREMPLISSAGE_H
#define DIALOGREMPLISSAGE_H

#include <QDialog>
#include <QVBoxLayout>
#include <QList>
#include "general.h"
#include "particuleajout.h"
#include "remplissage.h"

/*
 * [DialogParticles]
 *
 * Fenêtre de remplissage de la burette ou du becher
 */

namespace Ui {
class DialogRemplissage;
}

class DialogRemplissage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRemplissage(bool type, int ancienType, General *generalT, QWidget *parent = nullptr);
    ~DialogRemplissage();

public slots:
    void on_Maximum_currentIndexChanged(int index);
    void on_Ajouter_clicked();
    void renvoyerDonnees();
    void envoyerAnnuler();

signals:
    void remplir(Remplissage);
    void annuler();

private:
    Ui::DialogRemplissage *ui;
    QVBoxLayout *layout;

    QList<ParticuleAjout*> particules;

    General *general;

    bool typeVerrerie; //false = Burette ; true = Becher

    int maxFormat; //Valeur maximum formaté (Burette : 1 = 25 mL ; 2 = 50 mL ; Becher : 1 = 100 mL ; 2 = 200 mL ; 4 = 400 mL)
};

#endif // DIALOGREMPLISSAGE_H
