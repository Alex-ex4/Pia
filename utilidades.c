#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilidades.h"
#include "validaciones.h"
#include "reporte.h"
#include "archivos.h"
#define ARCHIVO_USUARIOS "usuarios.bin"
//#define MAX_NOMBRE 50
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"


// ===========  FUNCIONES DE VALIDACIÓN INTERNAS ===========

static int leer_entero_validado(const char* mensaje, int min, int max) {
    
    char buffer[20];
    int num;
    int es_numero, en_rango;

    do {
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        es_numero = validarEsNumero(buffer); //
        if (!es_numero) {
            printf("Error: Ingrese solo números.\n");
            en_rango = 0; 
            continue;
        }
        
        num = atoi(buffer); 
        
        en_rango = validarRango(num, min, max); //
        
        if (!en_rango) {
            printf("Error: Ingrese un valor entre %d y %d.\n", min, max);
        }

    } while (!es_numero || !en_rango);
    
    return num;
}


static void leer_cadena_validada(const char* mensaje, char* destino, int longitud_buffer, int min_len, int max_len) {
    
    int es_vacio, longitud_valida;

    do {
        printf("%s", mensaje);
        fgets(destino, longitud_buffer, stdin);
        destino[strcspn(destino, "\n")] = '\0';

        es_vacio = validaEsVacio(destino); //
        if (es_vacio) {
            printf("Error: El campo no puede estar vacío.\n");
            longitud_valida = 0; 
            continue;
        }

        longitud_valida = validarLongitud(destino, min_len, max_len); //
        if (!longitud_valida) {
            printf("Error: La longitud debe estar entre %d y %d caracteres.\n", min_len, max_len);
        }

    } while (es_vacio || !longitud_valida);
}

static float leer_float_validado(const char* mensaje, float min, float max) {

    char buffer[50]; 
    float num;
    int es_numero;

    do {
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        es_numero = validarEsFlotante(buffer); 
        
        if (!es_numero) {
            printf("Error: Ingrese un valor numérico válido (ej. 35.5).\n");
            continue;
        }
        
        num = atof(buffer); 
        
        if (num < min || num > max) {
            printf("Error: Ingrese un valor entre %.2f y %.2f.\n", min, max);
            es_numero = 0;
        }

    } while (!es_numero);
    
    return num;
}

// #--fin--#

// Edson 
Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado) {
    
    for(int i = 0; i < cont ; i++) {
        if(strcmp(arr[i].nom, nombreBuscado) == 0) {
            return &arr[i];
        }
    }
    return NULL;
}


Zona* buscar_zona_por_id(int id_zona, Zona *zona) {
    
    FILE *archivo = fopen(ARCHIVO, "rb");
    if (!archivo) {
        return NULL;
    }
    
    while (fread(zona, sizeof(Zona), 1, archivo) == 1) {
        if (zona->id == id_zona) {
            fclose(archivo);
            return zona;
        }
    }
    
    fclose(archivo);
    return NULL;
}


float generar_temperatura_aleatoria() {
    
    return 25.0 + (rand() % 150) / 10.0; // 25.0 a 40.0 grados, la neta no se si ya existia esta funcion
}


float generarTemp(int min, int max){
    
    srand(time(NULL));
    float temp=min+rand()%(max-min+1);
    return temp;
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



// en esta funcion se registran los eventos en un archivo binario
// esto forma parte de la estructura del historial
void registrar_evento(int id_zona, float temperatura, int estado_ventilador) {
    
    Historial evento;
    evento.idZona = id_zona;
    evento.hora = time(NULL);
    evento.temperatura = temperatura;
    evento.estado_ventilador = estado_ventilador;
    
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "ab");
    if (archivo != NULL) {
        fwrite(&evento, sizeof(Historial), 1, archivo);
        fclose(archivo);
    }
}


