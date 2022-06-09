#ifndef DIALOGREACTIONS_H
#define DIALOGREACTIONS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include "general.h"
#include "listedialogedition.h"
#include "dialogeditionreaction.h"

/*
 * [DialogParticles]
 *
 * Fenêtre d'édition des réactions à simuler
 */

namespace Ui {
class DialogReactions;
}

class DialogReactions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReactions(General *generalT, QWidget *parent = nullptr);
    ~DialogReactions();

    void refaireListe();

public slots:
    void boutonClic(int ID);
    void on_Ajout_clicked();
    void donneeReactionRecu(DonneeReaction reaction);
    void renvoyerDonnees();
    void envoyerAnnuler();

signals:
    void sauvegarder(General);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::DialogReactions *ui;

    General *generalCopie;

    QWidget *contenuListe;
    QList<ListeDialogEdition*> reactions;

    DialogEditionReaction *dialogEdition;

    bool ignorerPremierResize; //Pour ignorer le premier resizeEvent() qui donne des valeurs incorrects
    int IDEdition; //ID en train d'être éditer
};

#endif // DIALOGREACTIONS_H
