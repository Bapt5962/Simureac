#ifndef PARTICULEAJOUT_H
#define PARTICULEAJOUT_H

#include <QWidget>
#include "general.h"

/*
 * [ParticuleAjout]
 *
 * Un élément de la liste de DialogRemplissage
 */

namespace Ui {
class ParticuleAjout;
}

class ParticuleAjout : public QWidget
{
    Q_OBJECT

public:
    explicit ParticuleAjout(General *generalT, bool buretteT, QWidget *parent = nullptr);
    ~ParticuleAjout();

    int getIndexParticule();
    double getConcentration();

public slots:
    void on_Particule_currentIndexChanged(int index);

private:
    Ui::ParticuleAjout *ui;

    General* general;

    bool burette;
};

#endif // PARTICULEAJOUT_H
