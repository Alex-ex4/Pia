#ifndef UTILIDADES_H
#define UTILIDADES_H


typedef struct {
    int id;
    char nom[50];
    float umbral;
    char ventilador[5];
    Historial *historiales;
} Zona;

typedef struct {
    char usuario[50];
    char password[50];
} Usuario;

typedef struct {
    int idZona;
    float temperatura;
    char fecha[20];
    char hora[10];
} Historial;

void escribirArchivo(Zona *zonas, int cont);

void agregarArchivo(Zona zona);

void listaZonas(); 

float generarTemp(int min, int max);

void registrarZona(Zona **zonas, int *cont, float **tempPredet, int **contHistorial);

void temperaturaActual( Zona **zonas, int *cont);

void limpiar_buffer();

int validar_usuario();

void activarVent(Zona **zonas, int *cont);

void reporte(Zona **zonas, int *cont, int **contHistorial);

//void leer_cadena(const char* mensaje, char* cadena, int longitud);

#endif
