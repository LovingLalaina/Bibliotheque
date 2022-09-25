#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDate>
#include <QPropertyAnimation>

#define lecteurChemin "E:/BibliothequeEMIT/Base_de_Donnees/Lecteur.dat"
#define livreChemin "E:/BibliothequeEMIT/Base_de_Donnees/Livre.dat"
#define pretChemin "E:/BibliothequeEMIT/Base_de_Donnees/Pret.dat"
#define fichierTemporaire "E:/BibliothequeEMIT/Base_de_Donnees/FichierTemporaire.dat"

typedef struct lecteurStruct
{
    char numero[5] , nom[50];

}lecteurStruct;

typedef enum disponibilite
{
    Non = 0,
    Oui = 1

} disponibilite;

typedef struct livreStruct
{
    char numero[5] ,design[25] , auteur[50] , date[11];
    disponibilite disponibilite;
    int nombrePret;

} livreStruct;

typedef struct pretStruct
{
    unsigned int numero;
    char numeroLecteur[5] , numeroLivre[5] , datePret[11] , dateRetour[11];

} pretStruct;

typedef struct situation
{
    char designation[25] , auteur[50] , dateEdition[11] , datePret[11] , numeroLivre[5];

} situation;

typedef enum type
{
    lecteur = 0 ,
    livre = 1 ,
    pret = 2
} type;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow( QWidget *parent = nullptr );

    ~MainWindow();

    static void Creation_BaseDeDonnees();

    static void VerificationPret();

    static void afficherTableLecteur( MainWindow * );

    static void insererItemLecteur( MainWindow * , const lecteurStruct * , int * );

    static void afficherTableLivre( MainWindow * );

    static void insererItemLivre( MainWindow * , const livreStruct * , int * );

    static void afficherTablePret( MainWindow * , const char * , char * , const bool & = false );

    static void insererItemPret( MainWindow * , const situation * , int * );

    static void reinitialiserTable( MainWindow * , const type & );

private slots:

        //  BOUTONS LIENS

    //  PAGE PRINCIPALE
    void on_boutonLecteurPrincipale_clicked();

    void on_boutonLivrePrincipale_clicked();

    void on_boutonPretPrincipale_clicked();

    void on_boutonBilanPrincipale_clicked();

    void on_boutonQuitter_clicked();

    //  PAGE LECTEUR
    void on_boutonAjoutLecteur_clicked();

    void on_boutonModifierLecteur_clicked();

    void on_boutonSupprimerLecteur_clicked();

    void on_boutonRetourLecteur_clicked();

    void on_retourLecteurAjout_clicked();

    void on_retourLecteurModifier_clicked();

    void on_retourLecteurSupprimer_clicked();

    //  PAGE LIRVE
    void on_boutonAjoutLivre_clicked();

    void on_boutonModifierLivre_clicked();

    void on_boutonSupprimerLivre_clicked();

    void on_boutonRetourLivre_clicked();

    void on_retourLivreAjout_clicked();

    void on_retourLivreModifier_clicked();

    void on_retourLivreSupprimer_clicked();

    //  PAGE PRET
    void on_boutonSituationPret_clicked();

    void on_boutonAjoutPret_clicked();

    void on_boutonModificationPret_clicked();

    void on_boutonSuppressionPret_clicked();

    void on_boutonRetourPret_clicked();

    //  BOUTON EXECUTION
    void on_ajouterLecteur_clicked();

    void on_modifierLecteur_clicked();

    void on_supprimerLecteur_clicked();


    void on_ajouterLivre_clicked();

    void on_modifierLivre_clicked();

    void on_supprimerLivre_clicked();


    void on_afficherPret_clicked();

    void on_ajouterPret_clicked();

    void on_modifierPret_clicked();

    void on_supprimerPret_clicked();

private:

    Ui::MainWindow *ui;

    QPropertyAnimation *animation;

};

#endif // MAINWINDOW_H
