#ifndef LISTEPARTICULEREACTION_H
#define LISTEPARTICULEREACTION_H

#include <QWidget>
#include "donneeparticule.h"

/*
 * [ListeParticuleReaction]
 *
 * Un élement d'une liste de DialogEditionReaction
 *
 */

namespace Ui {
class ListeParticuleReaction;
}

class ListeParticuleReaction : public QWidget
{
    Q_OBJECT

public:
    explicit ListeParticuleReaction(bool catalyseurA, QList<DonneeParticule*> liste, QWidget *parent = nullptr);
    ~ListeParticuleReaction();

    bool getASupprimer() const;

    void setValeurs(double nombre, int ID);
    double getNombre() const;
    int getID() const;

public slots:
    void on_Supprimer_clicked();

signals:
    void supprime();

private:
    Ui::ListeParticuleReaction *ui;
    bool catalyseur;
    bool aSupprimer;
};

#endif // LISTEPARTICULEREACTION_H
