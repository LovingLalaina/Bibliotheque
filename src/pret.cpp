#include "pret.h"
#include "fonctionsecondaire.h"
#include "lecteur.h"
#include "livre.h"

void ajouterPret( const char *numeroLecteur , const char *numeroLivre )
{
        //  VERIFICATIONS DU LIVRE DEMANDE
    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if ( fichierErreur( monFichierLivre , type::livre ) )   return;

    livreStruct *livre( new livreStruct );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( !strcmp( numeroLivre , livre->numero ) )
        {
            rewind( monFichierLivre );
            break;
        }
    }

    if( feof( monFichierLivre ) )
    {
        delete livre;
        fclose( monFichierLivre );
        QMessageBox::information( nullptr , "Désolé" , "Le livre que vous avez demandé ne se trouve pas dans la bibliothèque");
        return;
    }
    else if( livre->disponibilite == disponibilite::Non )
    {
        delete livre;
        fclose( monFichierLivre );
        QMessageBox::information( nullptr , "Désolé" , "Ce livre est actuellement emprunté");
        return;
    }

    fclose( monFichierLivre );

    //  VERIFICATIONS DU LECTEUR DEMANDE

    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) );
    if ( fichierErreur( monFichierLecteur , type::lecteur ) ){  delete livre;   return; }

    lecteurStruct *lecteur( new lecteurStruct );

    while( !feof( monFichierLecteur ) )
    {
        lireLecteur( monFichierLecteur , lecteur );

        if( !strcmp( numeroLecteur , lecteur->numero ) )
        {
            fclose( monFichierLecteur );

            FILE *monFichierPret( fopen( pretChemin , "r" ) );
            if ( fichierErreur( monFichierPret , type::pret ) ){     delete lecteur;    delete livre;     return;     }

            pretStruct *pret( new pretStruct );

            unsigned int *dernierPret( new unsigned int(0) );

            while( !feof( monFichierPret ) )
            {
                lirePret( monFichierPret , pret );

                if( !strcmp( numeroLecteur , pret->numeroLecteur ) && pret->numero == 3 )
                {
                    delete pret;
                    delete livre;
                    delete lecteur;
                    delete dernierPret;
                    fclose( monFichierPret );
                    QMessageBox::warning( nullptr , "Désolé" , "Ce lecteur a déjà emprunté 3 livres");
                    return;
                }
                else if( !strcmp( numeroLecteur , pret->numeroLecteur ) )   *dernierPret = pret->numero;
            }

            delete pret;
            fclose( monFichierPret );

            if( QMessageBox::question( nullptr , "Confirmation" , QString("Confirmez le prêt :\nLecteur N° ") + numeroLecteur + QString(" (") + lecteur->nom + QString(")\nLivre N° ") + numeroLivre + QString(" (") + livre->design + QString(")"), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No )
            {
                delete livre;
                delete lecteur;
                delete dernierPret;
                return;
            }

            delete lecteur;
            delete livre;

            monFichierPret = fopen( pretChemin , "a" );
            if ( fichierErreur( monFichierPret , type::pret ) )     return;

            if( nonVide( monFichierPret ) )    fprintf( monFichierPret , "\n" );

            char aujourdui[11];
            dateFormatage( aujourdui , QDate::currentDate() );

            char dansUneSemaine[11];
            dateFormatage( dansUneSemaine , QDate::currentDate().addDays(7) );

            fprintf( monFichierPret , "%u\t%s\t%s\t%s\t%s" , *dernierPret + 1 , numeroLecteur , numeroLivre , aujourdui , dansUneSemaine );

            delete dernierPret;

            fclose( monFichierPret );

            //  MODIFICATION DU FICHIER LIVRE
            monFichierLivre = fopen( livreChemin , "r" );
            if ( fichierErreur( monFichierLivre , type::livre ) )   return;

            livre = new livreStruct();

            while( !feof( monFichierLivre ) )
            {
                lireLivre( monFichierLivre , livre );

                if( !strcmp( numeroLivre , livre->numero ) )
                {
                    fclose( monFichierLivre );

                    disponibilite *dispo( new disponibilite( Non ) );
                    int *prochainNombrePret( new int( livre->nombrePret +1 ) );

                    modifierLivre( livre->numero , livre->design , livre->auteur , livre->date , *dispo , *prochainNombrePret );

                    delete livre;
                    delete dispo;
                    delete prochainNombrePret;

                    QMessageBox::information( nullptr , "Information", "Pret reussi avec succès" );

                    return;
                }
            }
        }
    }

    delete lecteur;

    QMessageBox::information( nullptr , "Désolé" , "Le lecteur que vous avez demandé n'est pas membre ici");

}

