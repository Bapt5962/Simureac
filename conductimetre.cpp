#include "conductimetre.h"

Conductimetre::Conductimetre(General *generalT, QWidget *parent) : Instrument(generalT, parent)
{
    objet = new QLabel(this);
    normal = QPixmap("data/assets/conductimetreS.png");
    normal = normal.scaled(normal.width() / 3, normal.height() / 3);
    micro = QPixmap("data/assets/conductimetreU.png");
    micro = micro.scaled(micro.width() / 3, micro.height() / 3);
    mili = QPixmap("data/assets/conductimetreM.png");
    mili = mili.scaled(mili.width() / 3, mili.height() / 3);
    objet->setPixmap(micro);
    unite = 2;
    affichage = new QLCDNumber(this);
    affichage->setDigitCount(10);
    affichage->setSmallDecimalPoint(true);
    affichage->display("-----     ");
    affichage->move(100, 165);
    affichage->setPalette(QPalette(QColor(0, 0, 0)));
    resize(micro.width(), micro.height());
    loop(0);
}

Conductimetre::~Conductimetre()
{
    delete objet;
    delete affichage;
}

void Conductimetre::loop(double valeur)
{
    valeurAffiche = valeur;
    if(valeur >= 1)
    {
        if(valeur >= 1000)
        {
            affichage->display("Error     ");
            return;
        }
        else
        {
            if(unite != 0)
            {
                objet->setPixmap(normal);
                unite = 0;
            }
        }
    }
    else
    {
        valeur *= 1000;
        if(valeur < 1)
        {
            valeur *= 1000;
            if(unite != 2)
            {
                objet->setPixmap(micro);
                unite = 2;
            }
        }
        else
        {
            if(unite != 1)
            {
                objet->setPixmap(mili);
                unite = 1;
            }
        }
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
    affichage->display(display + "     ");
}
