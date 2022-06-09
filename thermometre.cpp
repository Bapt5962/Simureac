#include "thermometre.h"

Thermometre::Thermometre(General *generalT, QWidget *parent) : Instrument(generalT, parent)
{
    objet = new QLabel(this);
    QPixmap img("data/assets/thermometre.png");
    img = img.scaled(img.width() / 3, img.height() / 3);
    objet->setPixmap(img);
    affichage = new QLCDNumber(this);
    affichage->setDigitCount(8);
    affichage->setSmallDecimalPoint(true);
    affichage->display("--------");
    affichage->move(10, 20);
    affichage->setPalette(QPalette(QColor(0, 0, 0)));
    resize(img.width(), img.height());
}

Thermometre::~Thermometre()
{
    delete objet;
    delete affichage;
}

void Thermometre::loop(double valeur)
{
    valeurAffiche = valeur;
    valeur = qRound(valeur * 100) / 100.0;
    if(valeur >= 10000)
    {
        affichage->display("Error");
        return;
    }
    QString display(QString::number(valeur));
    if(display.indexOf('.') == -1) {
        display.append(".00");
    }
    else if(display.indexOf('.') == display.size() - 2) {
        display.append("0");
    }
    else if(display.indexOf('.') < display.size() - 3) {
        display.resize(display.indexOf('.') + 3);
    }
    affichage->display(display + "'C");
}
