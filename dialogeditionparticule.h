#ifndef DIALOGEDITIONPARTICULE_H
#define DIALOGEDITIONPARTICULE_H

#include <QDialog>
#include <QColorDialog>
#include "donneeparticule.h"

/*
 * [DialogEditionParticles]
 *
 * Fenêtre d'édition d'une particule à simuler
 */

namespace Ui {
class DialogEditionParticule;
}

class DialogEditionParticule : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditionParticule(DonneeParticule copieParticule, QWidget *parent = nullptr);
    ~DialogEditionParticule();

public slots:
    void on_Conductivite_clicked();
    void on_Absorbance_clicked();
    void on_Couleur_clicked();
    void renvoyerDonnees();

signals:
    void sauvegarder(DonneeParticule);

private:
    Ui::DialogEditionParticule *ui;

    DonneeParticule copie;
};

#endif // DIALOGEDITIONPARTICULE_H
