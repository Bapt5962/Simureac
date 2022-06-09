#include "phmetre.h"

PHMetre::PHMetre(General *generalT, QWidget *parent) : Instrument(generalT, parent)
{
    objet = new QLabel(this);
    QPixmap img("data/assets/phmetre.png");
    img = img.scaled(img.width() / 3, img.height() / 3);
    objet->setPixmap(img);
    affichage = new QLCDNumber(this);
    affichage->setDigitCount(4);
    affichage->setSmallDecimalPoint(true);
    affichage->display("----");
    affichage->move(125, 200);
    affichage->setPalette(QPalette(QColor(0, 0, 0)));
    resize(img.width(), img.height());
}

PHMetre::~PHMetre()
{
    delete objet;
    delete affichage;
}

void PHMetre::loop(double valeur)
{
    valeurAffiche = valeur;
    valeur = qRound(valeur * 10) / 10.0;
    QString display(QString::number(valeur));
    if(display.indexOf('.') == -1) {
        display.append(".0");
    }
    else if(display.indexOf('.') < display.size() - 2) {
        display.resize(display.indexOf('.') + 2);
    }
    affichage->display(display);
}
