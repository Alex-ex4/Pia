#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "utilidades.h"
#include "validaciones.h"
#include "archivos.h"
#define ARCHIVO_USUARIOS "usuarios.bin"
//#define MAX_NOMBRE 50
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"

void escribirArchivo(Zona *zonas, int cont) {
    FILE *archivo = fopen(ARCHIVO, "wb");
    if (archivo != NULL) {
        fwrite(zonas, sizeof(Zona), cont, archivo);
        fclose(archivo);
        printf("Archivo actualizado exitosamente.\n");
    } else {
        printf("Error al abrir el archivo para escribir.\n");
    }
}

void agregarArchivo(Zona zona) {
    FILE *archivo = fopen(ARCHIVO, "ab");
    if (archivo != NULL) {
        fwrite(&zona, sizeof(Zona), 1, archivo);
        fclose(archivo);
        printf("Zona agregada al archivo exitosamente.\n");
    } else {
        printf("Error al abrir el archivo para agregar la zona.\n");
    }
}