void registrarZona(Zona **zonas, int *cont) {
    
    Zona nuevaZona;
    
    // *** MODIFICADO (Entero) ***
    nuevaZona.id = leer_entero_validado("Ingrese ID de la zona (1-9999): ", 1, 9999);
    
    // *** MODIFICADO (Cadena) ***
    leer_cadena_validada("Ingrese nombre de la zona (1-49): ", nuevaZona.nom, sizeof(nuevaZona.nom), 1, 49);

    // *** MODIFICADO Y VALIDADO (Float) ***
    nuevaZona.umbral = leer_float_validado(
        "Ingrese umbral de temperatura (10.0-50.0 °C): ", 
        10.0, 
        50.0 
    );
    
    strcpy(nuevaZona.ventilador, "OFF");

    nuevaZona.historiales = (Historial *)malloc(sizeof(Historial));
    float temp_inicial = generar_temperatura_aleatoria();
    int estado_ventilador = (temp_inicial > nuevaZona.umbral) ? 1 : 0;
    nuevaZona.historiales[0].idZona = nuevaZona.id;
    nuevaZona.historiales[0].temperatura = temp_inicial;
    nuevaZona.historiales[0].hora = time(NULL);
    nuevaZona.historiales[0].estado_ventilador = estado_ventilador;
    strcpy(nuevaZona.ventilador, estado_ventilador ? "ON" : "OFF");
    nuevaZona.temp_predet = nuevaZona.umbral;
    nuevaZona.cont_historial = 1;
    (*cont)++;
    *zonas = realloc(*zonas, (*cont) * sizeof(Zona));
    (*zonas)[(*cont) - 1] = nuevaZona;
    registrar_evento(nuevaZona.id, temp_inicial, estado_ventilador);
    agregarArchivo(nuevaZona);
    printf("\nZona registrada exitosamente!\n");
    printf("Temperatura inicial: %.2f°C | Ventilador: %s\n", 
           temp_inicial, nuevaZona.ventilador);
}

