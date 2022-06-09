#include "dialogaide.h"
#include "ui_dialogaide.h"

DialogAide::DialogAide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAide)
{
    ui->setupUi(this);

    FileText chargement;
    if(chargement.load("data/assets/aide/aide.txt"))
    {
        int lecture(0);
        while (chargement.read(lecture).startsWith("SOMMAIRE:")) {
            sommaire.append(chargement.read(lecture).right(chargement.read(lecture).size() - 9));
            QListWidgetItem* nouveau(new QListWidgetItem(sommaire.last()));
            list.append(nouveau);
            ui->Sommaire->addItem(nouveau);
            lecture++;
        }
        QString texte;
        while (lecture < chargement.size()) {
            if(chargement.read(lecture).startsWith("TITRE:"))
            {
                pages.append(texte);
                texte = "<h1>" + chargement.read(lecture).right(chargement.read(lecture).size() - 6) + "</h1>";
            }
            else
            {
                texte += "<br>" + chargement.read(lecture);
            }
            lecture++;
        }
        pages.append(texte);
        pages.removeFirst();
    }

    lirePage(0);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogAide::~DialogAide()
{
    delete ui;
    emit dialogQuit(4);
}

void DialogAide::lirePage(int num)
{
    QString page(pages[num]);
    QStringList separation(page.split("IMG:"));
    ui->Aide->setText(separation[0]);
    for (int imgs(1); imgs < separation.size(); imgs++) {
        ajouterImage(separation[imgs].left(4));
        if(separation[imgs].left(4) != separation[imgs].right(4))
        {
            ui->Aide->append(separation[imgs].right(separation[imgs].size() - (separation[imgs].split("<br>")[0].size() + 4)));
        }
    }
}

void DialogAide::ajouterImage(QString id)
{
    QString file("data/assets/aide/" + id + ".png");
    QUrl Uri ( QString ( "file://%1" ).arg ( file ) );
    QImage image = QImageReader ( file ).read();

    QTextDocument * textDocument = ui->Aide->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    ui->Aide->moveCursor(QTextCursor::End);
    QTextCursor cursor = ui->Aide->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
}

void DialogAide::on_Fermer_clicked()
{
    close();
}

void DialogAide::on_Sommaire_itemClicked(QListWidgetItem *item)
{
    int index(list.indexOf(item));
    if(index != -1)
    {
        lirePage(index);
    }
}
