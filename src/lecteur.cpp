#include "lecteur.h"
#include "pret.h"
#include "fonctionSecondaire.h"

void ajouterLecteur( const char *numeroLecteur , const char *nomLecteur )
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) );
    if( fichierErreur( monFichierLecteur , type::lecteur ) )    return;

    lecteurStruct *lecteur( new lecteurStruct );

    bool *nomDifferent( new bool( true ) );

    while( !feof( monFichierLecteur ) )
    {
        lireLecteur( monFichierLecteur , lecteur );

        if( !( strcmp( lecteur->numero , numeroLecteur ) || strcmp( lecteur->nom ,  nomLecteur ) ) )
        {
            delete lecteur;
            delete nomDifferent;
            fclose( monFichierLecteur );
            QMessageBox::information( nullptr , "Information!!!",
            QString("Le lecteur ") + nomLecteur + QString(" est déjà saisi.") );
            return;
        }
        else if ( strcmp( lecteur->nom ,  nomLecteur )  && !strcmp( lecteur->numero , numeroLecteur) )
        {
            delete nomDifferent;
            fclose( monFichierLecteur );
            QMessageBox::information( nullptr , "Desole!!!",
            QString("Le N°") + numeroLecteur +
            QString(" est déjà le numero d'un membre de la bibliothèque (") + lecteur->nom +
            QString(")") );
            delete lecteur;
            return;
        }
        else if( strcmp( lecteur->numero , numeroLecteur ) && !strcmp( lecteur->nom , nomLecteur ) )
            *nomDifferent = false;
    }

    delete lecteur;

    fclose( monFichierLecteur );

    if( *nomDifferent )
        *nomDifferent = QMessageBox::question( nullptr , "Confirmation!!" ,
         QString("Voulez-vous vraiment ajouter ") + nomLecteur +
         QString(" comme nouveau membre?") ,
         QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No;
    else
        *nomDifferent = QMessageBox::question( nullptr , "Attention!!" ,
         nomLecteur + QString(" est déjà le nom d'un des membres de la bibliothèque") +
         QString("\nVoulez-vous quand même l'ajouter?") ,
         QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No;

    if( *nomDifferent )
    {
        delete nomDifferent;
        return;
    }

    delete nomDifferent;

    monFichierLecteur = fopen( lecteurChemin , "a" );
    if ( fichierErreur( monFichierLecteur , type::lecteur ) )   return;

    if( nonVide( monFichierLecteur ) )   fprintf( monFichierLecteur , "\n" );

    fprintf( monFichierLecteur , "%s\t%s" , numeroLecteur , nomLecteur );

    fclose( monFichierLecteur );

    QMessageBox::information( nullptr , "Felicitation!!!" , nomLecteur +
    QString( " a bien été admis(e) comme nouveau membre\nSon numero est le " ) +
    numeroLecteur );

}

void supprimerLecteur( const char *numeroLecteur )
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) );
    if( fichierErreur( monFichierLecteur , type::lecteur ) )    return;

    lecteurStruct *lecteur( new lecteurStruct );

    while( !feof( monFichierLecteur ) )
    {
        lireLecteur( monFichierLecteur , lecteur );

        if( !strcmp( lecteur->numero , numeroLecteur ) )
        {
            fclose( monFichierLecteur );

            FILE *monFichierPret( fopen( pretChemin , "r" ) );
            if( fichierErreur( monFichierPret , type::pret ) ){ delete lecteur;  return; }

            pretStruct *pret( new pretStruct );

            while( !feof( monFichierPret ) )
            {
                lirePret( monFichierPret , pret );

                if( !strcmp( numeroLecteur , pret->numeroLecteur ) )
                {
                    delete pret;
                    delete lecteur;
                    fclose( monFichierPret );
                    QMessageBox::warning( nullptr , "Avertissement" , QString("Ce lecteur prête encore le livre N°") + pret->numeroLivre + QString(" et ne peut être supprimer") );
                    return;
                }
            }

            delete pret;
            fclose( monFichierPret );

            if( QMessageBox::question( nullptr , "Attention!!!" , QString("Êtes-vous vraiment sûr de vouloir supprimer ") + lecteur->nom + QString(" ayant le numero ") + numeroLecteur + QString(" ?"), QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::No )
            {
                delete lecteur;
                return;
            }

            delete lecteur;

            monFichierLecteur = fopen( lecteurChemin , "r" );
            if( fichierErreur( monFichierLecteur , type::lecteur ) )    return;



            FILE *monNouveauFichier( fopen( fichierTemporaire , "w" ) );
            if( fichierErreur( monNouveauFichier , type::lecteur ) ){   fclose( monFichierLecteur );    return; }

            lecteur = new lecteurStruct;

            bool *uneFois( new bool(false) );

            while( !feof( monFichierLecteur ) )
           {
                lireLecteur( monFichierLecteur , lecteur );

                if( strcmp( lecteur->numero , numeroLecteur ) )
                {
                    if( *uneFois )    fprintf( monNouveauFichier , "\n" );
                    ecrireLecteur( monNouveauFichier , lecteur );
                    *uneFois = true;
                }
                else
                    QMessageBox::information( nullptr , "Information" , QString("Le lecteur ") + lecteur->nom + QString(" ayant le numero ") + numeroLecteur + QString(" a bien été supprimé") );
            }

            delete uneFois;
            delete lecteur;

            fclose( monNouveauFichier );
            fclose( monFichierLecteur );

            //  A RECHERCHER : RENOMMAGE ET SUPPRESSION DE FICHIER AVEC C
            actualiserFichierLecteur();

            return;
        }
    }

    delete lecteur;

    fclose( monFichierLecteur );

    QMessageBox::information( nullptr , "Information" , "Le Lecteur à supprimer n'a pas été trouvé" );

}

