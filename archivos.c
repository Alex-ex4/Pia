#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilidades.h"
#include "validaciones.h"
#include "archivos.h"
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"

void escribirArchivo(Zona *zonas, int cont){
    FILE *archivo = fopen(ARCHIVO, "wb");
    if (archivo != NULL) {
        fwrite(zonas, sizeof(Zona), cont, archivo);
        fclose(archivo);
        printf("Archivo actualizado exitosamente.\n");
    } else {
        printf("Error al abrir el archivo para escribir.\n");
    }
}

void agregarArchivo(Zona zona){
    FILE *archivo = fopen(ARCHIVO, "ab");
    if (archivo != NULL) {
        fwrite(&zona, sizeof(Zona), 1, archivo);
        fclose(archivo);
        printf("Zona agregada al archivo exitosamente.\n");
    } else {
        printf("Error al abrir el archivo para agregar la zona.\n");
    }
}

void listaZonas(){
    FILE *archivo = fopen(ARCHIVO, "rb");
    if (!archivo) {
        printf("No hay zonas registradas.\n");
        return;
    }
    Zona zona;
    printf("Zonas registradas:\n");
    while (fread(&zona, sizeof(Zona), 1, archivo) == 1) {
        printf("\nID: %d \nNombre: %s \nUmbral: %.2f \nVentilador: %s\n",
                zona.id, zona.nom, zona.umbral, zona.ventilador);
    }
    fclose(archivo);
}

void cargarZonasDesdeArchivo(Zona **zonas, int *cont) {
    FILE *archivo = fopen(ARCHIVO, "rb");
    
    if (archivo == NULL) {
        printf("Iniciando sin zonas precargadas.\n");
        *cont = 0;
        *zonas = NULL; 
        return;
    }

    Zona zonaLeida;
    int contador_cargado = 0;
    
    while (fread(&zonaLeida, sizeof(Zona), 1, archivo) == 1) {
        
        zonaLeida.historiales = NULL;
        zonaLeida.cont_historial = 0; 

        contador_cargado++;
        *zonas = (Zona *)realloc(*zonas, contador_cargado * sizeof(Zona));

        if (*zonas == NULL) {
            fclose(archivo);
            exit(EXIT_FAILURE);
        }
        
        (*zonas)[contador_cargado - 1] = zonaLeida;
    }

    *cont = contador_cargado;
    fclose(archivo);
    
    printf("Carga de zonas completada: %d zonas recuperadas.\n", *cont);
}


void cargarHistorialDesdeArchivo(Zona *zonas, int cont) {
    if (cont == 0) return; 

    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf("El historial se cargará vacío.\n");
        return;
    }

    Historial eventoLeido;
    int eventos_cargados = 0;


    while (fread(&eventoLeido, sizeof(Historial), 1, archivo) == 1) {
        Zona *zona_destino = NULL;
        for (int i = 0; i < cont; i++) {
            if (zonas[i].id == eventoLeido.idZona) {
                zona_destino = &zonas[i];
                break;
            }
        }
        
        if (zona_destino != NULL) {
            zona_destino->cont_historial++;
            zona_destino->historiales = (Historial *)realloc(zona_destino->historiales, zona_destino->cont_historial * sizeof(Historial));
            
            if (zona_destino->historiales == NULL) {
                exit(EXIT_FAILURE); 
            }

            zona_destino->historiales[zona_destino->cont_historial - 1] = eventoLeido;
            eventos_cargados++;
        }
    }

    fclose(archivo);
    printf("Reconstrucción de historial completada: %d eventos cargados.\n", eventos_cargados);
}