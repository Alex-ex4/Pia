#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int validarEsNumero( char *cadena); 

int validaEsVacio( char *cadena ); 

int validarLongitud( char *cadena, int minimo, int maximo ); 

int validarRango( int numero, int minimo, int maximo );


#endif