#include "dialogpriseenmain.h"
#include "ui_dialogpriseenmain.h"

DialogPriseEnMain::DialogPriseEnMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogPriseEnMain)
{
    ui->setupUi(this);
    load();
    indexAide = 0;
    changerAide();
}

DialogPriseEnMain::~DialogPriseEnMain()
{
    delete ui;
}

void DialogPriseEnMain::changerAide()
{
    ui->Precedent->setDisabled(indexAide == 0);
    ui->Suivant->setDisabled(indexAide == titres.size() - 1);
    ui->Titre->setText(titres[indexAide]);
    ui->Explication->setPlainText(descriptions[indexAide]);
    ui->Pages->setText(QString::number(indexAide + 1) + "/" + QString::number(titres.size()));
}

void DialogPriseEnMain::on_Precedent_clicked()
{
    indexAide--;
    changerAide();
}

void DialogPriseEnMain::on_Suivant_clicked()
{
    indexAide++;
    changerAide();
}

void DialogPriseEnMain::on_Quitter_clicked()
{
    close();
}

void DialogPriseEnMain::load()
{
    FileText chargement;
    if(chargement.load("data/assets/aide/priseEnMain.txt"))
    {
        QString titre;
        QString description;
        int lecture(0);
        while (lecture < chargement.size()) {
            titre = chargement.read(lecture);
            lecture++;
            description = "";
            while (chargement.read(lecture) != "<NEXT>") {
                description += chargement.read(lecture) + "\n";
                lecture++;
            }
            lecture++;
            titres.append(titre);
            descriptions.append(description);
        }
    }
}
