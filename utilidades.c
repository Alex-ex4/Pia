#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilidades.h"
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"


float generarTemp(int min, int max){
    srand(time(NULL));
    float temp=min+rand()%(max-min+1);
    return temp;
}

void registrarZona(Zona **zonas, int *cont){
    Zona nuevaZona;
    printf("Ingrese ID de la zona: ");
    scanf("%d", &nuevaZona.id);
    printf("Ingrese nombre de la zona: ");
    fgets(nuevaZona.nom, sizeof(nuevaZona.nom), stdin);
    nuevaZona.nom[strcspn(nuevaZona.nom, "\n")] = '\0';
    printf("Ingrese umbral de temperatura: ");
    scanf("%f", &nuevaZona.umbral);
    strcpy(nuevaZona.ventilador, "OFF");

    (*cont)++;
    *zonas=realloc(*zonas, ((*cont)+1)*sizeof(Zona));
    *zonas[(*cont) - 1] = nuevaZona;

    FILE *archivo = fopen(ARCHIVO, "ab");
    if (archivo != NULL) {
        fwrite(&nuevaZona, sizeof(Zona), 1, archivo);
        fclose(archivo);
        printf("Zona registrada exitosamente.\n");
    } else {
        printf("Error al abrir el archivo para registrar la zona.\n");
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
        printf("ID: %d  Nombre: %s  Umbral: %.2f  Ventilador: %s\n",
                zona.id, zona.nom, zona.umbral, zona.ventilador);
    }
    fclose(archivo);
}


// Edson 
Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado) {
    for(int i = 0; i < cont ; i++) {
        if(strcmp(arr[i].nom, nombreBuscado) == 0) {
            return &arr[i];
        }
    }
    return NULL;
}

// Edson
void actualizarVentilador( Zona *z) {
    int min = (int)(z->umbral - 4);
    int max = (int)(z->umbral + 4);
    float temp = generarTemp(min, max);
    if ( temp > z->umbral ) 
        strcpy(z->ventilador, "ON");
            else 
        strcpy(z->ventilador, "OFF");

    printf("Zona: %s\n", z->nom);
    printf("Temperatura actual: %.2f °C\nEstado del Ventilador: %s\n\n",
            temp, z->ventilador);
}

// Edson 
void temperaturaActual( Zona **zonas, int *cont) {
    if (zonas == NULL || *zonas == NULL || cont == NULL) return;

    int i, opcion;
    char nombre[50];
    Zona *arr = *zonas;

    do 
    {
        printf("\n1) Zona especifica por nombre.\n2) LIstar todas las zonas\n3)  Volver");
        printf("Seleccion: ");  
        scanf("%d", &opcion);
        
        switch (opcion)
        {
        case 1: 
        {
            printf("\tIngresar el nombre de la zona:");
            scanf("%49s", nombre);
            
            Zona *z = buscarZonaPorNombre(arr, *cont, nombre );
            
            if( z ) 
            {
                printf("Zona encontrada\n");
                actualizarVentilador(z);
            } else {
                printf("Zona no encontrada");
            }
            break;
        }
        case 2:
            for(i = 0; i < (*cont) ; i++) 
            {
                actualizarVentilador( &arr[i]);
            }
        default:
        printf("Opcion invalida...");
            break;
        }
    }while(opcion != 3);
}

// Edson
//Integracion: cambiarUmbral(arr,*cont);
void cambiarUmbral(Zona *arr, int cont) {
    if ( arr == NULL || cont <= 0) return;

    char nombre[50];
    int nuevoUmbral;
    printf("\tIngresar el nombre de la zona:");
    scanf("%49s", nombre);

    Zona *z = buscarZonaPorNombre( arr , cont , nombre );
    if( z == NULL) {
        printf("Zona no encontrada");
        return;
    }
    printf("> Zona _%s_ encontrada\n", z->nom);
    printf("\n\tUmbral actual: %d °C", z->umbral);
    printf("\n\tIngrese el nuevo valor del umbral: ");
    scanf("%d", &nuevoUmbral);

    z->umbral = nuevoUmbral;
    printf("> Umbral aztualizado correctamente.\n");
    printf("\n\tUmbral actual: %d °C", z->umbral);
    actualizarVentilador(z);
}   
