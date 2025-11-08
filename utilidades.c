#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilidades.h"
#include "validaciones.h"
#define ARCHIVO_USUARIOS "usuarios.bin"
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"



static int leer_entero_validado(const char* mensaje, int min, int max) {
    char buffer[20];
    int num;
    int es_numero, en_rango;

    do {
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        es_numero = validarEsNumero(buffer); 
        if (!es_numero) {
            printf("Error: Ingrese solo números.\n");
            en_rango = 0; 
            continue;
        }
        
        num = atoi(buffer); 
        
        en_rango = validarRango(num, min, max); 
        
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

        es_vacio = validaEsVacio(destino); 
        if (es_vacio) {
            printf("Error: El campo no puede estar vacío.\n");
            longitud_valida = 0; 
            continue;
        }

        longitud_valida = validarLongitud(destino, min_len, max_len); 
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



void cargarZonasDesdeArchivo(Zona **zonas, int *cont) {
    FILE *archivo = fopen(ARCHIVO, "rb");
    
    if (archivo == NULL) {
        printf("Iniciando sin zonas precargadas.\n");
        *cont = 0;
        *zonas = NULL; 
        return;
    }

    Zona zonaLeida;
    int contador_cargado = 0;
    
    while (fread(&zonaLeida, sizeof(Zona), 1, archivo) == 1) {
        
        zonaLeida.historiales = NULL;
        zonaLeida.cont_historial = 0; 

        contador_cargado++;
        *zonas = (Zona *)realloc(*zonas, contador_cargado * sizeof(Zona));

        if (*zonas == NULL) {
            fclose(archivo);
            exit(EXIT_FAILURE);
        }
        
        (*zonas)[contador_cargado - 1] = zonaLeida;
    }

    *cont = contador_cargado;
    fclose(archivo);
    
    printf("Carga de zonas completada: %d zonas recuperadas.\n", *cont);
}


void cargarHistorialDesdeArchivo(Zona *zonas, int cont) {
    if (cont == 0) return; 

    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf("El historial se cargará vacío.\n");
        return;
    }

    Historial eventoLeido;
    int eventos_cargados = 0;


    while (fread(&eventoLeido, sizeof(Historial), 1, archivo) == 1) {
        Zona *zona_destino = NULL;
        for (int i = 0; i < cont; i++) {
            if (zonas[i].id == eventoLeido.idZona) {
                zona_destino = &zonas[i];
                break;
            }
        }
        
        if (zona_destino != NULL) {
            zona_destino->cont_historial++;
            zona_destino->historiales = (Historial *)realloc(zona_destino->historiales, zona_destino->cont_historial * sizeof(Historial));
            
            if (zona_destino->historiales == NULL) {
                exit(EXIT_FAILURE); 
            }

            zona_destino->historiales[zona_destino->cont_historial - 1] = eventoLeido;
            eventos_cargados++;
        }
    }

    fclose(archivo);
    printf("Reconstrucción de historial completada: %d eventos cargados.\n", eventos_cargados);
}


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
    return 25.0 + (rand() % 150) / 10.0; 
}

float generarTemp(int min, int max){
    srand(time(NULL));
    float temp=min+rand()%(max-min+1);
    return temp;
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
        printf("\nID: %d \nNombre: %s \nUmbral: %.2f \nVentilador: %s\n",
                zona.id, zona.nom, zona.umbral, zona.ventilador);
    }
    fclose(archivo);
}


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
    

    leer_cadena_validada("Ingrese usuario: ", usuario, sizeof(usuario), 1, 49);
    
    leer_cadena_validada("Ingrese contraseña: ", password, sizeof(password), 1, 49);

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
    
    nuevaZona.id = leer_entero_validado("Ingrese ID de la zona (1-9999): ", 1, 9999);
    
    leer_cadena_validada("Ingrese nombre de la zona (1-49): ", nuevaZona.nom, sizeof(nuevaZona.nom), 1, 49);

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

