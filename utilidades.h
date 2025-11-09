#ifndef UTILIDADES_H
#define UTILIDADES_H

typedef struct {
    int idZona;
    float temperatura;
    int estado_ventilador; 
    time_t hora; 
} Historial;

typedef struct {
    int id;
    char nom[50];
    float umbral;
    char ventilador[5];
    Historial *historiales;
    float temp_predet;
    int cont_historial;
} Zona;

typedef struct {
    char usuario[50];
    char password[50];
} Usuario;


float generar_temperatura_aleatoria();
float generarTemp(int min, int max);

Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado);
Zona* buscar_zona_por_id(int id_zona, Zona *zona);

void actualizarVentilador( Zona *z);

void registrar_evento(int id_zona, float temperatura, int estado_ventilador);

int validar_usuario();

void registrarZona(Zona **zonas, int *cont);

void temperaturaActual( Zona **zonas, int *cont);

void activarVent(Zona **zonas, int *cont);

void historial_por_zona(Zona **zonas, int *cont);
void mostrar_historial_zona(int id_zona);

void simular_monitoreo_tiempo_real(Zona **zonas, int *cont);

void buscar_eventos_rango(Zona **zonas, int *cont);

void cambiarUmbral(Zona **arr, int cont);

void restaurar_configuracion_default(Zona **zonas, int *cont);

#endif

