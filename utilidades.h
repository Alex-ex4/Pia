#ifndef UTILIDADES_H
#define UTILIDADES_H


typedef struct {
    int id;
    char nom[50];
    float umbral;
    char ventilador[5];
} Zona;

typedef struct {
    char usuario[50];
    char password[50];
} Usuario;

void escribirArchivo(Zona *zonas, int cont);

void agregarArchivo(Zona zona);

void listaZonas(); 

float generarTemp(int min, int max);

void registrarZona(Zona **zonas, int *cont);

void temperaturaActual( Zona **zonas, int *cont);

void limpiar_buffer();

int validar_usuario();

//void leer_cadena(const char* mensaje, char* cadena, int longitud);

#endif