#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main( int argc , char **argv )
{
    QApplication a( argc , argv );

        //  TRADUCTION EN FRANCAIS
    QString locale( QLocale::system().name().section( '_' , 0 , 0 ) );
    QTranslator translator;
    translator.load( QString( "qt_" ) + locale , QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    a.installTranslator( &translator );

        //EXECUTION DE LA FENETRE
    MainWindow w;
    w.show();

        //IMPORTANT : CREATION ET VERIFICATION DE LA BASE DE DONNEE
    MainWindow::Creation_BaseDeDonnees();

        //VERIFICATION DES RETARDATAIRES
    MainWindow::VerificationPret();

        //SITUATIONS DE LA BIBLIOTHEQUE
    MainWindow::afficherTableLecteur( &w );
    MainWindow::afficherTableLivre( &w );

    return a.exec();

}
