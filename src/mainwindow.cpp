#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lecteur.h"
#include "livre.h"
#include "pret.h"
#include "fonctionsecondaire.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    setFixedSize( 800 , 600 );

    ui->tableLivreAjout->horizontalHeader()->setStretchLastSection(true);
    ui->tableLivreModifier->horizontalHeader()->setStretchLastSection(true);
    ui->tableLivreSupprimer->horizontalHeader()->setStretchLastSection(true);
    ui->tableLecteurAjout->horizontalHeader()->setStretchLastSection(true);
    ui->tableLecteurModifier->horizontalHeader()->setStretchLastSection(true);
    ui->tableLecteurSupprimer->horizontalHeader()->setStretchLastSection(true);
    ui->tablePret1->horizontalHeader()->setStretchLastSection(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Creation_BaseDeDonnees()
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "a" ) ) , *monFichierLivre( fopen( livreChemin , "a" ) ) , *monFichierPret( fopen( pretChemin , "a" ) );

    if( !monFichierLecteur )     QMessageBox::critical( nullptr , "Attention!!!" , "Fichier Lecteur non créé!! Autorisation manquant!!");
    if( !monFichierLivre )       QMessageBox::critical( nullptr , "Attention!!!" , "Fichier Livre non créé!! Autorisation manquant!!");
    if( !monFichierPret )        QMessageBox::critical( nullptr , "Attention!!!" , "Fichier Pret non créé!! Autorisation manquant!!");


    fclose( monFichierLecteur );
    fclose( monFichierLivre );
    fclose( monFichierPret );

}

void MainWindow::VerificationPret()
{
    FILE *monFichierPret( fopen( pretChemin , "r" ) );
    if( fichierErreur( monFichierPret , pret ) )  return;

    if( !nonVide( monFichierPret ) )    return;

    pretStruct *pret( new pretStruct );

    while( !feof( monFichierPret ) )
    {

        lirePret( monFichierPret , pret );

        if( toQDate( pret->dateRetour ) == QDate::currentDate() )      QMessageBox::warning( nullptr , "Avertissement" , QString("Le lecteur N° ") + pret->numeroLecteur + QString(" doit rendre le livre N° ") + pret->numeroLivre + QString(" aujourd'hui") );
        else if( toQDate( pret->dateRetour ) < QDate::currentDate() )  QMessageBox::warning( nullptr , "Avertissement" , QString("Le lecteur N° ") + pret->numeroLecteur + QString(" est en retard sur le retour du livre N° ") + pret->numeroLivre + QString("!!!\nAmende : 5000fmg") );

    }

    delete pret;
    fclose( monFichierPret );

}


void MainWindow::afficherTableLecteur( MainWindow *w  )
{
    reinitialiserTable( w , type::lecteur );

    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) );
    if( fichierErreur( monFichierLecteur , type::lecteur ) || !nonVide( monFichierLecteur ) )
    {
        fclose( monFichierLecteur );
        return;
    }

    lecteurStruct *lecteur( new lecteurStruct );

     int *i( new int( 0 ) );

    while( !feof( monFichierLecteur ) )
    {
        lireLecteur( monFichierLecteur , lecteur );

        insererItemLecteur( w , lecteur , i );

        ++(*i);
    }

    delete lecteur;
    delete i;
    fclose( monFichierLecteur );

}

