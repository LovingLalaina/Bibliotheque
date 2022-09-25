#ifndef FONCTIONSECONDAIRE_H
#define FONCTIONSECONDAIRE_H

#include "mainwindow.h"

bool nonVide( FILE * );

void dateFormatage( char * , const QDate & );

const QString conversion( const int & );

const QDate toQDate( const char * );

const QString accessibilite( const disponibilite & );

bool fichierErreur( FILE * , const type & );

#endif // FONCTIONSECONDAIRE_H
