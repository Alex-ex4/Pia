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
    float temp_predet;
    int cont_historial;
} Zona;

typedef struct {
    char usuario[50];
    char password[50];
} Usuario;


//void leer_cadena(const char* mensaje, char* cadena, int longitud);


//Otras funciones

void limpiar_buffer();

float generar_temperatura_aleatoria();
float generarTemp(int min, int max);

Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado);
Zona* buscar_zona_por_id(int id_zona, Zona *zona);

void escribirArchivo(Zona *zonas, int cont);

void agregarArchivo(Zona zona);

void listaZonas(); 

void actualizarVentilador( Zona *z);

void registrar_evento(int id_zona, float temperatura, int estado_ventilador);

//Usuarios
int validar_usuario();

//Opcion 1
void registrarZona(Zona **zonas, int *cont);

//Opcion 2
void temperaturaActual( Zona **zonas, int *cont);

//Opcion 3
void activarVent(Zona **zonas, int *cont);

//Opcion 4
void historial_por_zona();
void mostrar_historial_zona(int id_zona);

//Opcion 5
void simular_monitoreo_tiempo_real();

//Opcion 6
void buscar_eventos_rango();

//Opcion 7
void reporte(Zona **zonas, int *cont);

//Opcion 8
void exportar_historial_csv(Zona **zonas, int *cont);

//Opcion 9
void cambiarUmbral(Zona **arr, int cont);

//Opcion 10
void restaurar_configuracion_default(Zona **zonas, int *cont);

#endif
