#ifndef LECTEUR_H
#define LECTEUR_H

#include "mainwindow.h"

void ajouterLecteur( const char * , const char * );

void supprimerLecteur( const char * );

void modifierLecteur( const char * , const char * );


void lireLecteur( FILE * , lecteurStruct * );

void ecrireLecteur( FILE * , const lecteurStruct * );

void actualiserFichierLecteur();

#endif // LECTEUR_H