void MainWindow::insererItemLecteur( MainWindow *w , const lecteurStruct *lecteur , int *i )
{
    w->ui->tableLecteurAjout->insertRow( w->ui->tableLecteurAjout->rowCount() );
    w->ui->tableLecteurModifier->insertRow( w->ui->tableLecteurModifier->rowCount() );
    w->ui->tableLecteurSupprimer->insertRow( w->ui->tableLecteurSupprimer->rowCount() );

    QTableWidgetItem *itemNumero1( new QTableWidgetItem( QString( lecteur->numero ) ) );
    itemNumero1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNumero2( new QTableWidgetItem( QString( lecteur->numero ) ) );
    itemNumero2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNumero3( new QTableWidgetItem( QString( lecteur->numero ) ) );
    itemNumero3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLecteurAjout->setItem( *i , 0 , itemNumero1 );
    w->ui->tableLecteurModifier->setItem( *i , 0 , itemNumero2 );
    w->ui->tableLecteurSupprimer->setItem( *i , 0 , itemNumero3 );

    QTableWidgetItem *itemNom1( new QTableWidgetItem( QString( lecteur->nom ) ) );
    itemNom1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNom2( new QTableWidgetItem( QString( lecteur->nom ) ) );
    itemNom2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNom3( new QTableWidgetItem( QString( lecteur->nom ) ) );
    itemNom3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLecteurAjout->setItem( *i , 1 , itemNom1 );
    w->ui->tableLecteurModifier->setItem( *i , 1 , itemNom2 );
    w->ui->tableLecteurSupprimer->setItem( *i , 1 , itemNom3 );
}

void MainWindow::afficherTableLivre( MainWindow *w )
{
    reinitialiserTable( w , type::livre );

    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if( fichierErreur( monFichierLivre , type::livre ) || !nonVide( monFichierLivre ) )
    {
        fclose( monFichierLivre );
        return;
    }

    livreStruct *livre( new livreStruct );

    int *i( new int( 0 ) );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        insererItemLivre( w, livre , i );

        ++(*i);
    }

    delete livre;
    delete i;

    fclose( monFichierLivre );

}

void MainWindow::insererItemLivre(MainWindow *w, const livreStruct *livre, int *i )
{
    w->ui->tableLivreAjout->insertRow( w->ui->tableLivreAjout->rowCount() );
    w->ui->tableLivreModifier->insertRow( w->ui->tableLivreModifier->rowCount() );
    w->ui->tableLivreSupprimer->insertRow( w->ui->tableLivreSupprimer->rowCount() );

    QTableWidgetItem *itemNumero1( new QTableWidgetItem( QString( livre->numero ) ) );
    itemNumero1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNumero2( new QTableWidgetItem( QString( livre->numero ) ) );
    itemNumero2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNumero3( new QTableWidgetItem( QString( livre->numero ) ) );
    itemNumero3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLivreAjout->setItem( *i , 0 , itemNumero1 );
    w->ui->tableLivreModifier->setItem( *i , 0 , itemNumero2 );
    w->ui->tableLivreSupprimer->setItem( *i , 0 , itemNumero3 );

    QTableWidgetItem *itemDesign1( new QTableWidgetItem( QString( livre->design ) ) );
    itemDesign1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemDesign2( new QTableWidgetItem( QString( livre->design ) ) );
    itemDesign2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemDesign3( new QTableWidgetItem( QString( livre->design ) ) );
    itemDesign3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLivreAjout->setItem( *i , 1 , itemDesign1 );
    w->ui->tableLivreModifier->setItem( *i , 1 , itemDesign2 );
    w->ui->tableLivreSupprimer->setItem( *i , 1 , itemDesign3 );

    QTableWidgetItem *itemAuteur1( new QTableWidgetItem( QString( livre->auteur ) ) );
    itemAuteur1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemAuteur2( new QTableWidgetItem( QString( livre->auteur ) ) );
    itemAuteur2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemAuteur3( new QTableWidgetItem( QString( livre->auteur ) ) );
    itemAuteur3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLivreAjout->setItem( *i , 2 , itemAuteur1 );
    w->ui->tableLivreModifier->setItem( *i , 2 , itemAuteur2 );
    w->ui->tableLivreSupprimer->setItem( *i , 2 , itemAuteur3 );

    QTableWidgetItem *itemDateEdition1( new QTableWidgetItem( QString( livre->date ) ) );
    itemDateEdition1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemDateEdition2( new QTableWidgetItem( QString( livre->date ) ) );
    itemDateEdition2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemDateEdition3( new QTableWidgetItem( QString( livre->date ) ) );
    itemDateEdition3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLivreAjout->setItem( *i , 3 , itemDateEdition1 );
    w->ui->tableLivreModifier->setItem( *i , 3 , itemDateEdition2 );
    w->ui->tableLivreSupprimer->setItem( *i , 3 , itemDateEdition3 );

    QTableWidgetItem *itemDisponibilite1( new QTableWidgetItem( accessibilite( livre->disponibilite ) ) );
    itemDisponibilite1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemDisponibilite2( new QTableWidgetItem( accessibilite( livre->disponibilite ) ) );
    itemDisponibilite2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemDisponibilite3( new QTableWidgetItem( accessibilite( livre->disponibilite ) ) );
    itemDisponibilite3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLivreAjout->setItem( *i , 4 , itemDisponibilite1 );
    w->ui->tableLivreModifier->setItem( *i , 4 , itemDisponibilite2 );
    w->ui->tableLivreSupprimer->setItem( *i , 4 , itemDisponibilite3 );

    QTableWidgetItem *itemNombrePret1( new QTableWidgetItem( QString::number( livre->nombrePret ) ) );
    itemNombrePret1->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNombrePret2( new QTableWidgetItem( QString::number( livre->nombrePret ) ) );
    itemNombrePret2->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    QTableWidgetItem *itemNombrePret3( new QTableWidgetItem( QString::number( livre->nombrePret ) ) );
    itemNombrePret3->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

    w->ui->tableLivreAjout->setItem( *i , 5 , itemNombrePret1 );
    w->ui->tableLivreModifier->setItem( *i , 5 , itemNombrePret2 );
    w->ui->tableLivreSupprimer->setItem( *i , 5 , itemNombrePret3 );
}

