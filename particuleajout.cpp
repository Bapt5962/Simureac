#include "particuleajout.h"
#include "ui_particuleajout.h"

ParticuleAjout::ParticuleAjout(General *generalT, bool buretteT, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParticuleAjout)
{
    ui->setupUi(this);
    general = generalT;
    burette = buretteT;
    for (int IDp(0); IDp < general->nombreParticule(); IDp++) {
        ui->Particule->addItem(general->getParticule(IDp)->getNom());
    }
}

ParticuleAjout::~ParticuleAjout()
{
    delete ui;
}

int ParticuleAjout::getIndexParticule()
{
    return ui->Particule->currentIndex();
}

double ParticuleAjout::getConcentration()
{
    return ui->Concentration->value();
}

void ParticuleAjout::on_Particule_currentIndexChanged(int index)
{
    if(burette)
    {
        if(general->getParticule(index)->getEtat() == 1 || general->getParticule(index)->getEtat() == 3)
        {
            ui->Concentration->setValue(0);
            ui->Concentration->setEnabled(false);
        }
        else
        {
            ui->Concentration->setEnabled(true);
        }
    }
}
