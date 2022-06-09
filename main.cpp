#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QThread>
#include "filetext.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen* splash = new QSplashScreen(QPixmap("data/assets/splash.png"));
    splash->show();

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    if(!FileText::exists("data/preferences.txt"))
    {
        FileText creer;
        creer.write("Thème Sombre : Non");
        creer.write("Fond.R : 140");
        creer.write("Fond.G : 140");
        creer.write("Fond.B : 140");
        creer.write("Afficher FPS : Non");
        creer.write("Plein écran : Non");
        creer.save("data/preferences.txt");
    }
    FileText pref("data/preferences.txt");
    if(pref[0].endsWith("Oui"))
    {
        //Thème sombre par TheMachine QuantumCD et leplatrem sur GitHub Gist

        qApp->setStyle(QStyleFactory::create("Fusion"));

        QColor darkGray(53, 53, 53);
        QColor gray(128, 128, 128);
        QColor black(25, 25, 25);
        QColor blue(42, 130, 218);

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, darkGray);
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, black);
        darkPalette.setColor(QPalette::AlternateBase, darkGray);
        darkPalette.setColor(QPalette::ToolTipBase, blue);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, darkGray);
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Link, blue);
        darkPalette.setColor(QPalette::Highlight, blue);
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        darkPalette.setColor(QPalette::Active, QPalette::Button, gray.darker());
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, gray);
        darkPalette.setColor(QPalette::Disabled, QPalette::Light, darkGray);

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }

    MainWindow w;
    w.show();

    delete splash;

    return a.exec();
}
