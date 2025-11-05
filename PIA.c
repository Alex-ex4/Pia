#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilidades.h"
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"
// hola soi omelo chino

int main() {

    while(validar_usuario()!=1){
        if (!validar_usuario()) {
            return 1;
        }
    }
    /*if (!validar_usuario()) {
        return 1;
    }*/

    int opp;
    char ops;
    int cont=0;
    Zona *zonas=(Zona *) malloc(cont*sizeof(Zona));
    do
    {
        printf("\n---MENU---\n");
        printf("1. Zonas \n");
        printf("2. Control de Temperaturas \n");
        printf("3. Consultas\n");
        printf("4. Configuración\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opp);
        switch (opp)
        {
        case 1:
            do{
                printf("\n\t---ZONAS---\n");
                printf("\ta. Registro de Zona\n");
                printf("\tb. Volver al menú principal\n");
                printf("\tSeleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    registrarZona(&zonas, &cont);
                    break;
                case 'b':
                    printf("\tVolviendo al menú principal...\n");
                    break;
                default:
                    printf("\tOpción no válida\n");
                    break;
                }
            } while (ops!='b');
            
            break;
        case 2:
            do{
                printf("\n\t---TEMPERATURAS---\n");
                printf("\ta. Ver temperatura actual\n");
                printf("\tb. Activar ventilador manualmente\n");
                printf("\tc. Ver historial de eventos\n");
                printf("\td. Volver al menú principal\n");
                printf("\tSeleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    break;
                case 'b':
                    break;
                case 'c': 
                    break;
                case 'd': 
                    printf("\tVolviendo al menú principal...\n");
                    break;
                default:
                    printf("\tOpción no válida\n");    
                    break;
                }
            } while (ops!='d');
            break;
        case 3: 
            do
            {
                printf("\n\t---CONSULTAS---\n");
                printf("\ta. Buscar eventos por rango de temperatura o fecha\n");
                printf("\tb. Generar reporte estadistico\n");
                printf("\tc. Exportar historial a archivo csv\n");
                printf("\td. Volver al menú principal\n");
                printf("\tSeleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    break;
                case 'b':
                    break;
                case 'c':
                    break;
                case 'd':
                    printf("\tVolviendo al menú principal...\n");
                    break;
                default:
                    printf("\tOpción no válida\n");
                    break;
                }
            } while (ops!='d');
            
            break;
        case 4:
            do
            {
                printf("\n\t---CONFIGURACION---\n");
                printf("\ta. Configurar umbral de temperatura por zona\n");
                printf("\tb. Restaurar configuración por defecto por zona\n");
                printf("\tc. Volver al menú principal\n\t");
                printf("Seleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    break;
                case 'b':
                    break;
                case 'c': 
                    printf("\tVolviendo al menú principal...\n");
                    break;
                default:
                    printf("\tOpción no válida\n");
                    break;
                }
            } while (ops!='c');
          
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opción no válida.\n");
            break;
        }
    } while (opp!=5);
    
    free(zonas);
    return 0;
}