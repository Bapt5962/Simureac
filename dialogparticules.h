#ifndef DIALOGPARTICULES_H
#define DIALOGPARTICULES_H

#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include "general.h"
#include "listedialogedition.h"
#include "dialogeditionparticule.h"

/*
 * [DialogParticles]
 *
 * Fenêtre d'édition des particules à simuler
 */

namespace Ui {
class DialogParticules;
}

class DialogParticules : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParticules(General *generalT, QWidget *parent = nullptr);
    ~DialogParticules();

    void refaireListe();

public slots:
    void boutonClic(int ID);
    void on_Ajout_clicked();
    void donneeParticuleRecu(DonneeParticule particule);
    void renvoyerDonnees();
    void envoyerAnnuler();

signals:
    void sauvegarder(General);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::DialogParticules *ui;

    General *generalCopie;

    QWidget *contenuListe;
    QList<ListeDialogEdition*> particules;

    DialogEditionParticule *dialogEdition;

    bool ignorerPremierResize; //Pour ignorer le premier resizeEvent() qui donne des valeurs incorrects
    int IDEdition; //ID en train d'être éditer
};

#endif // DIALOGPARTICULES_H
