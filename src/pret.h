#ifndef PRET_H
#define PRET_H

#include "mainwindow.h"

void ajouterPret( const char * , const char * );

void supprimerPret( char * , const char * );

void modifierPret( const char * , const char * , const char * );

void lirePret( FILE * , pretStruct * );

void ecrirePret( FILE * , const pretStruct * );

void actualiserFichierPret();

void reOrdonnerNumeroPret( const char * );

#endif // PRET_H
