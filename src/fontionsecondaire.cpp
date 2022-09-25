#include "fonctionSecondaire.h"
#include "mainwindow.h"
#include "pret.h"

bool nonVide( FILE *monFichier )
{
    fpos_t *positionSauvegarde( new fpos_t() );

    fgetpos( monFichier , positionSauvegarde );

    rewind( monFichier );
    fseek( monFichier , 0 , SEEK_END );

    bool res( ftell(monFichier) );

    fsetpos( monFichier , positionSauvegarde );

    delete positionSauvegarde;

    return res;
}

void dateFormatage( char *chaineDate , const QDate &dateRepere )
{
    strcpy( chaineDate , (
    conversion( dateRepere.day() ) +
    QString("/") +
    conversion( dateRepere.month() ) +
    QString("/") +
    QString::number( dateRepere.year() ) ).toUtf8() );
}

const QString conversion( const int &nombre )
{
    if( nombre < 10 )     return QString("0") + QString::number( nombre );
    return QString::number( nombre );
}

const QDate toQDate( const char *datePret )
{
    int jour( 1 ) , mois( 1 ) , annee( 2000 );

    sscanf( datePret , "%d/%d/%d" , &jour , &mois , &annee );

    return QDate( annee , mois , jour );
}

const QString accessibilite( const disponibilite &dispo )
{
    if( dispo == disponibilite::Oui )   return "Oui";
    else                                           return "Non";
}

bool fichierErreur( FILE *monFichier , const type &fichier )
{
    if( !monFichier )
    {
        fclose( monFichier );
        if( fichier == type::lecteur )
            QMessageBox::critical( nullptr , "Attention!!" , "Le fichier Lecteur.dat n'est pas accessible :(" );
        else if( fichier == type::livre )
            QMessageBox::critical( nullptr , "Attention!!" , "Le fichier Livre.dat n'est pas accessible :(" );
        else
            QMessageBox::critical( nullptr , "Attention!!" , "Le fichier Pret.dat n'est pas accessible :(" );
        return true;
    }

    return false;
}
