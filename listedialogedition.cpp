#include "listedialogedition.h"
#include "ui_listedialogedition.h"

ListeDialogEdition::ListeDialogEdition(int IDa, QString nom, QColor fond, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListeDialogEdition)
{
    ui->setupUi(this);

    ID = IDa;
    ui->Nom->setText(nom);
    couleur = fond;
}

ListeDialogEdition::~ListeDialogEdition()
{
    delete ui;
}

void ListeDialogEdition::on_Editer_pressed()
{
    emit boutonClic(ID);
}

void ListeDialogEdition::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter dessin;
    dessin.begin(this);
    dessin.eraseRect(0, 0, 1000, 1000); //Efface tout

    dessin.fillRect(0, 0, width(), height(), couleur);
}
