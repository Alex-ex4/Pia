#ifndef UTILIDADES_H
#define UTILIDADES_H

typedef struct {
    int idZona;
    float temperatura;
    int estado_ventilador; // 1 para ON, 0 para OFF, me sirve mas que dejarlo como char asi que lo dejo asi
    time_t hora; //time_t es un tipo de dato para almacenar tiempo para que no se confundan plebes
} Historial;

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



void escribirArchivo(Zona *zonas, int cont);

void agregarArchivo(Zona zona);

void listaZonas(); 

float generar_temperatura_aleatoria();

float generarTemp(int min, int max);

void registrarZona(Zona **zonas, int *cont, float **tempPredet, int **contHistorial);

void temperaturaActual( Zona **zonas, int *cont);

void limpiar_buffer();

int validar_usuario();

void activarVent(Zona **zonas, int *cont);

void reporte(Zona **zonas, int *cont, int **contHistorial);

void historial_por_zona();
//void leer_cadena(const char* mensaje, char* cadena, int longitud);

void buscar_eventos_rango();

Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado);

void actualizarVentilador( Zona *z);

void temperaturaActual( Zona **zonas, int *cont);

void cambiarUmbral(Zona *arr, int cont);

void registrar_evento(int id_zona, float temperatura, int estado_ventilador);

void mostrar_historial_zona(int id_zona);

void buscar_eventos_rango();

Zona* buscar_zona_por_id(int id_zona, Zona *zona);

void simular_monitoreo_tiempo_real();

void restaurar_configuracion_default(Zona **zonas, int *cont, float **tempPredet);


#endif
