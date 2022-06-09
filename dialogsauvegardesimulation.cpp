#include "dialogsauvegardesimulation.h"
#include "ui_dialogsauvegardesimulation.h"

DialogSauvegardeSimulation::DialogSauvegardeSimulation(General* generalU, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSauvegardeSimulation)
{
    ui->setupUi(this);

    general = generalU;

    if(general->getPossedeUnFichier())
    {
        ui->NomFichier->setText(general->getNomDeFichier());
        ui->Nom->setText(general->getNom());
        ui->Description->setPlainText(general->getDescription());
    }

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(QPixmap("data/assets/icon.png")));
}

DialogSauvegardeSimulation::~DialogSauvegardeSimulation()
{
    delete ui;
    emit dialogQuit(2);
}

void DialogSauvegardeSimulation::on_Sauvegarder_clicked()
{
    if(ui->Description->toPlainText().contains("ENDOFTHEDESCRIPTION"))
    {
        QMessageBox::critical(this, "Erreur", "ENDOFTHEDESCRIPTION ne doit pas figurer dans la description");
        return;
    }
    //Sauvegarde des particules
    bool toutSauvegarder(true);
    QString retourParticules("");
    QStringList particulesSauvegarde;
    for (int p(0); p < general->nombreParticule(); p++) {
        DonneeParticule* particule(general->getParticule(p));
        QString nomDeFichier;
        bool nouveauFichier(false);

        if(particule->getPossedeUnFichier())
        {
            nomDeFichier = particule->getNomDeFichier();
        }
        else
        {
            nomDeFichier = particule->getNom();
            if(nomDeFichier.size() > 200)
            {
                nomDeFichier.resize(200);
            }
            while (nomDeFichier.startsWith(' ') || nomDeFichier.startsWith('.')) {
                nomDeFichier.remove(0, 1);
            }
            while (nomDeFichier.endsWith(' ') || nomDeFichier.endsWith('.')) {
                nomDeFichier.chop(1);
            }
            nomDeFichier = nomDeFichier.replace('\\', '_');
            nomDeFichier = nomDeFichier.replace('/', '_');
            nomDeFichier = nomDeFichier.replace(':', '_');
            nomDeFichier = nomDeFichier.replace('*', '_');
            nomDeFichier = nomDeFichier.replace('?', '_');
            nomDeFichier = nomDeFichier.replace('"', '_');
            nomDeFichier = nomDeFichier.replace('<', '_');
            nomDeFichier = nomDeFichier.replace('>', '_');
            nomDeFichier = nomDeFichier.replace('|', '_');

            particule->setNomDeFichier(nomDeFichier);
            particule->setPossedeUnFichier(true);

            nouveauFichier = true;
        }

        FileText sauvegarde;
        sauvegarde.write(particule->getNom());
        sauvegarde.write(particule->getFormule());
        sauvegarde.write(QString::number(particule->getCouleurParticule().red()));
        sauvegarde.write(QString::number(particule->getCouleurParticule().green()));
        sauvegarde.write(QString::number(particule->getCouleurParticule().blue()));
        sauvegarde.write(QString::number(particule->getEtat()));
        sauvegarde.write(QString::number(particule->getPHValeur()));
        sauvegarde.write(QString::number(particule->getRouge()));
        sauvegarde.write(QString::number(particule->getVert()));
        sauvegarde.write(QString::number(particule->getBleu()));
        sauvegarde.write(QString::number(particule->getConductivite()));
        if(sauvegarde.save("data/bibliotheque/particules/" + nomDeFichier + ".srpart"))
        {
            if(nouveauFichier)
            {
                retourParticules += "\n" + nomDeFichier + " a été créée dans le dossier particule";
            }
            particulesSauvegarde.append(nomDeFichier);
        }
        else
        {
            toutSauvegarder = false;
            retourParticules += "\n" + nomDeFichier + " n'a pas pu être sauvegardé";
        }
    }
    if(!toutSauvegarder)
    {
        QMessageBox::critical(this, "Erreur", "La simulation n'a pas pu être sauvegardée" + retourParticules);
        return;
    }
    QStringList sauvegardeReactions;
    for (int r(0); r < general->nombreReaction(); r++) {
        for (int p(0); p < general->getReaction(r)->getProportionReactifs().size(); p++) {
            sauvegardeReactions.append(QString::number(general->getReaction(r)->getProportionReactifs()[p]));
        }
        sauvegardeReactions.append("ENDLIST");
        for (int p(0); p < general->getReaction(r)->getReactifsID().size(); p++) {
            sauvegardeReactions.append(QString::number(general->getReaction(r)->getReactifsID()[p]));
        }
        sauvegardeReactions.append("ENDLIST");
        for (int p(0); p < general->getReaction(r)->getProportionProduits().size(); p++) {
            sauvegardeReactions.append(QString::number(general->getReaction(r)->getProportionProduits()[p]));
        }
        sauvegardeReactions.append("ENDLIST");
        for (int p(0); p < general->getReaction(r)->getProduitsID().size(); p++) {
            sauvegardeReactions.append(QString::number(general->getReaction(r)->getProduitsID()[p]));
        }
        sauvegardeReactions.append("ENDLIST");
        for (int p(0); p < general->getReaction(r)->getEfficaciteCatalyseurs().size(); p++) {
            sauvegardeReactions.append(QString::number(general->getReaction(r)->getEfficaciteCatalyseurs()[p]));
        }
        sauvegardeReactions.append("ENDLIST");
        for (int p(0); p < general->getReaction(r)->getCatalyseursID().size(); p++) {
            sauvegardeReactions.append(QString::number(general->getReaction(r)->getCatalyseursID()[p]));
        }
        sauvegardeReactions.append("ENDLIST");
        sauvegardeReactions.append(QString::number(general->getReaction(r)->getFacteurPreExp()));
        sauvegardeReactions.append(QString::number(general->getReaction(r)->getEnergieActivation()));
        sauvegardeReactions.append(QString::number(general->getReaction(r)->getDeltaTemperature()));
        sauvegardeReactions.append(QString::number(general->getReaction(r)->getPHCentre()));
        sauvegardeReactions.append(QString::number(general->getReaction(r)->getPHAmplitude()));
        if(general->getReaction(r)->getVersAcide())
        {
            sauvegardeReactions.append("1");
        }
        else
        {
            sauvegardeReactions.append("0");
        }
        sauvegardeReactions.append("ENDREACTION");
    }
    FileText sauvegardeSimulation;
    sauvegardeSimulation.write(ui->Nom->text());
    sauvegardeSimulation.write(ui->Description->toPlainText());
    sauvegardeSimulation.write("ENDOFTHEDESCRIPTION");
    sauvegardeSimulation.write(particulesSauvegarde);
    sauvegardeSimulation.write("ENDPARTICLES");
    sauvegardeSimulation.write(sauvegardeReactions);
    sauvegardeSimulation.write("ENDREACTIONS");
    if(!sauvegardeSimulation.save("data/bibliotheque/simulations/" + ui->NomFichier->text() + ".srsim"))
    {
        QMessageBox::critical(this, "Erreur", "La simulation n'a pas pu être sauvegardée" + retourParticules);
        return;
    }
    general->setNomDeFichier(ui->NomFichier->text());
    general->setNom(ui->Nom->text());
    general->setDescription(ui->Description->toPlainText());
    general->setPossedeUnFichier(true);
    QMessageBox::information(this, "Sauvegarde", "La simulation a bien été sauvegardée" + retourParticules);
    close();
}

void DialogSauvegardeSimulation::on_Annuler_clicked()
{
    close();
}

void DialogSauvegardeSimulation::on_NomFichier_textChanged()
{
    bool nomValide(true);
    QString nom(ui->NomFichier->text());
    if(nom.size() == 0)
    {
        nomValide = false;
    }
    if(nom.startsWith('.') || nom.startsWith(' ') || nom.endsWith('.') || nom.endsWith(' '))
    {
        nomValide = false;
    }
    if(nom.contains('\\') || nom.contains('/') || nom.contains(':') || nom.contains('*') || nom.contains('?') || nom.contains('"') || nom.contains('<') || nom.contains('>') || nom.contains('|'))
    {
        nomValide = false;
    }
    if(!nomValide)
    {
        ui->Sauvegarder->setEnabled(false);
        return;
    }
    ui->Sauvegarder->setEnabled(true);
    if(FileText::exists("data/bibliotheque/simulations/" + nom + ".srsim"))
    {
        ui->Sauvegarder->setText("Sauvegarder et remplacer");
    }
    else
    {
        ui->Sauvegarder->setText("Sauvegarder");
    }
}
