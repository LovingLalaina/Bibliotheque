#ifndef LIVRE_H
#define LIVRE_H

#include "mainwindow.h"
#include "fonctionsecondaire.h"

void ajouterLivre( const char * , const char * , const char * , const char * );

void supprimerLivre( const char * );

void modifierLivre( const char * , const char * , const char * , const char * , disponibilite & , int & );


void lireLivre( FILE * , livreStruct * );

void ecrireLivre( FILE * , const livreStruct * );

void actualiserFichierLivre();

bool verifierLivre( const livreStruct * );

#endif // LIVRE_H