void modifierLecteur( const char *numeroLecteur , const char *nomLecteur )
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) );
    if( fichierErreur( monFichierLecteur , type::lecteur ) )    return;

    lecteurStruct *lecteur( new lecteurStruct );

    do  lireLecteur( monFichierLecteur , lecteur );
    while ( strcmp( lecteur->numero , numeroLecteur ) && !feof( monFichierLecteur ) );

    fclose( monFichierLecteur );

    if( strcmp( lecteur->numero , numeroLecteur ) )
    {
        delete lecteur;
        QMessageBox::information( nullptr , "Information" , "Le lecteur à modifier n'a pas été trouvé" );
        return;
    }

    if( !strcmp( lecteur->nom , nomLecteur ) )
    {
        delete lecteur;
        QMessageBox::information( nullptr , "Information" , "Il n'y rien à modifer" );
        return;
    }

    monFichierLecteur = fopen( lecteurChemin , "r" );
    if( fichierErreur( monFichierLecteur , type::lecteur ) ){   delete lecteur; return; }

    QString *nom_a_modifier( new QString( lecteur->nom ) );

    do  lireLecteur( monFichierLecteur , lecteur );
    while( strcmp( lecteur->nom , nomLecteur ) && !feof( monFichierLecteur ) );

    fclose( monFichierLecteur );

    bool *nomDejaUtilise( new bool( false ) );

    if( !strcmp( lecteur->nom , nomLecteur ) )      *nomDejaUtilise = true;

    if( *nomDejaUtilise )   *nomDejaUtilise = QMessageBox::question( nullptr , "Confirmation!!" , nomLecteur + QString(" est déjà le nom d'un membre de la bibliothèque (N°") + lecteur->numero + QString(")\nVoulez-vous quand même modifier le lecteur N°") + numeroLecteur + QString(" ?") , QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::No;
    else                           *nomDejaUtilise = QMessageBox::question( nullptr , "Confirmation!!" , QString("Voulez-vous vraiment modifier ") + nom_a_modifier + QString(" ayant le N°") + numeroLecteur + QString(" par ") + nomLecteur + QString(" ?") , QMessageBox::Yes | QMessageBox::No) == QMessageBox::No;

    delete nom_a_modifier;

    if( *nomDejaUtilise )
    {
        delete lecteur;
        delete nomDejaUtilise;
        return;
    }

    delete lecteur;
    delete nomDejaUtilise;

    monFichierLecteur = fopen( lecteurChemin , "r" );
    if( fichierErreur( monFichierLecteur , type::lecteur ) )    return;

    FILE *monNouveauFichier( fopen( fichierTemporaire , "w" ) );
    if( fichierErreur( monNouveauFichier , type::lecteur ) ){   fclose( monFichierLecteur ); return;    }

    lecteur = new lecteurStruct;

    bool *uneFois( new bool( false ) );

    while(  !feof( monFichierLecteur ) )
    {
        lireLecteur( monFichierLecteur , lecteur );

        if( strcmp( lecteur->numero , numeroLecteur ) )
        {
            if( *uneFois )  fprintf( monNouveauFichier , "\n" );
            ecrireLecteur( monNouveauFichier , lecteur );
        }
        else
        {
            if( *uneFois )  fprintf( monNouveauFichier , "\n" );
            fprintf( monNouveauFichier , "%s\t%s" , lecteur->numero , nomLecteur );
            QMessageBox::information( nullptr , "Information" , QString("Le lecteur ") + lecteur->nom + QString(" ayant le numero ") + lecteur->numero + QString(" a bien été modifié par ") + nomLecteur );
        }

        *uneFois = true;
    }

    //  A RECHERCHER : RENOMMAGE ET SUPPRESSION DE FICHIER AVEC C

    delete uneFois;
    delete lecteur;

    fclose( monFichierLecteur );
    fclose( monNouveauFichier );

    actualiserFichierLecteur();

}

void lireLecteur( FILE *monFichierLecteur , lecteurStruct *lecteur )
{
    fscanf( monFichierLecteur , "%s\t%[^\n]" , lecteur->numero , lecteur->nom );
}

void ecrireLecteur( FILE *monFichierLecteur , const lecteurStruct *lecteur )
{
    fprintf( monFichierLecteur , "%s\t%s" , lecteur->numero , lecteur->nom );
}

void actualiserFichierLecteur()
{
    FILE *monNouveauFichier( fopen( lecteurChemin , "w" ) );
    if( fichierErreur( monNouveauFichier , type::lecteur ) )    return;

    FILE *monAncienFichier( fopen( fichierTemporaire  , "r" ) );
    if( fichierErreur( monAncienFichier , type::lecteur ) ){    fclose( monNouveauFichier );    return; }

    lecteurStruct *lecteur( new lecteurStruct );

    if( nonVide( monAncienFichier ) )
    {
        lireLecteur( monAncienFichier , lecteur );
        ecrireLecteur( monNouveauFichier , lecteur );

        while( !feof( monAncienFichier ) )
        {
           fprintf( monNouveauFichier , "\n");
           lireLecteur( monAncienFichier , lecteur );
           ecrireLecteur( monNouveauFichier , lecteur );
        }
    }

    delete lecteur;
    fclose( monAncienFichier );
    fclose( monNouveauFichier );
    remove( fichierTemporaire );
}