// Edson 
void temperaturaActual( Zona **zonas, int *cont) {
    
    if (zonas == NULL || *zonas == NULL || cont == NULL) return;

    int i, opcion;
    char nombre[50];
    Zona *arr = *zonas;

    do 
    {
        printf("\n1) Zona especifica por nombre.\n2) Listar todas las zonas\n3) Volver\n");
        
        // *** MODIFICADO (Entero) ***
        opcion = leer_entero_validado("Seleccion: ", 1, 3);
        
        switch (opcion)
        {
        case 1: 
        {
            // *** MODIFICADO (Cadena) ***
            leer_cadena_validada("\tIngresar el nombre de la zona: ", nombre, sizeof(nombre), 1, 49);
            
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
            break;
        case 3:
            break;
        default:
            printf("Opcion invalida...");
            break;
        }
    }while(opcion != 3);
}


void activarVent(Zona **zonas, int *cont){
    
    if(*zonas == NULL || *cont <= 0){
        printf("No hay zonas registradas.\n");
        return;
    }
    char nombreZona[50];
    int opcion;
    
    // *** MODIFICADO (Cadena) ***
    leer_cadena_validada("Ingresa el nombre de la zona: ", nombreZona, sizeof(nombreZona), 1, 49);

    Zona *zonaEncontrada = buscarZonaPorNombre(*zonas, *cont, nombreZona);
    if(zonaEncontrada == NULL){ 
        printf("Zona no encontrada.\n");
        return;
    }

    if (zonaEncontrada->cont_historial == 0) {
        printf("Error: La zona no tiene historial de eventos. No se puede obtener la temperatura actual.\n");
        return; 
    }

    printf("Seleccione accion: \n");
    printf("1. Encender ventilador\n"); 
    printf("2. Apagar ventilador\n");   
    
    // *** MODIFICADO (Entero) ***
    opcion = leer_entero_validado("Seleccion: ", 1, 2);
    
    Historial nuevoEvento;
    nuevoEvento.idZona = zonaEncontrada->id;
    nuevoEvento.temperatura = zonaEncontrada->historiales[zonaEncontrada->cont_historial - 1].temperatura; 
    nuevoEvento.hora = time(NULL);

    if(opcion==1){
        nuevoEvento.estado_ventilador = 1;

        zonaEncontrada->cont_historial++;
        zonaEncontrada->historiales= realloc(zonaEncontrada->historiales, zonaEncontrada->cont_historial*sizeof(Historial));
        zonaEncontrada->historiales[zonaEncontrada->cont_historial-1]=nuevoEvento;
        strcpy(zonaEncontrada->ventilador, "ON");
        printf("Ventilador encendido manually.\n");
    } else if(opcion==2){ 
        nuevoEvento.estado_ventilador = 0;

        zonaEncontrada->cont_historial++;
        zonaEncontrada->historiales= realloc(zonaEncontrada->historiales, zonaEncontrada->cont_historial*sizeof(Historial));
        zonaEncontrada->historiales[zonaEncontrada->cont_historial-1]=nuevoEvento;
        strcpy(zonaEncontrada->ventilador, "OFF");
        printf("Ventilador apagado manually.\n");
    } 
    
    escribirArchivo(*zonas, *cont);
    registrar_evento(nuevoEvento.idZona, nuevoEvento.temperatura, nuevoEvento.estado_ventilador);
}


// Aca empieza el desmadre la verdad no se como explicarlo

// muestra primero las zonas para que el usuario pueda ver el ID y elegir
//y despues llamo a la funcion mostrar_historial_zona que muestra el historial
void historial_por_zona() {
    
    listaZonas();
    int id_zona;
    
    // *** MODIFICADO (Entero) ***
    id_zona = leer_entero_validado("Ingrese el ID de la zona para ver su historial (1-9999): ", 1, 9999);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    printf("\n=== HISTORIAL DE EVENTOS - %s ===\n", zona.nom);
    mostrar_historial_zona(id_zona);
}

//Aca ees donde se muestra el historial de una zona especifica
void mostrar_historial_zona(int id_zona) {
    
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf("No hay eventos registrados.\n");
        return;
    }
    
    Historial evento;
    int encontrados = 0;
    

    printf("Eventos para la zona ID %d:\n", id_zona);
    while (fread(&evento, sizeof(Historial), 1, archivo)) {
        if (evento.idZona == id_zona) {

            // struct tm* tiempo me sirve para convertir el tiempo en una estructura legible
            // en este caso localtime convierte el tiempo a la hora local
            // y gracias a eso puedo imprimir la fecha y hora del evento con tm_mday, tm_mon, tm_hour, tm_min, tm_sec

            struct tm* tiempo = localtime(&evento.hora);
            printf("[%02d/%02d %02d:%02d:%02d] Temperatura: %.1f ºC - Ventilador: %s\n",
                   tiempo->tm_mday, tiempo->tm_mon + 1,
                   tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec,
                   evento.temperatura,
                   evento.estado_ventilador ? "ON" : "OFF");
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("No se encontraron eventos para esta zona.\n");
    } else {
        printf("Total de eventos encontrados: %d\n", encontrados);
    }
    
    fclose(archivo);
}


// en esta funcion use el metodo que vi en el pdf usando ciclos y sleep para simular el monitoreo en tiempo real

void simular_monitoreo_tiempo_real() {
    
    listaZonas();
    int id_zona;
    
    // *** MODIFICADO (Entero) ***
    id_zona = leer_entero_validado("Ingrese el ID de la zona para simular monitoreo (1-9999): ", 1, 9999);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    int ciclos;
    int intervalo;
    
    // *** MODIFICADO (Entero) ***
    ciclos = leer_entero_validado("Ingrese numero de ciclos de monitoreo (1-100): ", 1, 100);
    intervalo = leer_entero_validado("Ingrese intervalo entre ciclos (segundos) (1-60): ", 1, 60);

    printf("\n=== SIMULACIÓN EN TIEMPO REAL ===\n");
    printf("Zona: %s | Umbral: %.1f°C\n", zona.nom, zona.umbral);
    printf("Ciclos: %d | Intervalo: %d segundos\n\n", ciclos, intervalo);
    
    for (int i = 0; i < ciclos; i++) {
        float temperatura = generar_temperatura_aleatoria();
        int estado_ventilador = (temperatura > zona.umbral) ? 1 : 0;
        
        time_t ahora = time(NULL);
        struct tm* tiempo = localtime(&ahora);
        
        printf("[%02d:%02d:%02d] Temperatura: %.1f ºC - Ventilador: %s\n",
               tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec,
               temperatura, estado_ventilador ? "ON" : "OFF");
        
        registrar_evento(id_zona, temperatura, estado_ventilador);
        
        if (i < ciclos - 1) {
            sleep(intervalo);
        }
    }
    
    printf("\nSimulación completada. %d eventos registrados.\n", ciclos);
}

// aqui la verdad no se como hacer que busque por fecha asi que lo deje solo por rango de temperatura
// asi dejemoslo ya que sera muy complicado implementar la busqueda por fecha sorry
void buscar_eventos_rango() {
    
    listaZonas();
    int id_zona;
    
    // *** MODIFICADO (Entero) ***
    id_zona = leer_entero_validado("Ingrese el ID de la zona para buscar eventos (1-9999): ", 1, 9999);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    float temp_min, temp_max;
    
    // *** MODIFICADO Y VALIDADO (Float) ***
    temp_min = leer_float_validado(
        "Ingrese la temperatura minima del rango (-50.0-100.0 °C): ", 
        -50.0, 
        100.0
    );
    temp_max = leer_float_validado(
        "Ingrese la temperatura maxima del rango (-50.0-100.0 °C): ", 
        -50.0, 
        100.0
    );
    
    printf("\n=== EVENTOS ENCONTRADOS (%.1f - %.1f °C) ===\n", temp_min, temp_max);
    printf("Zona: %s\n\n", zona.nom);
    
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf("No hay eventos registrados.\n");
        return;
    }
    
    Historial evento;
    int encontrados = 0;
    
    while (fread(&evento, sizeof(Historial), 1, archivo)) {
        if (evento.idZona == id_zona && 
            evento.temperatura >= temp_min && 
            evento.temperatura <= temp_max) {
            
            struct tm* tiempo = localtime(&evento.hora);
            printf("[%02d/%02d %02d:%02d] Temperatura: %.1f ºC - Ventilador: %s\n",
                   tiempo->tm_mday, tiempo->tm_mon + 1,
                   tiempo->tm_hour, tiempo->tm_min,
                   evento.temperatura,
                   evento.estado_ventilador ? "ON" : "OFF");
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("No se encontraron eventos en el rango especificado.\n");
    } else {
        printf("\nTotal de eventos encontrados: %d\n", encontrados);
    }
    
    fclose(archivo);
}


// Edson
//Integracion: cambiarUmbral(arr,*cont);
// No se que pase aca pero al momento de correr el programa hay algunos fallos en la consola
// nada relacionado con la logica del programa sino quiza hay un error de memoria o algo asi
void cambiarUmbral(Zona **arr, int cont) {
    
    if ( *arr == NULL || cont <= 0) return;

    char nombre[50];
    float nuevoUmbral;
    
    // *** MODIFICADO (Cadena) ***
    leer_cadena_validada("\tIngresar el nombre de la zona:", nombre, sizeof(nombre), 1, 49);

    Zona *z = buscarZonaPorNombre( *arr , cont , nombre );
    if( z == NULL) {
        printf("Zona no encontrada");
        return;
    }
    printf("> Zona _%s_ encontrada\n", z->nom);
    printf("\n\tUmbral actual: %f °C", z->umbral);
    
    // *** MODIFICADO Y VALIDADO (Float) ***
    nuevoUmbral = leer_float_validado(
        "\n\tIngrese el nuevo valor del umbral (10.0-50.0 °C): ", 
        10.0, 
        50.0
    );

    z->umbral = nuevoUmbral;
    printf("> Umbral aztualizado correctamente.\n");
    printf("\n\tUmbral actual: %f °C\n", z->umbral);
    actualizarVentilador(z);
    escribirArchivo(*arr, cont);
}   



// Esta funcion restaura la configuracion por defecto de una zona especifica
// aun quiero checar lo de almacenar los umbrales predeterminados
// asi que por defecto puse 25.0 grados
void restaurar_configuracion_default(Zona **zonas, int *cont) {
    
    if (*zonas == NULL || *cont <= 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listaZonas();
    char nombre[50];
    
    // *** MODIFICADO (Cadena) ***
    leer_cadena_validada("Ingrese el nombre de la zona a restaurar: ", nombre, sizeof(nombre), 1, 49);
    
    
    // Buscar zona por nombre
    Zona *zona = buscarZonaPorNombre(*zonas, *cont, nombre);
    if (zona == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    printf("\nZona encontrada: %s\n", zona->nom);
    printf("Umbral actual: %.2f°C\n", zona->umbral);
    
    // Restaurar umbral por defecto (25°C)
    float umbral_default = zona->temp_predet; // Se modificara despues que vea como conservar los umbrales predeterminados
    zona->umbral = umbral_default;
    
    
    // Limpiar historial de eventos individual para esta zona
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    FILE *temp_archivo = fopen("temp_eventos.bin", "wb");
    
    if (archivo != NULL && temp_archivo != NULL) {
        Historial evento;
        int eventos_eliminados = 0;
        
        while (fread(&evento, sizeof(Historial), 1, archivo)) {
            if (evento.idZona != zona->id) {
                // Conservar eventos de otras zonas
                fwrite(&evento, sizeof(Historial), 1, temp_archivo);
            } else {
                eventos_eliminados++;
            }
        }
        
        fclose(archivo);
        fclose(temp_archivo);
        
        // Reemplazar archivo original
        remove(ARCHIVO_EVENTOS);
        rename("temp_eventos.bin", ARCHIVO_EVENTOS);
        
        printf("Historial de eventos eliminado: %d eventos\n", eventos_eliminados);
    } else {
        printf("No se pudo limpiar el historial de eventos.\n");
    }
    
    // Limpiar historial en memoria
    if (zona->historiales != NULL) {
        free(zona->historiales);
        zona->historiales = (Historial *)malloc(sizeof(Historial));
        
        // Crear nuevo evento inicial
        zona->historiales[0].idZona = zona->id;
        zona->historiales[0].temperatura = generar_temperatura_aleatoria();
        zona->historiales[0].hora = time(NULL);
        zona->historiales[0].estado_ventilador = 0;
    }
    
    // Actualizar ventilador
    strcpy(zona->ventilador, "OFF");
    
    printf("\n✓ Configuración restaurada exitosamente para la zona: %s\n", zona->nom);
    printf("✓ Nuevo umbral: %.2f°C\n", zona->umbral);
    printf("✓ Historial de eventos limpiado\n");
    printf("✓ Ventilador: %s\n", zona->ventilador);
    
    // Actualizar archivo de zonas
    escribirArchivo(*zonas, *cont);
}