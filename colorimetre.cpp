#include "colorimetre.h"

Colorimetre::Colorimetre(General *generalT, QWidget *parent) : Instrument(generalT, parent)
{
    objet = new QLabel(this);
    absorbanceImg = QPixmap("data/assets/colorimetreA.png");
    absorbanceImg = absorbanceImg.scaled(absorbanceImg.width() / 3, absorbanceImg.height() / 3);
    transmitanceImg = QPixmap("data/assets/colorimetreT.png");
    transmitanceImg = transmitanceImg.scaled(transmitanceImg.width() / 3, transmitanceImg.height() / 3);
    objet->setPixmap(absorbanceImg);
    modeAbsorbance = true;
    couleurUtilise = 0;
    affichage = new QLCDNumber(this);
    affichage->setDigitCount(8);
    affichage->setSmallDecimalPoint(true);
    affichage->display("--------");
    affichage->move(180, 67);
    affichage->setPalette(QPalette(QColor(0, 0, 0)));
    resize(absorbanceImg.width(), absorbanceImg.height());

    rougeBouton = new QPushButton("Rouge (633 nm)", this);
    rougeBouton->move(10, 56);
    rougeBouton->setFixedSize(110, 23);
    connect(rougeBouton, SIGNAL(clicked()), this, SLOT(rougeClicked()));
    vertBouton = new QPushButton("Vert (528 nm)", this);
    vertBouton->move(10, 78);
    vertBouton->setFixedSize(110, 23);
    connect(vertBouton, SIGNAL(clicked()), this, SLOT(vertClicked()));
    bleuBouton = new QPushButton("Bleu (470 nm)", this);
    bleuBouton->move(10, 100);
    bleuBouton->setFixedSize(110, 23);
    connect(bleuBouton, SIGNAL(clicked()), this, SLOT(bleuClicked()));
    mode = new QPushButton("Mode", this);
    mode->move(10, 133);
    mode->setFixedSize(110, 23);
    connect(mode, SIGNAL(clicked()), this, SLOT(modeClicked()));
}

Colorimetre::~Colorimetre()
{
    delete objet;
    delete affichage;
}

void Colorimetre::afficherValeur()
{
    valeurAffiche = absorbances[couleurUtilise];
    double valeurUtilise;
    if(!modeAbsorbance)
    {
        valeurUtilise = pow(10, 2 - absorbances[couleurUtilise]);
    }
    else
    {
        valeurUtilise = absorbances[couleurUtilise];
    }
    QString display(QString::number(valeurUtilise));
    if(display.contains('e'))
    {
        if(valeurUtilise < 1)
        {
            if(!modeAbsorbance)
            {
                affichage->display("0.000 ");
            }
            else
            {
                affichage->display("0.0000");
            }
        }
        else
        {
            if(!modeAbsorbance)
            {
                affichage->display("Error ");
            }
            else
            {
                affichage->display("Error");
            }
        }
        return;
    }
    if(!modeAbsorbance)
    {
        if(display.indexOf('.') == -1) {
            display.append(".000");
        }
        else if(display.indexOf('.') == display.size() - 2) {
            display.append("00");
        }
        else if(display.indexOf('.') == display.size() - 3) {
            display.append("0");
        }
        else if(display.indexOf('.') < display.size() - 4) {
            display.resize(display.indexOf('.') + 4);
        }
        if(display.size() > 8)
        {
            display = "Error";
        }
        display += " ";
    }
    else
    {
        if(display.indexOf('.') == -1) {
            display.append(".0000");
        }
        else if(display.indexOf('.') == display.size() - 2) {
            display.append("000");
        }
        else if(display.indexOf('.') == display.size() - 3) {
            display.append("00");
        }
        else if(display.indexOf('.') == display.size() - 4) {
            display.append("0");
        }
        else if(display.indexOf('.') < display.size() - 5) {
            display.resize(display.indexOf('.') + 5);
        }
        if(display.size() > 8)
        {
            display = "Error";
        }
    }
    affichage->display(display);
}

void Colorimetre::loop(double rouge, double vert, double bleu)
{
    absorbances[0] = rouge;
    absorbances[1] = vert;
    absorbances[2] = bleu;
    afficherValeur();
}

void Colorimetre::rougeClicked()
{
    couleurUtilise = 0;
}

void Colorimetre::vertClicked()
{
    couleurUtilise = 1;
}

void Colorimetre::bleuClicked()
{
    couleurUtilise = 2;
}

void Colorimetre::modeClicked()
{
    modeAbsorbance = !modeAbsorbance;
    if(modeAbsorbance)
    {
        objet->setPixmap(absorbanceImg);
    }
    else
    {
        objet->setPixmap(transmitanceImg);
    }
}

void Colorimetre::bloquerBoutons(bool bloquer)
{
    rougeBouton->setDisabled(bloquer);
    vertBouton->setDisabled(bloquer);
    bleuBouton->setDisabled(bloquer);
}
