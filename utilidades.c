#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilidades.h"
#define ARCHIVO_USUARIOS "usuarios.bin"
//#define MAX_NOMBRE 50
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
#define ARCHIVO_EVENTOS "eventos.bin"

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

float generar_temperatura_aleatoria() {
    return 25.0 + (rand() % 150) / 10.0; // 25.0 a 40.0 grados, la neta no se si ya existia esta funcion
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
// No se que pase aca pero al momento de correr el programa hay algunos fallos en la consola
// nada relacionado con la logica del programa sino quiza hay un error de memoria o algo asi
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

float generarTemp(int min, int max){
    srand(time(NULL));
    float temp=min+rand()%(max-min+1);
    return temp;
}


//Hice algunos cambios aqui Lazaro
//genere un evento inicial al registrar la zona, modifique los detalles que tenia
//esto debido a que actualice las estructuras en el archivo utilidades.h
//pero en general funciona igual
void registrarZona(Zona **zonas, int *cont, float **tempPredet, int **contHistorial) {
    Zona nuevaZona;
    printf("Ingrese ID de la zona: ");
    scanf("%d", &nuevaZona.id);
    limpiar_buffer();

    printf("Ingrese nombre de la zona: ");
    fgets(nuevaZona.nom, sizeof(nuevaZona.nom), stdin);
    nuevaZona.nom[strcspn(nuevaZona.nom, "\n")] = '\0';

    printf("Ingrese umbral de temperatura: ");
    scanf("%f", &nuevaZona.umbral);
    strcpy(nuevaZona.ventilador, "OFF");

    // Inicializar historial
    nuevaZona.historiales = (Historial *)malloc(sizeof(Historial));
    
    // Generar temperatura inicial y registrar evento
    float temp_inicial = generar_temperatura_aleatoria();
    int estado_ventilador = (temp_inicial > nuevaZona.umbral) ? 1 : 0;
    
    // Configurar primer evento del historial
    nuevaZona.historiales[0].idZona = nuevaZona.id;
    nuevaZona.historiales[0].temperatura = temp_inicial;
    nuevaZona.historiales[0].hora = time(NULL);
    nuevaZona.historiales[0].estado_ventilador = estado_ventilador;
    
    // Actualizar estado del ventilador según la temperatura
    strcpy(nuevaZona.ventilador, estado_ventilador ? "ON" : "OFF");

    (*cont)++;
    *zonas = realloc(*zonas, (*cont) * sizeof(Zona));
    (*zonas)[(*cont) - 1] = nuevaZona;

    *tempPredet = realloc(*tempPredet, (*cont) * sizeof(float));
    (*tempPredet)[(*cont) - 1] = nuevaZona.umbral;

    *contHistorial = realloc(*contHistorial, (*cont) * sizeof(int));
    (*contHistorial)[(*cont) - 1] = 1;

    // Registrar evento en el archivo de eventos
    registrar_evento(nuevaZona.id, temp_inicial, estado_ventilador);
    
    agregarArchivo(nuevaZona);
    
    printf("\nZona registrada exitosamente!\n");
    printf("Temperatura inicial: %.2f°C | Ventilador: %s\n", 
           temp_inicial, nuevaZona.ventilador);
}


void activarVent(Zona **zonas, int *cont){
    //funcion para buscar zona con nombre
    
}

//--------------
//IMPORTANTE DE LEER
//NO SE PORQUE PERO ESTA FUNCION HACE QUE NO COMPILE EL PROGRAMA ASI QUE LA COMENTE
// PARA DESPUES VER CON MAS CALMA COMO SOLUCIONARLO
//--------------

/*void reporte(Zona **zonas, int *cont, int **contHistorial){
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
        prom=suma/(*(contHistorial+i)); // EN ESPECIFICO AQUI HAY UN PROBLEMA CON EL PUNTERO contHistorial
        printf("Zona: %s\n", (*zonas)[i].nom);
        printf("Reporte estadistico:\n");
        printf("Temperatura maxima: %.2f °C\n", tempmax);
        printf("Temperatura minima: %.2f °C\n", tempmin);
        printf("Temperatura promedio: %.2f °C\n\n", prom); 
    }
}*/

// Aca empieza el desmadre la verdad no se como explicarlo

// muestra primero las zonas para que el usuario pueda ver el ID y elegir
//y despues llamo a la funcion mostrar_historial_zona que muestra el historial
void historial_por_zona() {
    listaZonas();
    int id_zona;
    printf("Ingrese el ID de la zona para ver su historial: ");
    scanf("%d", &id_zona);
    limpiar_buffer();
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        getchar();
        return;
    }
    
    printf("\n=== HISTORIAL DE EVENTOS - %s ===\n", zona.nom);
    mostrar_historial_zona(id_zona);
    getchar();
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

//Aca ees donde se muestra el historial de una zona especifica
// utilizo la libreria time.h para mostrar la fecha y hora de cada evento
// no recuerdo si es en esta funcion pero tambien tiene algunos fallos en cuestion a lo que imprime pero no afecta la logica del programa
// pulire mejor esos detalles despues
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
    printf("Ingrese el ID de la zona para simular monitoreo: ");
    scanf("%d", &id_zona);
    limpiar_buffer();
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        getchar();
        return;
    }
    
    int ciclos;
    int intervalo;
    printf("Ingrese número de ciclos de monitoreo: ");
    scanf("%d", &ciclos);
    printf("Ingrese intervalo entre ciclos (segundos): ");
    scanf("%d", &intervalo);
    limpiar_buffer();

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
    getchar();
}

