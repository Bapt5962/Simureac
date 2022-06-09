#include "particuleliste.h"
#include "ui_particuleliste.h"

ParticuleListe::ParticuleListe(int y, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParticuleListe)
{
    ui->setupUi(this);

    couleur = QColor(0, 0, 0);

    move(0, y);
}

ParticuleListe::~ParticuleListe()
{
    delete ui;
}

void ParticuleListe::setCouleur(QColor fond)
{
    couleur = fond;
}

void ParticuleListe::setFormule(QString formule)
{
    ui->Formule->setText(formule);
}

void ParticuleListe::setValeur(QString valeur)
{
    ui->Valeur->setText(valeur);
}

bool ParticuleListe::getVisible()
{
    return ui->Visible->isChecked();
}

void ParticuleListe::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter dessin;
    dessin.begin(this);
    dessin.eraseRect(0, 0, 1000, 1000); //Efface tout

    dessin.fillRect(0, 0, width(), height(), couleur);
}