void temperaturaActual( Zona **zonas, int *cont) {
    if (zonas == NULL || *zonas == NULL || cont == NULL) {
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }

    int i, opcion;
    char nombre[50];
    Zona *arr = *zonas;

    do 
    {
        printf("\n1) Zona especifica por nombre.\n2) Listar todas las zonas\n3) Volver\n");
        
        opcion = leer_entero_validado("Seleccion: ", 1, 3);
        
        switch (opcion)
        {
        case 1: 
        {
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
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    char nombreZona[50];
    int opcion;
    limpiar_buffer();
    
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

    printf("Seleccione acción: \n");
    printf("1. Encender ventilador\n"); 
    printf("2. Apagar ventilador\n");   
    
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
        printf("Ventilador encendido manualmente.\n");
    } else if(opcion==2){ 
        nuevoEvento.estado_ventilador = 0;

        zonaEncontrada->cont_historial++;
        zonaEncontrada->historiales= realloc(zonaEncontrada->historiales, zonaEncontrada->cont_historial*sizeof(Historial));
        zonaEncontrada->historiales[zonaEncontrada->cont_historial-1]=nuevoEvento;
        strcpy(zonaEncontrada->ventilador, "OFF");
        printf("Ventilador apagado manualmente.\n");
    } 
    
    escribirArchivo(*zonas, *cont);
    registrar_evento(nuevoEvento.idZona, nuevoEvento.temperatura, nuevoEvento.estado_ventilador);
}



void historial_por_zona(Zona **zonas, int *cont) {
    if(*zonas == NULL || *cont <= 0){
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    listaZonas();
    int id_zona;
    limpiar_buffer();
    
    id_zona = leer_entero_validado("Ingrese el ID de la zona para ver su historial (1-9999): ", 1, 9999);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    printf("\n=== HISTORIAL DE EVENTOS - %s ===\n", zona.nom);
    mostrar_historial_zona(id_zona);
}

void mostrar_historial_zona(int id_zona) {
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    
    Historial evento;
    int encontrados = 0;
    

    printf("Eventos para la zona ID %d:\n", id_zona);
    while (fread(&evento, sizeof(Historial), 1, archivo)) {
        if (evento.idZona == id_zona) {
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



void simular_monitoreo_tiempo_real(Zona **zonas, int *cont) {
    if(*zonas == NULL || *cont <= 0){
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    listaZonas();
    int id_zona;
    
    id_zona = leer_entero_validado("Ingrese el ID de la zona para simular monitoreo (1-9999): ", 1, 9999);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    int ciclos;
    int intervalo;
    
    ciclos = leer_entero_validado("Ingrese número de ciclos de monitoreo (1-100): ", 1, 100);
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

void buscar_eventos_rango(Zona **zonas, int *cont) {
    if(*zonas == NULL || *cont <= 0){
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    listaZonas();
    int id_zona;
    int op_busqueda;
    
    id_zona = leer_entero_validado("Ingrese el ID de la zona para buscar eventos (1-9999): ", 1, 9999);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }

    printf("\nSeleccione el tipo de búsqueda:\n");
    printf("1. Por TEMPERATURA\n");
    printf("2. Por FECHA\n");
    op_busqueda = leer_entero_validado("Opción: ", 1, 2);
    
    float temp_min, temp_max;
    long fecha_i = 0, fecha_f = 0; 

    if(op_busqueda==1){
        temp_min = leer_float_validado(
        "Ingrese la temperatura mínima del rango (-50.0-100.0 °C): ", 
        -50.0, 
        100.0
        );
        temp_max = leer_float_validado(
            "Ingrese la temperatura máxima del rango (-50.0-100.0 °C): ", 
            -50.0, 
            100.0
        );
        printf("\n=== EVENTOS ENCONTRADOS (%.1f - %.1f °C) ===\n", temp_min, temp_max);
    }else{
        int dia_i, mes_i, anio_i, dia_f, mes_f, anio_f;
        
        printf("\n--- INGRESE FECHA DE INICIO ---\n");
        dia_i = leer_entero_validado("Día (1-31): ", 1, 31);
        mes_i = leer_entero_validado("Mes (1-12): ", 1, 12);
        anio_i = leer_entero_validado("Año (ej. 2025): ", 2020, 2100);

        printf("\n--- INGRESE FECHA DE FIN ---\n");
        dia_f = leer_entero_validado("Día (1-31): ", 1, 31);
        mes_f = leer_entero_validado("Mes (1-12): ", 1, 12);
        anio_f = leer_entero_validado("Año (ej. 2025): ", 2020, 2100);
        
        struct tm tm_inicio = {0}, tm_fin = {0};

        tm_inicio.tm_mday = dia_i; 
        tm_inicio.tm_mon = mes_i - 1; 
        tm_inicio.tm_year = anio_i - 1900;
        
        tm_fin.tm_mday = dia_f; 
        tm_fin.tm_mon = mes_f - 1; 
        tm_fin.tm_year = anio_f - 1900;

        fecha_i = mktime(&tm_inicio);

        tm_fin.tm_hour = 23; tm_fin.tm_min = 59; tm_fin.tm_sec = 59;
        fecha_f = mktime(&tm_fin);

        if (fecha_i == (long)-1 || fecha_f == (long)-1 || fecha_i > fecha_f) {
            printf("Error: Rango de fechas inválido.\n");
            return;
        }

        printf("\n=== EVENTOS ENCONTRADOS (Rango de Fecha) ===\n");
    }
    
        
    printf("Zona: %s\n\n", zona.nom);
    
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf("No hay eventos registrados.\n");
        return;
    }
    
    Historial evento;
    int encontrados = 0;
    
    while (fread(&evento, sizeof(Historial), 1, archivo)) {
        if (evento.idZona == id_zona) {
            if (op_busqueda == 1) {
                if (evento.temperatura >= temp_min && evento.temperatura <= temp_max) {
                    struct tm* tiempo = localtime(&evento.hora);
                    printf("[%02d/%02d %02d:%02d] Temperatura: %.1f ºC - Ventilador: %s\n",
                           tiempo->tm_mday, tiempo->tm_mon + 1,
                           tiempo->tm_hour, tiempo->tm_min,
                           evento.temperatura,
                           evento.estado_ventilador ? "ON" : "OFF");
                    encontrados++;
                }
            } else { 
                if (evento.hora >= fecha_i && evento.hora <= fecha_f) {
                    struct tm* tiempo = localtime(&evento.hora);
                    printf("[%02d/%02d %02d:%02d] Temperatura: %.1f ºC - Ventilador: %s\n",
                           tiempo->tm_mday, tiempo->tm_mon + 1,
                           tiempo->tm_hour, tiempo->tm_min,
                           evento.temperatura,
                           evento.estado_ventilador ? "ON" : "OFF");
                    encontrados++;
                }

            }
        }
    }
    
    if (encontrados == 0) {
        printf("No se encontraron eventos en el rango especificado.\n");
    } else {
        printf("\nTotal de eventos encontrados: %d\n", encontrados);
    }
    
    fclose(archivo);
}


void reporte(Zona **zonas, int *cont){
    if(*zonas == NULL || *cont <= 0){
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
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


void exportar_historial_csv(Zona **zonas, int *cont){
    if(*zonas == NULL || *cont <= 0){
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
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


void cambiarUmbral(Zona **arr, int cont) {
    if ( *arr == NULL || cont <= 0) {
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    char nombre[50];
    float nuevoUmbral;
    
    leer_cadena_validada("\tIngresar el nombre de la zona:", nombre, sizeof(nombre), 1, 49);

    Zona *z = buscarZonaPorNombre( *arr , cont , nombre );
    if( z == NULL) {
        printf("Zona no encontrada");
        return;
    }
    printf("> Zona _%s_ encontrada\n", z->nom);
    printf("\n\tUmbral actual: %f °C", z->umbral);
    
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


void restaurar_configuracion_default(Zona **zonas, int *cont) {
    if (*zonas == NULL || *cont <= 0) {
        printf(ANSI_COLOR_RED "\tNo hay zonas registradas.\n" ANSI_COLOR_RESET);
        return;
    }
    
    listaZonas();
    char nombre[50];
    
    leer_cadena_validada("Ingrese el nombre de la zona a restaurar: ", nombre, sizeof(nombre), 1, 49);
    
    
    Zona *zona = buscarZonaPorNombre(*zonas, *cont, nombre);
    if (zona == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    printf("\nZona encontrada: %s\n", zona->nom);
    printf("Umbral actual: %.2f°C\n", zona->umbral);
    
    float umbral_default = zona->temp_predet; 
    zona->umbral = umbral_default;
    
    
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    FILE *temp_archivo = fopen("temp_eventos.bin", "wb");
    
    if (archivo != NULL && temp_archivo != NULL) {
        Historial evento;
        int eventos_eliminados = 0;
        
        while (fread(&evento, sizeof(Historial), 1, archivo)) {
            if (evento.idZona != zona->id) {
                fwrite(&evento, sizeof(Historial), 1, temp_archivo);
            } else {
                eventos_eliminados++;
            }
        }
        
        fclose(archivo);
        fclose(temp_archivo);
        
        remove(ARCHIVO_EVENTOS);
        rename("temp_eventos.bin", ARCHIVO_EVENTOS);
        
        printf("Historial de eventos eliminado: %d eventos\n", eventos_eliminados);
    } else {
        printf("No se pudo limpiar el historial de eventos.\n");
    }
    
    if (zona->historiales != NULL) {
        free(zona->historiales);
        zona->historiales = (Historial *)malloc(sizeof(Historial));
        
        zona->historiales[0].idZona = zona->id;
        zona->historiales[0].temperatura = generar_temperatura_aleatoria();
        zona->historiales[0].hora = time(NULL);
        zona->historiales[0].estado_ventilador = 0;
    }
    
    strcpy(zona->ventilador, "OFF");
    
    printf("\n✓ Configuración restaurada exitosamente para la zona: %s\n", zona->nom);
    printf("✓ Nuevo umbral: %.2f°C\n", zona->umbral);
    printf("✓ Historial de eventos limpiado\n");
    printf("✓ Ventilador: %s\n", zona->ventilador);
    
    escribirArchivo(*zonas, *cont);
}