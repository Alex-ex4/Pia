#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilidades.h"
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"

int main() {

    while(validar_usuario()!=1){
        if (!validar_usuario()) {
            return 1;
        }
    }

    int cont = 0;
    float *tempPredet = NULL; // el proposito de este puntero es almacenar los umbrales predeterminados
    int *contHistorial = NULL; // el proposito de este puntero es almacenar la cantidad de eventos en el historial por zona
    Zona *zonas = NULL;

    // aun necesito ver como arreglar todos los inconvenientes con el tema de la temperatura predeterminada y el historial

    int opp;
    char ops;
    
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
                printf("\tb. Listar zonas registradas\n");
                printf("\tc. Volver al menú principal\n");
                printf("\tSeleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    registrarZona(&zonas, &cont, &tempPredet, &contHistorial); // Aqui exactamente no se que pasa
                    // aun trato de entender el porque de &tempPredet y &contHistorial (fue algo que me sugirio el IDE para que funcione)
                    
                    break;
                case 'b':
                    listaZonas();
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
        case 2:
            do{
                printf("\n\t---CONTROL DE TEMPERATURAS---\n");
                printf("\ta. Ver temperatura actual\n");
                printf("\tb. Activar ventilador manualmente\n");
                printf("\tc. Ver historial de eventos por zona\n");
                printf("\td. Simular monitoreo en tiempo real\n");
                printf("\te. Volver al menú principal\n");
                printf("\tSeleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    temperaturaActual(&zonas, &cont);
                    break;
                case 'b':
                    //activarVent(&zonas, &cont);
                    printf("\tFunción en desarrollo...\n");
                    break;
                case 'c': 
                    historial_por_zona();
                    break;
                case 'd':
                    simular_monitoreo_tiempo_real();
                    break;
                case 'e': 
                    printf("\tVolviendo al menú principal...\n");
                    break;
                default:
                    printf("\tOpción no válida\n");    
                    break;
                }
            } while (ops!='e');
            break;
        case 3: 
            do
            {
                printf("\n\t---CONSULTAS---\n");
                printf("\ta. Buscar eventos por rango de temperatura\n");
                printf("\tb. Generar reporte estadistico\n");
                printf("\tc. Exportar historial a archivo csv\n");
                printf("\td. Volver al menú principal\n");
                printf("\tSeleccione una opcion: ");
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    buscar_eventos_rango();
                    break;
                case 'b':
                    //reporte(&zonas, &cont, &contHistorial);
                    break;
                case 'c':
                    //exportar_historial_csv();
                    printf("\tFunción en desarrollo...\n");
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
                    if (zonas != NULL && cont > 0) {
                        cambiarUmbral(zonas, cont);
                    } else {
                        printf("\tNo hay zonas registradas para configurar.\n");
                    }
                    break;
                case 'b':
                    restaurar_configuracion_default(&zonas, &cont, &tempPredet);
                    //printf("\tFunción en desarrollo...\n");
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
    
    // Liberar memoria gg
    if (zonas != NULL) {
        for (int i = 0; i < cont; i++) {
            if (zonas[i].historiales != NULL) {
                free(zonas[i].historiales);
            }
        }
        free(zonas);
    }
    if (tempPredet != NULL) free(tempPredet);
    if (contHistorial != NULL) free(contHistorial);
    
    return 0;
}
    } while (opp!=5);
    
    free(zonas);
    return 0;

}