void MainWindow::afficherTablePret( MainWindow *w , const char *numeroLecteur, char *nomLecteur , const bool &afficher )
{
    reinitialiserTable( w , type::pret );

    w->ui->tablePret1->setEnabled( false );

    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) );
    if( fichierErreur( monFichierLecteur , type::lecteur ) )    return;

    lecteurStruct *lecteur( new lecteurStruct );

    do  lireLecteur( monFichierLecteur , lecteur );
    while( strcmp( numeroLecteur , lecteur->numero ) && !feof( monFichierLecteur ) );

    fclose( monFichierLecteur );

    if( !strcmp( nomLecteur , "" ) )
        if( !strcmp( numeroLecteur , lecteur->numero ) )
            strcpy( nomLecteur , lecteur->nom );
    if( strcmp( numeroLecteur , lecteur->numero ) || strcmp( nomLecteur , lecteur->nom ) )
    {
        delete lecteur;
        if( afficher )   QMessageBox::warning( nullptr , "Attention" , "Le lecteur n'a pas été trouvé" );
        return;
    }

    delete lecteur;

    //  ICI LE LECTEUR A ETE TROUVE MAIS PEUT AVOIR EMPRUNTE OU NON

    FILE *monFichierPret( fopen( pretChemin , "r" ) );
    if( fichierErreur( monFichierPret , type::pret ) )  return;

    int *i( new int( 0 ) );

    pretStruct *pret( new pretStruct );

    situation pretSituation[3];

    strcpy( pretSituation[0].numeroLivre , "0" );
    strcpy( pretSituation[1].numeroLivre , "0" );
    strcpy( pretSituation[2].numeroLivre , "0" );

    while( !feof( monFichierPret ) )
    {

        lirePret( monFichierPret , pret );

        if( !strcmp( pret->numeroLecteur , numeroLecteur ) )
        {
            strcpy( pretSituation[*i].numeroLivre , pret->numeroLivre );
            strcpy( pretSituation[*i].datePret , pret->datePret );
            ++(*i);
        }
    }

    delete pret;

    if( *i == 0 )
    {
        if( afficher )  QMessageBox::information( nullptr , "Information" , "Ce lecteur ne prête actuellement aucun livre." );
    }

    FILE *monFichierLivre( fopen( livreChemin , "r" ) );
    if( fichierErreur( monFichierLivre , type::livre ) )    return;

    livreStruct *livre( new livreStruct );

    while( !feof( monFichierLivre ) )
    {
        lireLivre( monFichierLivre , livre );

        if( !strcmp( livre->numero , pretSituation[0].numeroLivre ) )
        {
            strcpy( pretSituation[0].designation , livre->design );
            strcpy( pretSituation[0].auteur , livre->auteur );
            strcpy( pretSituation[0].dateEdition , livre->date );
        }
        else if( !strcmp( livre->numero , pretSituation[1].numeroLivre ) )
        {
            strcpy( pretSituation[1].designation , livre->design );
            strcpy( pretSituation[1].auteur , livre->auteur );
            strcpy( pretSituation[1].dateEdition , livre->date );
        }
        else if( !strcmp( livre->numero , pretSituation[2].numeroLivre ) )
        {
            strcpy( pretSituation[2].designation , livre->design );
            strcpy( pretSituation[2].auteur , livre->auteur );
            strcpy( pretSituation[2].dateEdition , livre->date );
        }
    }

    delete livre;
    fclose( monFichierLivre );

    int *j( new int( 0 ) );

    while( *j < *i )
    {
        insererItemPret( w , pretSituation , j );

        ++(*j);
    }

    delete i;
    delete j;

    fclose( monFichierPret );
}

