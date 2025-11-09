#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "validaciones.h"

#define ARCHIVO_USUARIOS "usuarios.bin"
//#define MAX_NOMBRE 50
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"


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

int validar_usuario() {
    FILE *archivo = fopen(ARCHIVO_USUARIOS, "rb");
    if (archivo == NULL) {
        archivo = fopen(ARCHIVO_USUARIOS, "wb");
        if (archivo != NULL) {
            Usuario admin = {"Padron", "123456"};
            fwrite(&admin, sizeof(Usuario), 1, archivo);
            fclose(archivo);
        }
    } else {
        fclose(archivo);
    }

    char usuario[50];
    char password[50];

    printf("=== Bienvenido ===\n");
    
    // *** MODIFICADO ***
    // Se usa la nueva función de validación de cadenas
    leer_cadena_validada("Ingrese usuario: ", usuario, sizeof(usuario), 1, 49);
    
    // *** MODIFICADO ***
    // Se usa la nueva función de validación de cadenas
    leer_cadena_validada("Ingrese contraseña: ", password, sizeof(password), 1, 49);

    archivo = fopen(ARCHIVO_USUARIOS, "rb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo de usuarios.\n");
        return 0;
    }

    Usuario user;
    int encontrado = 0;
    while (fread(&user, sizeof(Usuario), 1, archivo)) {
        if (strcmp(user.usuario, usuario) == 0 && strcmp(user.password, password) == 0) {
            encontrado = 1;
            break;
        }
    }
    fclose(archivo);

    if (!encontrado) {
        printf("Error: Usuario o contraseña incorrectos.\n\n");
    }
    return encontrado;
}
