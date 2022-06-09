#include "dialogremplissage.h"
#include "ui_dialogremplissage.h"

DialogRemplissage::DialogRemplissage(bool type, int ancienType, General *generalT, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRemplissage)
{
    ui->setupUi(this);
    layout = new QVBoxLayout;
    ui->Elements->setLayout(layout);
    typeVerrerie = type;
    general = generalT;

    if(typeVerrerie)
    {
        ui->Maximum->addItem("100 mL");
        ui->Maximum->addItem("200 mL");
        ui->Maximum->addItem("400 mL");
        setWindowTitle("Remplir le becher");
    }
    else
    {
        ui->Maximum->addItem("25 mL");
        ui->Maximum->addItem("50 mL");
        ui->Temperature->setMinimum(0);
        setWindowTitle("Remplir la burette");
    }

    if(ancienType == 4)
    {
        ui->Maximum->setCurrentIndex(2);
    }
    else
    {
        ui->Maximum->setCurrentIndex(ancienType - 1);
    }

    connect(this, SIGNAL(accepted()), this, SLOT(renvoyerDonnees()));
    connect(this, SIGNAL(rejected()), this, SLOT(envoyerAnnuler()));

    this->setAttribute(Qt::WA_DeleteOnClose); //Appelle le destructeur quand la croix de la fenêtre est pressé

    on_Maximum_currentIndexChanged(ui->Maximum->currentIndex());

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogRemplissage::~DialogRemplissage()
{
    qDeleteAll(particules);
    delete layout;
    delete ui;
}

void DialogRemplissage::on_Maximum_currentIndexChanged(int index)
{
    maxFormat = index + 1;
    if(maxFormat == 3)
    {
        maxFormat = 4;
    }
    if(typeVerrerie)
    {
        ui->Remplir->setMaximum(100 * maxFormat);
    }
    else
    {
        ui->Remplir->setMaximum(25 * maxFormat);
    }
}

void DialogRemplissage::on_Ajouter_clicked()
{
    ParticuleAjout* nouveau(new ParticuleAjout(general, !typeVerrerie));
    particules.append(nouveau);
    layout->addWidget(nouveau);
}

void DialogRemplissage::renvoyerDonnees()
{
    Remplissage renvoi;
    renvoi.setVerrerie(typeVerrerie);
    renvoi.setTypeFormat(maxFormat);
    renvoi.setRemplir(ui->Remplir->value());
    renvoi.setTemperature(ui->Temperature->value());
    QList<int> types;
    QList<double> concentrations;
    for (int IDp(0); IDp < particules.size(); IDp++) {
        types.append(particules[IDp]->getIndexParticule());
        concentrations.append(particules[IDp]->getConcentration());
    }
    renvoi.setElements(types);
    renvoi.setConcentrations(concentrations);
    emit remplir(renvoi);
}

void DialogRemplissage::envoyerAnnuler()
{
    emit annuler();
}