void supprimerPret( char *numeroLecteur , const char *numeroLivre )
{
    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if( fichierErreur( monFichierLivre , type::livre ) )    return;

    livreStruct *livre( new livreStruct );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( !strcmp( livre->numero , numeroLivre ) )
        {
            delete livre;
            fclose( monFichierLivre );

            FILE *monFichierPret( fopen( pretChemin , "r" ) );
            if( fichierErreur( monFichierPret , type::pret ) )  return;

            bool *enRetard( new bool( false ) );

            pretStruct *pret( new pretStruct );

            do  lirePret( monFichierPret , pret );
            while( strcmp( pret->numeroLivre , numeroLivre ) && !feof( monFichierPret ) );

            fclose( monFichierPret );

            if( !strcmp( numeroLecteur , "" ) )
                if( !strcmp( pret->numeroLivre , numeroLivre ) )
                    strcpy( numeroLecteur , pret->numeroLecteur );

            if( feof( monFichierPret ) && strcmp( pret->numeroLivre , numeroLivre ) )
            {
                delete pret;
                delete enRetard;
                QMessageBox::information( nullptr , "Information" , "Actuellement ce livre n'est pas emprunté");
                return;
            }
            else if( !strcmp( pret->numeroLivre , numeroLivre ) && toQDate( pret->dateRetour) < QDate::currentDate() )
                *enRetard = true;

            if( *enRetard )
                *enRetard = QMessageBox::question( nullptr , "Avertissement" , QString("Le retour de ce livre est en retard\nLe lecteur N°") + pret->numeroLecteur + QString(" doit 5000fmg!!\nConfirmez le retour du livre") , QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::No;
            else
                *enRetard = QMessageBox::question( nullptr , "Confirmation" , QString("Confirmez le retour du livre N°") + pret->numeroLivre + QString(" du lecteur N°") + pret->numeroLecteur, QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No;

            if( *enRetard )
            {
                delete enRetard;
                delete pret;
            }

            delete enRetard;
            delete pret;

            monFichierPret = fopen( pretChemin , "r" );

            FILE *monNouveauFichier = fopen( fichierTemporaire , "w" );
            if( !monNouveauFichier ){   fclose( monFichierPret );   return; }

            bool *uneFois( new bool( false ) );

            pret = new pretStruct();

            while( !feof( monFichierPret ) )
            {
                lirePret( monFichierPret , pret );

                if( strcmp( pret->numeroLivre , numeroLivre ) )
                {
                    if( *uneFois )  fprintf( monNouveauFichier , "\n" );
                    ecrirePret( monNouveauFichier , pret );
                    *uneFois = true;
                }
                else
                    QMessageBox::information( nullptr , "Information" , QString("Le livre N°") + numeroLivre + QString(" a bien été rendu a la bibliothèque\nMerci de votre fidélité" ) );
            }

            delete pret;
            delete uneFois;

            fclose( monFichierPret );
            fclose( monNouveauFichier );

            actualiserFichierPret();

            //MODIFICATION DU FICHIER LIVRE
            FILE *monFichierLivre( fopen( livreChemin , "r" ) );
            if( !monFichierLivre )  return;

            livreStruct *livre( new livreStruct );

            while( !feof( monFichierLivre ) )
            {
                lireLivre( monFichierLivre , livre );

                if( !strcmp( livre->numero , numeroLivre ) )
                {
                    fclose( monFichierLivre );

                    disponibilite *dispo( new disponibilite( Oui ) );
                    modifierLivre( livre->numero , livre->design , livre->auteur , livre->date , *dispo , livre->nombrePret );

                    delete livre;
                    delete dispo;

                    reOrdonnerNumeroPret( numeroLecteur );

                    return;
                }
            }
        }
    }

    delete livre;

    fclose( monFichierLivre );

    QMessageBox::information( nullptr , "Information" , "Ce Livre ne se trouve pas dans la bibliothèque" );

}

void modifierPret( const char *numeroLecteur , const char *numeroPret , const char *datePret )
{
    if( toQDate( datePret ).addDays( 7 ) < QDate::currentDate() )
    {
        if( QMessageBox::question( nullptr , "Attention!!" , "La date de retour sera antérieure à aujourdui , Ce lecteur devra payer une amende de 5000fmg. Confirmez??" , QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::No )
                return;
    }

    FILE *monFichierPret( fopen( pretChemin , "r" ) );
    if( fichierErreur( monFichierPret , type::pret ) )  return;

    pretStruct *pret( new pretStruct );

    while( !feof( monFichierPret ) )
    {
        lirePret( monFichierPret , pret );

        if( !strcmp( pret->numeroLecteur , numeroLecteur ) && !strcmp( QString::number( pret->numero ).toUtf8() , numeroPret ) )
        {
            rewind( monFichierPret );

            FILE *monNouveauFichier( fopen( fichierTemporaire , "w" ) );
            if( fichierErreur( monFichierPret , type::pret ) ){   delete pret;    fclose( monFichierPret )    ;   return; }

            bool *uneFois( new bool( false ) );
            while( !feof( monFichierPret ) )
            {
                lirePret( monFichierPret , pret );

                if( strcmp( QString::number( pret->numero ).toUtf8() , numeroPret ) || strcmp( numeroLecteur , pret->numeroLecteur ) )
                {
                    if( *uneFois )  fprintf( monNouveauFichier , "\n" );
                    ecrirePret( monNouveauFichier , pret );
                }
                else
                {
                    if( *uneFois )  fprintf( monNouveauFichier , "\n" );

                    char dansUneSemaine[11];
                    dateFormatage( dansUneSemaine , toQDate( datePret ).addDays( 7 ) );

                    fprintf( monNouveauFichier , "%u\t%s\t%s\t%s\t%s" , pret->numero , pret->numeroLecteur , pret->numeroLivre , datePret , dansUneSemaine );
                    QMessageBox::information( nullptr , "Information" , QString("Le pret N°") + numeroPret + QString(" du lecteur N°") + numeroLecteur + QString(" a bien été renouvelé le ") + datePret );
                }

                *uneFois = true;
            }

            delete pret;

            fclose( monNouveauFichier );
            fclose( monFichierPret );

            actualiserFichierPret();

            return;
        }
    }

    delete pret;
    fclose( monFichierPret );

    QMessageBox::warning( nullptr , "Désolé" , "Le prêt à renouveler n'a pas été trouvé" );

}

void lirePret( FILE *monFichierPret , pretStruct *pret )
{
    fscanf( monFichierPret , "%u\t%s\t%s\t%s\t%s\n" , &pret->numero , pret->numeroLecteur , pret->numeroLivre , pret->datePret , pret->dateRetour );
}

void ecrirePret( FILE *monFichierPret , const pretStruct *pret )
{
    fprintf( monFichierPret , "%u\t%s\t%s\t%s\t%s" , pret->numero , pret->numeroLecteur , pret->numeroLivre , pret->datePret , pret->dateRetour );
}

void actualiserFichierPret()
{
    FILE *monNouveauFichier( fopen( pretChemin , "w" ) );
    if( fichierErreur( monNouveauFichier , type::pret ) )  return;

    FILE *monAncienFichier( fopen( fichierTemporaire  , "r" ) );
    if( fichierErreur( monAncienFichier , type::pret ) ){  fclose( monNouveauFichier );    return; }

    pretStruct *pret( new pretStruct );

    if( nonVide( monAncienFichier ) )
    {
        lirePret( monAncienFichier , pret );
        ecrirePret( monNouveauFichier , pret );

        while( !feof( monAncienFichier ) )
        {
           fprintf( monNouveauFichier , "\n");
           lirePret( monAncienFichier , pret );
           ecrirePret( monNouveauFichier , pret );
        }

    }

    delete pret;

    fclose( monAncienFichier );
    fclose( monNouveauFichier );

    remove( fichierTemporaire );

}

void reOrdonnerNumeroPret( const char *numeroLecteur )
{
    FILE *monNouveauFichier( fopen( fichierTemporaire , "w" ) ) , *monAncienFichier( fopen( pretChemin , "r" ) );
    if( fichierErreur( monNouveauFichier , type::pret ) || fichierErreur( monAncienFichier , type::pret ) ){    fclose( monNouveauFichier );    fclose( monAncienFichier );     return; }

    unsigned int *i( new unsigned int( 1 ) );
    bool *uneFois( new bool( false ) );

    pretStruct *pret( new pretStruct );

    while( !feof( monAncienFichier ) )
    {
        lirePret( monAncienFichier , pret );

        if( *uneFois )  fprintf( monNouveauFichier , "\n" );

        if( strcmp( numeroLecteur , pret->numeroLecteur ) )
        {
            ecrirePret( monNouveauFichier , pret );
        }
        else
        {
            fprintf( monNouveauFichier , "%u\t%s\t%s\t%s\t%s" , *i , pret->numeroLecteur , pret->numeroLivre , pret->datePret , pret->dateRetour );
            ++(*i);
        }

        *uneFois = true;
    }

    delete pret;
    delete i;
    delete uneFois;

    fclose( monNouveauFichier );
    fclose( monAncienFichier );

   actualiserFichierPret();

}