// aqui la verdad no se como hacer que busque por fecha asi que lo deje solo por rango de temperatura
// asi dejemoslo ya que sera muy complicado implementar la busqueda por fecha sorry
void buscar_eventos_rango() {
    listaZonas();
    int id_zona;
    printf("Ingrese el ID de la zona para buscar eventos: ");
    scanf("%d", &id_zona);
    
    Zona zona;
    if (buscar_zona_por_id(id_zona, &zona) == NULL) {
        printf("Error: Zona no encontrada.\n");
        getchar();
        return;
    }
    
    float temp_min, temp_max;
    printf("Ingrese la temperatura mínima del rango: ");
    scanf("%f", &temp_min);
    printf("Ingrese la temperatura máxima del rango: ");
    scanf("%f", &temp_max);
    limpiar_buffer();
    
    printf("\n=== EVENTOS ENCONTRADOS (%.1f - %.1f °C) ===\n", temp_min, temp_max);
    printf("Zona: %s\n\n", zona.nom);
    
    FILE *archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        printf("No hay eventos registrados.\n");
        getchar();
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
    getchar();
}

// aqui se llama a traves de Zona* porque necesito devolver la direccion de memoria de la zona encontrada
// esto sirve para no cargar todo el archivo en memoria
// la funcion no es void porque devuelve un puntero a Zona

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


// Esta funcion restaura la configuracion por defecto de una zona especifica
// aun quiero checar lo de almacenar los umbrales predeterminados
// asi que por defecto puse 25.0 grados
void restaurar_configuracion_default(Zona **zonas, int *cont, float **tempPredet) {
    if (*zonas == NULL || *cont <= 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    listaZonas();
    
    char nombre[50];
    printf("Ingrese el nombre de la zona a restaurar: ");
    scanf("%49s", nombre);
    limpiar_buffer();
    
    // Buscar zona por nombre
    Zona *zona = buscarZonaPorNombre(*zonas, *cont, nombre);
    if (zona == NULL) {
        printf("Error: Zona no encontrada.\n");
        return;
    }
    
    printf("\nZona encontrada: %s\n", zona->nom);
    printf("Umbral actual: %.2f°C\n", zona->umbral);
    
    // Restaurar umbral por defecto (25°C)
    float umbral_default = 25.0; // Se modificara despues que vea como conservar los umbrales predeterminados
    zona->umbral = umbral_default;
    
    // Actualizar el array tempPredet
    for (int i = 0; i < *cont; i++) {
        if (strcmp((*zonas)[i].nom, nombre) == 0) {
            (*tempPredet)[i] = umbral_default;
            break;
        }
    }
    
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
