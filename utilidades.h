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

float generarTemp(int min, int max);

void registrarZona(Zona **zonas, int *cont);

void limpiar_buffer();

int validar_usuario();
//void leer_cadena(const char* mensaje, char* cadena, int longitud);

//Edson

void listaZonas(); 

Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado);

void actualizarVentilador( Zona *z);

void temperaturaActual( Zona **zonas, int *cont);

void cambiarUmbral(Zona *arr, int cont);



#endif