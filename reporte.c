#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilidades.h"
#include "validaciones.h"
#include "archivos.h"
#define ARCHIVO_USUARIOS "usuarios.bin"
//#define MAX_NOMBRE 50
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"


void reporte(Zona **zonas, int *cont) {
    float tempmax_T=0.0, tempmin_T=100.0, prom_T, suma_T=0.0;
    char zona_max[50], zona_min[50];
    int num_eventos=0;
    for(int i=0; i<(*cont); i++){
        float suma=0.0,prom, tempmax=0.0, tempmin=100.0;
        for(int j=0; j<(*zonas)[i].cont_historial; j++){
            suma+=(*zonas)[i].historiales[j].temperatura;
            if ((*zonas)[i].historiales[j].temperatura>tempmax){
                tempmax=(*zonas)[i].historiales[j].temperatura;
            }
            if((*zonas)[i].historiales[j].temperatura<tempmin){
                tempmin=(*zonas)[i].historiales[j].temperatura;
            }

            if((*zonas)[i].historiales[j].temperatura>tempmax_T){
                tempmax_T=(*zonas)[i].historiales[j].temperatura;
                strcpy(zona_max, (*zonas)[i].nom);
            }
            if((*zonas)[i].historiales[j].temperatura<tempmin_T){
                tempmin_T=(*zonas)[i].historiales[j].temperatura;
                strcpy(zona_min, (*zonas)[i].nom);
            }
            suma_T+=(*zonas)[i].historiales[j].temperatura;
            num_eventos++;
        }
        prom=suma/((*zonas)[i].cont_historial); 
        printf("Zona: %s\n", (*zonas)[i].nom);
        printf("Reporte estadistico:\n");
        printf("Temperatura maxima: %.2f °C\n", tempmax);
        printf("Temperatura minima: %.2f °C\n", tempmin);
        printf("Temperatura promedio: %.2f °C\n\n", prom); 
    }
    prom_T=suma_T/num_eventos;
    printf("\nReporte general:\n");
    printf("Zona con temperatura maxima: %s (%.2f °C)\n", zona_max, tempmax_T);
    printf("Zona con temperatura minima: %s (%.2f °C)\n", zona_min, tempmin_T);
    printf("Temperatura promedio: %.2f °C\n\n", prom_T); 

}


void exportar_historial_csv(Zona **zonas, int *cont) {
    FILE *archivo_csv = fopen(HISTORIAL, "w");
    if(archivo_csv==NULL){
        printf("Error al crear el archivo CSV.\n");
        return;
    }
    fprintf(archivo_csv, "ID Zona,Fecha,Hora,Temperatura,Estado Ventilador\n");
    for(int i=0;i<*cont;i++){
        for(int j=0;j<(*zonas)[i].cont_historial;j++){
            struct tm* tiempo = localtime(&(*zonas)[i].historiales[j].hora);
            fprintf(archivo_csv, "%d,[%02d/%02d],[%02d:%02d:%02d],%.2f,%s\n",
                    (*zonas)[i].historiales[j].idZona,
                    tiempo->tm_mday, tiempo->tm_mon + 1,
                    tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec,
                    (*zonas)[i].historiales[j].temperatura,
                    (*zonas)[i].historiales[j].estado_ventilador ? "ON" : "OFF");
        }
    }
    fclose(archivo_csv);
    printf("Historial exportado exitosamente a %s\n", HISTORIAL);
}
