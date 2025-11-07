#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilidades.h"
#define ARCHIVO_USUARIOS "usuarios.bin"
//#define MAX_NOMBRE 50
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"

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
    printf("Ingrese usuario: ");
    fgets(usuario, 50, stdin);
    usuario[strcspn(usuario, "\n")] = '\0';

    //leer_cadena("", usuario, 50);
    printf("Ingrese contraseña: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = '\0';
    //leer_cadena("", password, 50);

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

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*int leer_entero(const char* mensaje) {
    int valor;
    printf("%s", mensaje);
    while (scanf("%d", &valor) != 1) {
        printf("Error: Ingrese un número entero válido: ");
        limpiar_buffer();
    }
    limpiar_buffer();
    return valor;
}

float leer_flotante(const char* mensaje) {
    float valor;
    printf("%s", mensaje);
    while (scanf("%f", &valor) != 1) {
        printf("Error: Ingrese un número válido: ");
        limpiar_buffer();
    }
    limpiar_buffer();
    return valor;
}*/

/*void leer_cadena(const char* mensaje, char* cadena, int longitud) {
    printf("%s", mensaje);
    if (fgets(cadena, longitud, stdin) != NULL) {
        cadena[strcspn(cadena, "\n")] = '\0';
    }
}*/


Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado) {
    for(int i = 0; i < cont ; i++) {
        if(strcmp(arr[i].nom, nombreBuscado) == 0) {
            return &arr[i];
        }
    }
    return NULL;
}

int posBuscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado) {
    for(int i = 0; i < cont ; i++) {
        if(strcmp(arr[i].nom, nombreBuscado) == 0) {
            return i;
        }
    }
    return -1;
}


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
        printf("ID: %d  Nombre: %s  Umbral: %.2f  Ventilador: %s\n",
                zona.id, zona.nom, zona.umbral, zona.ventilador);
    }
    fclose(archivo);
}


float generarTemp(int min, int max){
    srand(time(NULL));
    float temp=min+rand()%(max-min+1);
    return temp;
}

void registrarZona(Zona **zonas, int *cont, float **tempPredet, int **contHistorial) {
    Zona nuevaZona;
    printf("Ingrese ID de la zona: ");
    scanf("%d", &nuevaZona.id);
    getchar();

    printf("Ingrese nombre de la zona: ");
    fgets(nuevaZona.nom, sizeof(nuevaZona.nom), stdin);
    nuevaZona.nom[strcspn(nuevaZona.nom, "\n")] = '\0';

    printf("Ingrese umbral de temperatura: ");
    scanf("%f", &nuevaZona.umbral);

    nuevaZona.historiales =(Historial *) malloc(1*sizeof(Historial));
    nuevaZona.historiales[0].idZona = nuevaZona.id;
    nuevaZona.historiales[0].temperatura = nuevaZona.umbral-5;
    nuevaZona.historiales[0].fecha = '30/06/2025';
    nuevaZona.historiales[0].hora = '12:00';
    strcpy(nuevaZona.historiales[0].ventilador, "OFF");

    (*cont)++;
    *zonas=realloc(*zonas, ((*cont))*sizeof(Zona));
    *zonas[(*cont) - 1] = nuevaZona;

    *tempPredet=realloc(*tempPredet, ((*cont))*sizeof(float));
    (*tempPredet)[(*cont) - 1] = nuevaZona.umbral;

    *contHistorial=realloc(*contHistorial, ((*cont))*sizeof(int));
    (*contHistorial)[(*cont) - 1] = 1;

    agregarArchivo(nuevaZona);
}


void temperaturaActual( Zona **zonas, int *cont) {
    int i;
    if (zonas == NULL || *zonas == NULL || cont == NULL) return;
    Zona *arr = *zonas;
    //posible cambio de la linea  62-66 (leer en documentacion)
    for(i = 0; i < (*cont) ; i++) {
        int min = (int)(arr[i].umbral - 5);
        int max = (int)(arr[i].umbral + 5);
        float temp = generarTemp(min, max);
        if (temp > arr[i].umbral) strcpy(arr[i].ventilador, "ON");
        else strcpy(arr[i].ventilador, "OFF");

        printf("Zona: %s\n", arr[i].nom);
        printf("Temperatura actual: %.2f °C\nEstado del Ventilador: %s\n\n",
        temp, arr[i].ventilador);
    }
}

void activarVent(Zona **zonas, int *cont, int **contHistorial){
    char nombreZona[50];
    int opcion;
    printf("Ingresa el nombre de la zona: ");
    fgets(nombreZona, sizeof(nombreZona), stdin);
    nombreZona[strcspn(nombreZona, "\n")] = '\0';
    Zona *zonaEncontrada = buscarZonaPorNombre(*zonas, *cont, nombreZona);
    int pos=posBuscarZonaPorNombre(*zonas, *cont, nombreZona);
    if(zonaEncontrada == NULL|| pos==-1){ {
        printf("Zona no encontrada.\n");
        return;
    }
    Historial nuevoHistorial= zonaEncontrada->historiales[(*contHistorial)[pos]];
    printf("Seleccione acción: \n");
    printf("1. Encender ventilador\n"); 
    printf("2. Apagar ventilador\n");   
    scanf("%d", &opcion);
    if(opcion==1){
        (*contHistorial)[pos]++;
        zonaEncontrada->historiales= realloc(zonaEncontrada->historiales, (*contHistorial)[pos]*sizeof(Historial));
        zonaEncontrada->historiales[(*contHistorial)[pos]]=nuevoHistorial;
        strcpy(zonaEncontrada->historiales[(*contHistorial)[pos]], "ON");
        printf("Ventilador encendido manualmente.\n");
    } else if(opcion==2){
        (*contHistorial)[pos]++;
        zonaEncontrada->historiales= realloc(zonaEncontrada->historiales, (*contHistorial)[pos]*sizeof(Historial));
        zonaEncontrada->historiales[(*contHistorial)[pos]]=nuevoHistorial;
        strcpy(zonaEncontrada->historiales[(*contHistorial)[pos]], "OFF");
        printf("Ventilador apagado manualmente.\n");
    } else {
        printf("Opción no válida.\n");
    }
    }
}

void reporte(Zona **zonas, int *cont, int **contHistorial){
    float suma=0.0,prom, tempmax=0.0, tempmin=100.0;
    for(int i=0; i<(*cont); i++){
        for(int j=0; j<*(contHistorial+i); j++){
            suma+=(*zonas)[i].historiales[j].temperatura;
            if ((*zonas)[i].historiales[j].temperatura>tempmax){
                tempmax=(*zonas)[i].historiales[j].temperatura;
            }
            if((*zonas)[i].historiales[j].temperatura<tempmin){
                tempmin=(*zonas)[i].historiales[j].temperatura;
            }
        }
        prom=suma/(*(contHistorial+i));
        printf("Zona: %s\n", (*zonas)[i].nom);
        printf("Reporte estadistico:\n");
        printf("Temperatura maxima: %.1f °C\n", tempmax);
        printf("Temperatura minima: %.1f °C\n", tempmin);
        printf("Temperatura promedio: %.1f °C\n\n", prom); 
    }
}
