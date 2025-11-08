#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "validaciones.h"


// al querer solo  N cantidad de caracteres  
// -> scanf("%4s",cadena); aqui es 4
int validarEsNumero( char *cadena) 
{
    int i;
    for (i = 0 ; cadena[i] != '\0' ; i++) {
        if(!isdigit(cadena[i])) {
            return 0;
        }
    }
    return 1; //Exito
}


int validaEsVacio( char *cadena ) 
{
    int i;
    for ( i = 0 ; cadena[i] != '\0' ; i++ ) {
        if( !isspace(cadena[i])) {
            return 0;
        }
    }
    return 1;
}


int validarLongitud( char *cadena, int minimo, int maximo ) 
{
    int longitud = strlen(cadena);
    return ( longitud >= minimo && longitud <= maximo );
}


int validarRango( int numero, int minimo, int maximo ) 
{
    return ( numero>= minimo && numero <= maximo );
}


