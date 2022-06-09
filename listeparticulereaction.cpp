#include "listeparticulereaction.h"
#include "ui_listeparticulereaction.h"

ListeParticuleReaction::ListeParticuleReaction(bool catalyseurA, QList<DonneeParticule*> liste, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListeParticuleReaction)
{
    ui->setupUi(this);
    aSupprimer = false;
    catalyseur = catalyseurA;
    if(catalyseur)
    {
        ui->Nombre->setMinimum(-1000000000000000000);
        ui->Nombre->setMaximum(1000000000000000000);
        ui->Nombre->setDecimals(18);
        ui->Nombre->setValue(0);
    }
    else
    {
        ui->Nombre->setMinimum(1);
        ui->Nombre->setMaximum(1000);
        ui->Nombre->setDecimals(0);
        ui->Nombre->setValue(1);
    }
    for (int p(0); p < liste.size(); p++) {
        ui->Particule->addItem(liste[p]->getNom());
    }
}

ListeParticuleReaction::~ListeParticuleReaction()
{
    delete ui;
}

void ListeParticuleReaction::on_Supprimer_clicked()
{
    aSupprimer = true;
    emit supprime();
}

bool ListeParticuleReaction::getASupprimer() const
{
    return aSupprimer;
}

void ListeParticuleReaction::setValeurs(double nombre, int ID)
{
    ui->Nombre->setValue(nombre);
    ui->Particule->setCurrentIndex(ID);
}

double ListeParticuleReaction::getNombre() const
{
    return ui->Nombre->value();
}

int ListeParticuleReaction::getID() const
{
    return ui->Particule->currentIndex();
}
