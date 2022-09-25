#include "livre.h"
#include "pret.h"
#include "fonctionSecondaire.h"

void ajouterLivre( const char *numeroLivre , const char *designLivre , const char *auteurLivre , const char *dateLivre )
{
    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if ( fichierErreur( monFichierLivre , type::livre ) )   return;

    bool *memeLivre( new bool( false ) );

    livreStruct *livre( new livreStruct ) , *livreCopie( new livreStruct );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( !( strcmp( livre->numero , numeroLivre ) || strcmp( livre->design ,  designLivre ) || strcmp( livre->auteur , auteurLivre ) || strcmp( livre->date , dateLivre ) ) )
        {
            delete livre;
            delete livreCopie;
            delete memeLivre;
            fclose( monFichierLivre );
            QMessageBox::information( nullptr , "Information!!!", "Ce livre est deja saisi" );
            return;
        }
        else if ( strcmp( livre->design ,  designLivre ) && !strcmp( livre->numero , numeroLivre ) )
        {
            delete livre;
            delete livreCopie;
            delete memeLivre;
            fclose( monFichierLivre );
            QMessageBox::information( nullptr , "Desole!!!", QString("Le numero ") + livre->numero + QString(" correspond deja à un autre livre : ") + livre->design );
            return;
        }
        else if( ! ( strcmp( livre->numero , numeroLivre ) || strcmp( livre->design ,  designLivre ) ) )
        {
            delete livre;
            delete livreCopie;
            delete memeLivre;
            fclose( monFichierLivre );
            QMessageBox::information( nullptr , "Information" , livre->auteur + QString(" avait déjà ecrit un même livre ") + livre->design + QString(" le ") + toQDate( livre->date ).toString() );
            return;
        }
        else if( strcmp( livre->numero , numeroLivre ) && !strcmp( livre->design ,  designLivre ) )
        {
            *memeLivre = true;
            *livreCopie = *livre;
        }

    }

    delete livre;
    fclose( monFichierLivre );

    if( *memeLivre )
    {
        if( !( strcmp( livreCopie->auteur , auteurLivre ) || strcmp( livreCopie->date , dateLivre ) ) )
        {
            delete livreCopie;
            delete memeLivre;
            QMessageBox::warning( nullptr , "Désolé" , QString("Ajout impossible!!! il ne peut y avoir qu' un seul exemplaire de ce livre dans la bibliothèque :(\nC'est le livre N°") + livreCopie->numero );
            return;
        }
        else *memeLivre = QMessageBox::question( nullptr , "Confirmation!!", livreCopie->auteur + QString(" avait déjà ecrit un même livre ") + livreCopie->design + QString(" le ") + toQDate(livreCopie->date).toString() + QString("\nConfirmez l'ajout!!") , QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No;
    }
    else
        *memeLivre =  QMessageBox::question( nullptr , "Confirmation" , QString("Voulez-vous vraiment ajouter le livre ") + designLivre + QString(" à la bibliothèque?") , QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No;

    delete livreCopie;

    if( *memeLivre )
    {
        delete memeLivre;
        return;
    }

    delete memeLivre;

    monFichierLivre = fopen( livreChemin , "a" );
    if ( fichierErreur( monFichierLivre , type::livre ) )   return;

    if( nonVide( monFichierLivre ) )   fprintf( monFichierLivre , "\n" );

    fprintf( monFichierLivre , "%s\t%s\t%s\t%s\t1\t0" , numeroLivre , designLivre ,  auteurLivre ,  dateLivre );

    fclose( monFichierLivre );

    QMessageBox::information( nullptr , "Felicitation!!!" , QString("Le livre ") + designLivre + QString( " a bien été ajouté à la bibliothèque\nSon numero est le " ) + numeroLivre );

}

void supprimerLivre( const char *numeroLivre )
{
    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if( fichierErreur( monFichierLivre , type::livre ) )    return;

    livreStruct *livre( new livreStruct );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( !strcmp( livre->numero , numeroLivre ) )
        {
            fclose( monFichierLivre );

            FILE *monFichierPret( fopen( pretChemin , "r" ) );
            if( fichierErreur( monFichierPret , type::pret ) ){ delete livre;   return; }

            pretStruct *pret( new pretStruct );

            while( !feof( monFichierPret ) )
            {
                lirePret( monFichierPret , pret );

                if( !strcmp( numeroLivre , pret->numeroLivre ) )
                {
                    delete livre;
                    delete pret;
                    fclose( monFichierPret );
                    QMessageBox::warning( nullptr , "Avertissement" , QString("Ce livre est encore prêté par le lecteur N°") + pret->numeroLecteur + QString(" et ne peut être supprimer"));
                    return;
                }
            }

            delete pret;
            fclose( monFichierPret );

            if( QMessageBox::question( nullptr , "Attention!!!" , QString("Êtes-vous vraiment sûr de vouloir supprimer le livre ") + livre->design + QString(" ayant le numero ") + livre->numero + QString(" ?"), QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::No )
            {
                delete livre;
                return;
            }

            delete livre;

            monFichierLivre = fopen( livreChemin , "r" );

            FILE *monNouveauFichier( fopen( fichierTemporaire , "w" ) );
            if( fichierErreur( monNouveauFichier , type::livre ) ){ fclose( monFichierLivre );  return; }

            bool *uneFois( new bool(false) );

            livre = new livreStruct();

            while( !feof( monFichierLivre ) )
           {
                lireLivre( monFichierLivre , livre );

                if( strcmp( livre->numero , numeroLivre ) )
                {
                    if( *uneFois )  fprintf( monNouveauFichier , "\n" );
                    ecrireLivre( monNouveauFichier , livre );
                    *uneFois = true;
                }
                else
                    QMessageBox::information( nullptr , "Information" , QString("Le livre ") + livre->design + QString(" ayant le numero ") + livre->numero + QString(" a bien été supprimé") );
            }

            //  A RECHERCHER : RENOMMAGE ET SUPPRESSION DE FICHIER AVEC C

            delete livre;
            delete uneFois;

            fclose( monNouveauFichier );
            fclose( monFichierLivre );

            actualiserFichierLivre();

            return;
        }
    }

    delete livre;
    fclose( monFichierLivre );

    QMessageBox::information( nullptr , "Information" , "Le livre à supprimer n'a pas été trouvé" );

}

void modifierLivre( const char *numeroLivre , const char *designLivre , const char *auteurLivre , const char *dateLivre , disponibilite &disponibilite , int &nombrePret )
{
    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if( fichierErreur( monFichierLivre , type::livre ) )    return;

    livreStruct *livre( new livreStruct );

    if( nombrePret == -1 )
    {
        do  lireLivre( monFichierLivre , livre );
        while ( strcmp( livre->numero , numeroLivre ) && !feof( monFichierLivre ) );

        fclose( monFichierLivre );
        if( strcmp( livre->numero , numeroLivre ) )
        {
            delete livre;
            QMessageBox::information( nullptr , "Information" , "Le Livre à modifier n'a pas été trouvé" );
            return;
        }

        if( !( ( strcmp( livre->design , designLivre ) && strcmp( "" , designLivre ) ) ||  ( strcmp( livre->auteur , auteurLivre ) && strcmp( "" , auteurLivre ) ) || ( strcmp( livre->date , dateLivre ) && strcmp( "" , dateLivre ) ) ) )
        {
            delete livre;
            QMessageBox::information( nullptr , "Information" , "Il n'y rien à modifer" );
            return;
        }

        monFichierLivre = fopen( livreChemin , "r" );

        QString *nom_a_modifier( new QString( livre->design ) );

        do  lireLivre( monFichierLivre , livre );
        while( strcmp( livre->design , designLivre ) && !feof( monFichierLivre ) );

        fclose( monFichierLivre );

        bool *nomDejaUtilise = new bool( false );

        if( strcmp( livre->numero , numeroLivre ) && !strcmp( livre->design , designLivre ) )      *nomDejaUtilise = true;

        if( *nomDejaUtilise )   *nomDejaUtilise = QMessageBox::question( nullptr , "Attention!!" , designLivre + QString(" est déjà un livre de la bibliothèque (N°") + livre->numero + QString(")\nVoulez-vous quand même le modifier ?") , QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::No;
        else                           *nomDejaUtilise = QMessageBox::question( nullptr , "Confirmation!!" , QString("Confirmez les modifications :\nLivre N°") + numeroLivre + QString("\nDesign : ") + designLivre + QString("\nAuteur : ") + auteurLivre + QString("\nDate d'edition : ") + toQDate(dateLivre).toString() , QMessageBox::Yes | QMessageBox::No) == QMessageBox::No;

        delete livre;
        delete nom_a_modifier;

        if( *nomDejaUtilise )
        {
            delete nomDejaUtilise;
            return;
        }

        delete nomDejaUtilise;

        monFichierLivre = fopen( livreChemin , "r" );

    }

    FILE *monNouveauFichier( fopen( fichierTemporaire , "w" ) );
    if( fichierErreur( monNouveauFichier , type::livre ) ){ fclose( monFichierLivre );  return; }

    livre = new livreStruct();

    bool *uneFois( new bool( false ) );
    livreStruct *livreCopie( new livreStruct );
    int *temoin( new int( nombrePret ) );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( strcmp( livre->numero , numeroLivre ) )
        {
            if( *uneFois )  fprintf( monNouveauFichier , "\n" );
            ecrireLivre( monNouveauFichier , livre );
         }
        else
        {

            if( strcmp( "" , designLivre ) )    strcpy( livre->design , designLivre );
            if( strcmp( "" , auteurLivre ) )    strcpy( livre->auteur , auteurLivre );

            if( nombrePret == -1 )
            {
                disponibilite = livre->disponibilite;
                nombrePret = livre->nombrePret;
             }

           *livreCopie = *livre;
            strcpy( livreCopie->date , dateLivre );

            if( *uneFois )  fprintf( monNouveauFichier , "\n" );
            fprintf( monNouveauFichier , "%s\t%s\t%s\t%s\t%d\t%d" , livre->numero , livre->design , livre->auteur , dateLivre , disponibilite , nombrePret );
        }

        *uneFois = true;
    }

    //  A RECHERCHER : RENOMMAGE ET SUPPRESSION DE FICHIER AVEC C
    delete uneFois;
    delete livre;

    fclose( monNouveauFichier );
    fclose( monFichierLivre );

    if( verifierLivre( livreCopie ) )   remove( fichierTemporaire );
    else
    {
        if( *temoin == -1 )  QMessageBox::information( nullptr , "Information" , QString("Le livre N°") + numeroLivre + QString(" a bien été modifié ") );
        actualiserFichierLivre();
    }

    delete temoin;
    delete livreCopie;

}

void lireLivre( FILE *monFichierLivre , livreStruct *livre )
{
    fscanf( monFichierLivre , "%s\t%s\t%s\t%s\t%d\t%d\n" , livre->numero , livre->design , livre->auteur , livre->date , &livre->disponibilite , &livre->nombrePret );
}

void ecrireLivre( FILE *monFichierLivre , const livreStruct *livre )
{
    fprintf( monFichierLivre , "%s\t%s\t%s\t%s\t%d\t%d" , livre->numero , livre->design , livre->auteur , livre->date , livre->disponibilite , livre->nombrePret );
}

void actualiserFichierLivre()
{
    FILE *monNouveauFichier( fopen( livreChemin , "w" ) );
    if( fichierErreur( monNouveauFichier , type::livre ) )  return;

    FILE *monAncienFichier = fopen( fichierTemporaire  , "r" );
    if( fichierErreur( monAncienFichier , type::livre ) ){  fclose( monNouveauFichier );   return;  }

    livreStruct *livre( new livreStruct );

    if( nonVide( monAncienFichier ) )
    {
        lireLivre( monAncienFichier , livre );
        ecrireLivre( monNouveauFichier , livre );

        while( !feof( monAncienFichier ) )
        {
           fprintf( monNouveauFichier , "\n");
           lireLivre( monAncienFichier , livre );
           ecrireLivre( monNouveauFichier , livre );
        }

    }

    delete livre;

    fclose( monAncienFichier );
    fclose( monNouveauFichier );

    remove( fichierTemporaire );

}

bool verifierLivre( const livreStruct *livreCopie )
{
    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if( fichierErreur( monFichierLivre , type::livre ) )    return false;

    livreStruct *livre( new livreStruct );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( strcmp( livreCopie->numero , livre->numero ) && !( strcmp( livreCopie->auteur , livre->auteur ) || strcmp( livreCopie->design , livre->design ) || strcmp( livreCopie->date , livre->date ) ) )
        {
            delete livre;
            fclose( monFichierLivre );
            QMessageBox::warning( nullptr , "Désolé" , "Modification impossible!!! il ne peut y avoir qu' un seul exemplaire de ce livre dans la bibliothèque :(" );
            return true;
        }
    }

    delete livre;
    fclose( monFichierLivre );

    return false;
}
