#ifndef DIALOGEDITIONREACTION_H
#define DIALOGEDITIONREACTION_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QList>
#include <QScrollArea>
#include "donneeparticule.h"
#include "donneereaction.h"
#include "listeparticulereaction.h"

/*
 * [DialogEditionReaction]
 *
 * Fenêtre d'édition d'une réaction à simuler
 */

namespace Ui {
class DialogEditionReaction;
}

class DialogEditionReaction : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditionReaction(DonneeReaction copieReaction, QList<DonneeParticule*> particulesListe, QWidget *parent = nullptr);
    ~DialogEditionReaction();

public slots:
    void ajouterParticuleClic();
    void supprimerParticuleClic();
    void on_Temperature_clicked();
    void on_pH_clicked();
    void renvoyerDonnees();
    void refaireListe();

signals:
    void sauvegarder(DonneeReaction);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::DialogEditionReaction *ui;

    bool enSuppression; //Ne pas refaire la liste si la fenêtre se ferme

    DonneeReaction copie;
    QList<DonneeParticule*> particules;

    QVBoxLayout* layoutReaction;
    QTabWidget* tabs;
    QPushButton* ajouterParticule;

    QScrollArea* tabReactif;
    QWidget *sousTabReactif;
    QList<ListeParticuleReaction*> listeReactifs;
    QScrollArea* tabProduit;
    QWidget *sousTabProduit;
    QList<ListeParticuleReaction*> listeProduits;
    QScrollArea* tabCatalyseur;
    QWidget *sousTabCatalyseur;
    QList<ListeParticuleReaction*> listeCatalyseurs;
};

#endif // DIALOGEDITIONREACTION_H