void MainWindow::insererItemPret(MainWindow *w , const situation *pretSituation , int *j )
{
    w->ui->tablePret1->insertRow( w->ui->tablePret1->rowCount() );

    QTableWidgetItem *itemNumero( new QTableWidgetItem( QString( pretSituation[*j].numeroLivre ) ) );
    w->ui->tablePret1->setItem( *j , 0 , itemNumero );

    QTableWidgetItem *itemDesignation( new QTableWidgetItem( QString( pretSituation[*j].designation ) ) );
    w->ui->tablePret1->setItem( *j , 1 , itemDesignation );

    QTableWidgetItem *itemAuteur( new QTableWidgetItem( QString( pretSituation[*j].auteur ) ) );
    w->ui->tablePret1->setItem( *j , 2 , itemAuteur );

    QTableWidgetItem *itemDateEdition( new QTableWidgetItem( QString( pretSituation[*j].dateEdition ) ) );
    w->ui->tablePret1->setItem( *j , 3 , itemDateEdition );

    QTableWidgetItem *itemDatePret( new QTableWidgetItem( QString( pretSituation[*j].datePret ) ) );
    w->ui->tablePret1->setItem( *j , 4 , itemDatePret );
}

void MainWindow::reinitialiserTable( MainWindow *w , const type &Table )
{
    if( Table == type::lecteur )
        for( int i( 0 ) ; i < w->ui->tableLecteurAjout->rowCount() ; )
        {
            w->ui->tableLecteurAjout->removeRow( i );
            w->ui->tableLecteurModifier->removeRow( i );
            w->ui->tableLecteurSupprimer->removeRow( i );
        }
    else if( Table == type::livre )
        for( int i( 0 ) ; i < w->ui->tableLivreAjout->rowCount() ; )
        {
            w->ui->tableLivreAjout->removeRow( i );
            w->ui->tableLivreModifier->removeRow( i );
            w->ui->tableLivreSupprimer->removeRow( i );
        }
    else
        for( int i( 0 ) ; i < w->ui->tablePret1->rowCount() ; )
            w->ui->tablePret1->removeRow( i );
}


