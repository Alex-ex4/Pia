#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "validaciones.h"



void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// al querer solo  N cantidad de caracteres  
// -> scanf("%4s",cadena); aqui es 4
int validarEsNumero( char *cadena) {
    int i;
    for (i = 0 ; cadena[i] != '\0' ; i++) {
        if(!isdigit(cadena[i])) {
            return 0;
        }
    }
    return 1; //Exito
}


int validaEsVacio( char *cadena ) {
    int i;
    for ( i = 0 ; cadena[i] != '\0' ; i++ ) {
        if( !isspace(cadena[i])) {
            return 0;
        }
    }
    return 1;
}


int validarLongitud( char *cadena, int minimo, int maximo ) {
    int longitud = strlen(cadena);
    return ( longitud >= minimo && longitud <= maximo );
}


int validarRango( int numero, int minimo, int maximo ) {
    return ( numero>= minimo && numero <= maximo );
}


int validarEsFlotante( char *cadena ) 
{
    int i = 0;
    int punto_encontrado = 0;
    int digito_encontrado = 0;
    
    if (cadena[i] == '-' || cadena[i] == '+') {
        i++;
    }
    
    for (; cadena[i] != '\0' ; i++) {
        if (isdigit(cadena[i])) {
            digito_encontrado = 1;
            continue;
        } else if (cadena[i] == '.') {
            if (punto_encontrado) {
                return 0; 
            }
            punto_encontrado = 1;
        } else {
            return 0; 
        }
    }
    return digito_encontrado;
}