void MainWindow::on_boutonLecteurPrincipale_clicked()
{
    ui->stackedWidget->setCurrentIndex( 2 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_boutonLivrePrincipale_clicked()
{
    ui->stackedWidget->setCurrentIndex( 1 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_boutonPretPrincipale_clicked()
{
    ui->stackedWidget->setCurrentIndex( 3 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonBilanPrincipale_clicked()
{
    ui->stackedWidget->setCurrentIndex( 4 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonQuitter_clicked()
{
    if( QMessageBox:: question( this , "Exit" ,"Êtes-vous sur de vouloir quitter?", QMessageBox::Yes | QMessageBox::No , QMessageBox::No ) == QMessageBox::Yes )
        QApplication::quit();
}

void MainWindow::on_boutonAjoutLecteur_clicked()
{
     ui->stackedWidget_3->setCurrentIndex( 1 );
     setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonModifierLecteur_clicked()
{
    ui->stackedWidget_3->setCurrentIndex( 2 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonSupprimerLecteur_clicked()
{
    ui->stackedWidget_3->setCurrentIndex( 3 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonRetourLecteur_clicked()
{
    ui->stackedWidget->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_retourLecteurAjout_clicked()
{
    ui->stackedWidget_3->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_retourLecteurModifier_clicked()
{
    ui->stackedWidget_3->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_retourLecteurSupprimer_clicked()
{
    ui->stackedWidget_3->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_boutonAjoutLivre_clicked()
{
    ui->stackedWidget_4->setCurrentIndex( 1 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonModifierLivre_clicked()
{
    ui->stackedWidget_4->setCurrentIndex( 2 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonSupprimerLivre_clicked()
{
    ui->stackedWidget_4->setCurrentIndex( 3 );
    setFixedSize( 1200 , 900 );
}

void MainWindow::on_boutonRetourLivre_clicked()
{
    ui->stackedWidget->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_retourLivreAjout_clicked()
{
    ui->stackedWidget_4->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_retourLivreModifier_clicked()
{
    ui->stackedWidget_4->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_retourLivreSupprimer_clicked()
{
    ui->stackedWidget_4->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}

void MainWindow::on_boutonSituationPret_clicked()
{
    ui->stackedWidget_2->setCurrentIndex( 0 );
}

void MainWindow::on_boutonAjoutPret_clicked()
{
    ui->stackedWidget_2->setCurrentIndex( 1 );
}

void MainWindow::on_boutonModificationPret_clicked()
{
    ui->stackedWidget_2->setCurrentIndex( 2 );
}

void MainWindow::on_boutonSuppressionPret_clicked()
{
    ui->stackedWidget_2->setCurrentIndex( 3 );
}

void MainWindow::on_boutonRetourPret_clicked()
{
    ui->stackedWidget->setCurrentIndex( 0 );
    setFixedSize( 800 , 600 );
}



void MainWindow::on_ajouterLecteur_clicked()
{
    if( ui->numeroLecteurAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du lecteur à ajouter!!" );
        ui->numeroLecteurAjout->setFocus();
        return;
    }
    else if( ui->nomLecteurAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le nom du lecteur à ajouter!!" );
        ui->nomLecteurAjout->setFocus();
        return;
    }

    ajouterLecteur( ui->numeroLecteurAjout->text().toUtf8() , ui->nomLecteurAjout->text().toUtf8() );

    afficherTableLecteur( this );

    ui->numeroLecteurAjout->clear();
    ui->nomLecteurAjout->clear();
}

void MainWindow::on_modifierLecteur_clicked()
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "a" ) );
    if( !nonVide( monFichierLecteur ) )
    {
        fclose( monFichierLecteur );
        QMessageBox::information( this , "Information" ,
                                  "Il n'y a actuellement aucun lecteur dans la bibliothèque" );
        return;
    }

    fclose( monFichierLecteur );

        //  VERIFICATION DES CHAMPS NECESSAIRES
    if( ui->numeroLecteurModifier->text().isEmpty() || ui->nomLecteurModifier->text().isEmpty() )
    {
        QMessageBox::information( this , "Information" ,
                                  "Veuillez donner le numero du lecteur à modifier ainsi que le nouveau nom" );
        ui->numeroLecteurModifier->setFocus();
        return;
    }

    modifierLecteur( ui->numeroLecteurModifier->text().toUtf8() , ui->nomLecteurModifier->text().toUtf8() );

    afficherTableLecteur( this );

    ui->numeroLecteurModifier->clear();
    ui->nomLecteurModifier->clear();

}

void MainWindow::on_supprimerLecteur_clicked()
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "a" ) );
    if( !nonVide( monFichierLecteur ) )
    {
        fclose( monFichierLecteur );
        QMessageBox::information( this , "Information" , "Il n'y a actuellement aucun lecteur dans la bibliothèque" );
        return;
    }

    fclose( monFichierLecteur );

    if( ui->numeroLecteurSupprimer->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du lecteur à supprimer!!");
        ui->numeroLecteurSupprimer->setFocus();
        return;
    }

    supprimerLecteur(  ui->numeroLecteurSupprimer->text().toUtf8() );

    afficherTableLecteur( this );

    ui->numeroLecteurSupprimer->clear();

}


void MainWindow::on_ajouterLivre_clicked()
{
    if( ui->numeroLivreAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du livre à ajouter s'il vous plaît");
        ui->numeroLivreAjout->setFocus();
        return;
    }
    else if( ui->designLivreAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le design du livre à ajouter s'il vous plaît");
        ui->designLivreAjout->setFocus();
        return;
    }
    else if( ui->auteurLivreAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner l'auteur du livre à ajouter s'il vous plaît");
        ui->auteurLivreAjout->setFocus();
        return;
    }

    char dateLivre[11];
    dateFormatage( dateLivre , ui->dateLivreAjout->date() );

    ajouterLivre( ui->numeroLivreAjout->text().toUtf8() , ui->designLivreAjout->text().toUtf8() , ui->auteurLivreAjout->text().toUtf8() , dateLivre );

    afficherTableLivre( this );

    ui->numeroLivreAjout->clear();
    ui->designLivreAjout->clear();
    ui->auteurLivreAjout->clear();

}

void MainWindow::on_modifierLivre_clicked()
{
    FILE *monFichierLivre( fopen( livreChemin , "a" ) );
    if( !nonVide( monFichierLivre ) )
    {
        fclose( monFichierLivre );
        QMessageBox::information( this , "Information" , "Il n'y a actuellement aucun livre dans la bibliothèque" );
        return;
    }

    fclose( monFichierLivre );

    if( ui->numeroLivreModifier->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du livre à modifier!!");
        ui->numeroLivreModifier->setFocus();
        return;
    }

    int *nombrePretLivre( new int( -1 ) );
    disponibilite *dispo( new disponibilite( Oui ) );

    char dateLivre[11];
    dateFormatage( dateLivre , ui->dateLivreModifier->date() );

    modifierLivre( ui->numeroLivreModifier->text().toUtf8() , ui->designLivreModifier->text().toUtf8() , ui->auteurLivreModifier->text().toUtf8() , dateLivre , *dispo , *nombrePretLivre );

    delete nombrePretLivre;
    delete dispo;

    afficherTableLivre( this );

    ui->numeroLivreModifier->clear();
    ui->designLivreModifier->clear();
    ui->auteurLivreModifier->clear();

}

void MainWindow::on_supprimerLivre_clicked()
{
    FILE *monFichierLivre( fopen( livreChemin , "a" ) );
    if( !nonVide( monFichierLivre ) )
    {
        fclose( monFichierLivre );
        QMessageBox::information( this , "Information" , "Il n'y a actuellement aucun livre dans la bibliothèque" );
        return;
    }
    fclose( monFichierLivre );

    if( ui->numeroLivreSupprimer->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du livre à supprimer!!");
        ui->numeroLivreSupprimer->setFocus();
        return;
    }

    supprimerLivre( ui->numeroLivreSupprimer->text().toUtf8() );

    afficherTableLivre( this );

    ui->numeroLivreModifier->clear();

}


void MainWindow::on_afficherPret_clicked()
{
    char nomLecteur[50];
    strcpy( nomLecteur , ui->nomLecteurPretSituation->text().toUtf8() );
    afficherTablePret( this , ui->numeroLecteurPretSituation->text().toUtf8() , nomLecteur , true );
}

void MainWindow::on_ajouterPret_clicked()
{
    FILE *monFichierLecteur( fopen( lecteurChemin , "r" ) ) , *monFichierLivre( fopen( livreChemin , "r" ) );
    if( !nonVide( monFichierLecteur ) )
    {
        fclose( monFichierLivre );
        QMessageBox::information( this , "Information" , "Il n'y a actuellement aucun membre dans la bibliothèque" );
        return;
    }
    else if( !nonVide( monFichierLivre ) )
    {
        fclose( monFichierLecteur );
        QMessageBox::information( this , "Information" , "Il n'y a actuellement aucun livre dans la bibliothèque" );
        return;
    }

    fclose( monFichierLecteur );
    fclose( monFichierLivre );

    if( ui->numeroLecteurPretAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du lecteur qui veut prêter un livre!!");
        ui->numeroLecteurPretAjout->setFocus();
        return;
    }
    else if( ui->numeroLivrePretAjout->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du livre à prêter!!");
        ui->numeroLivrePretAjout->setFocus();
        return;
    }

    ajouterPret( ui->numeroLecteurPretAjout->text().toUtf8() , ui->numeroLivrePretAjout->text().toUtf8() );

    char nomLecteur[50] = "";

    afficherTablePret( this , ui->numeroLecteurPretAjout->text().toUtf8() , nomLecteur );

    ui->numeroLecteurPretAjout->clear();
    ui->numeroLivrePretAjout->clear();

}

void MainWindow::on_modifierPret_clicked()
{
    FILE *monFichierPret( fopen( pretChemin , "a" ) );
    if( !nonVide( monFichierPret ) )
    {
        fclose( monFichierPret );
        QMessageBox::information( this , "Information" , "Aucun livre n'est actuellement prêté" );
        return;
    }
    fclose( monFichierPret );

    if( ui->numeroLecteurPret->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du lecteur qui prête un livre!!");
        ui->numeroLecteurPretModifier->setFocus();
        return;
    }

    if( ui->numeroPretModifier->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du prêt du lecteur!!");
        ui->numeroPretModifier->setFocus();
        return;
    }

    char datePret[11];
    dateFormatage( datePret , ui->datePretModifier->date() );

    modifierPret( ui->numeroLecteurPretModifier->text().toUtf8() , ui->numeroPretModifier->text().toUtf8() , datePret );

    char nomLecteur[50] = "";

    afficherTablePret( this , ui->numeroLecteurPretModifier->text().toUtf8() , nomLecteur );

    ui->numeroLecteurPretModifier->clear();
    ui->numeroPretModifier->clear();

}

void MainWindow::on_supprimerPret_clicked()
{
    FILE *monFichierPret( fopen( pretChemin , "a" ) );
    if( !nonVide( monFichierPret ) )
    {
        fclose( monFichierPret );
        QMessageBox::information( this , "Information" , "Aucun livre n'est actuellement prêté" );
        return;
    }
    fclose( monFichierPret );

    if( ui->numeroLivrePretSupprimer->text().isEmpty() )
    {
        QMessageBox::information( this , "Information!!!" , "Veuillez donner le numero du livre à rendre à la bibliothèque!!");
        ui->numeroLivrePretSupprimer->setFocus();
        return;
    }

    char numeroLecteur[5] = "" , nomLecteur[50] = "";

    supprimerPret( numeroLecteur , ui->numeroLivrePretSupprimer->text().toUtf8() );

    afficherTablePret( this , numeroLecteur , nomLecteur );

    ui->numeroLivrePretSupprimer->clear();

}
