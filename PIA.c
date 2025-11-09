#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utilidades.h"
#include "validaciones.h"
#include "archivos.h"
#include "reporte.h"
#define ARCHIVO "archivo.bin"
#define HISTORIAL "historial.csv"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main() {

    while(validar_usuario()!=1){
        if (!validar_usuario()) {
            return 1;
        }
    }

    int cont = 0;
    Zona *zonas = NULL;

    cargarZonasDesdeArchivo(&zonas, &cont); 
    
    cargarHistorialDesdeArchivo(zonas, cont);

    int opp;
    char ops;
    
    do
    {

        printf("\n" ANSI_COLOR_BLUE "=========================================" ANSI_COLOR_RESET "\n");
        printf(ANSI_COLOR_BLUE "||" ANSI_COLOR_RESET ANSI_COLOR_CYAN "       SISTEMA DE MONITOREO DE TEMPERATURA       " ANSI_COLOR_RESET ANSI_COLOR_BLUE "||" ANSI_COLOR_RESET "\n");
        printf(ANSI_COLOR_BLUE "||" ANSI_COLOR_RESET ANSI_COLOR_CYAN "              M E N U   P R I N C I P A L        " ANSI_COLOR_RESET ANSI_COLOR_BLUE "||" ANSI_COLOR_RESET "\n");
        printf(ANSI_COLOR_BLUE "=========================================" ANSI_COLOR_RESET "\n");
        printf(ANSI_COLOR_GREEN " 1. [ Z O N A S ]            " ANSI_COLOR_RESET " - Registro y Listado\n");
        printf(ANSI_COLOR_GREEN " 2. [ C O N T R O L ]        " ANSI_COLOR_RESET " - Temperaturas y Ventiladores\n");
        printf(ANSI_COLOR_GREEN " 3. [ C O N S U L T A S ]    " ANSI_COLOR_RESET " - Historial y Reportes\n");
        printf(ANSI_COLOR_GREEN " 4. [ C O N F I G ]          " ANSI_COLOR_RESET " - Umbrales y Restauracion\n");
        printf(ANSI_COLOR_RED   " 5. [ S A L I R ]            " ANSI_COLOR_RESET " - Finalizar Programa\n");
        printf(ANSI_COLOR_BLUE "-----------------------------------------" ANSI_COLOR_RESET "\n");
        printf(ANSI_COLOR_YELLOW "   >>> Seleccione una opcion: " ANSI_COLOR_RESET);
        
        scanf("%d", &opp);
        switch (opp)
        {
        case 1:
            do{
                printf("\n" ANSI_COLOR_CYAN "\t---[ SECCION: ZONAS ]---\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "a. >> REGISTRAR Zona\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "b. >> LISTAR zonas registradas\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_RED   "c. << VOLVER al menu principal\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_CYAN "--------------------------\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_YELLOW "Seleccione una opcion: " ANSI_COLOR_RESET);
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    registrarZona(&zonas, &cont);
                    break;
                case 'b':
                    listaZonas();
                    break;
                case 'c':
                    printf(ANSI_COLOR_MAGENTA "\tVolviendo al menu principal...\n" ANSI_COLOR_RESET);
                    break;
                default:
                    printf(ANSI_COLOR_RED "\tOpcion no valida\n" ANSI_COLOR_RESET);
                    break;
                }
            } while (ops!='c');
            
            break;
        case 2:
            do{
                printf("\n" ANSI_COLOR_CYAN "\t---[ SECCION: CONTROL DE TEMPERATURAS ]---\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "a. >> Ver temperatura actual\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "b. >> Activar ventilador manualmente\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "c. >> Ver historial de eventos por zona\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "d. >> Simular monitoreo en tiempo real\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_RED   "e. << VOLVER al menu principal\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_CYAN "------------------------------------------\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_YELLOW "Seleccione una opcion: " ANSI_COLOR_RESET);
                scanf(" %c", &ops);
                switch (ops)
                {
                case 'a':
                    temperaturaActual(&zonas, &cont);
                    break;
                case 'b':
                    activarVent(&zonas, &cont);
                    break;
                case 'c': 
                    historial_por_zona(&zonas, &cont);
                    break;
                case 'd':
                    simular_monitoreo_tiempo_real(&zonas, &cont);
                    break;
                case 'e': 
                    printf(ANSI_COLOR_MAGENTA "\tVolviendo al menu principal...\n" ANSI_COLOR_RESET);
                    break;
                default:
                    printf(ANSI_COLOR_RED "\tOpcion no valida\n" ANSI_COLOR_RESET);    
                    break;
                }
            } while (ops!='e');
            break;
        case 3: 
            do
            {
                printf("\n" ANSI_COLOR_CYAN "\t---[ SECCION: CONSULTAS Y REPORTES ]---\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "a. >> Buscar eventos por rango de temperatura o fecha\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "b. >> Generar reporte estadistico\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "c. >> Exportar historial a archivo CSV\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_RED   "d. << VOLVER al menu principal\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_CYAN "--------------------------------------\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_YELLOW "Seleccione una opcion: " ANSI_COLOR_RESET);
                scanf(" %c", &ops);
                getchar();
                switch (ops)
                {
                case 'a':
                    buscar_eventos_rango(&zonas, &cont);
                    break;
                case 'b':
                    reporte(&zonas, &cont);
                    break;
                case 'c':
                    exportar_historial_csv(&zonas, &cont);
                    break;
                case 'd':
                    printf(ANSI_COLOR_MAGENTA "\tVolviendo al menu principal...\n" ANSI_COLOR_RESET);
                    break;
                default:
                    printf(ANSI_COLOR_RED "\tOpcion no valida\n" ANSI_COLOR_RESET);
                    break;
                }
            } while (ops!='d');
            
            break;
        case 4:
            do
            {
                printf("\n" ANSI_COLOR_CYAN "\t---[ SECCION: CONFIGURACION ]---\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "a. >> Configurar umbral de temperatura por zona\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_GREEN "b. >> Restaurar configuracion por defecto por zona\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_RED   "c. << VOLVER al menu principal\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_CYAN "--------------------------------\n" ANSI_COLOR_RESET);
                printf("\t" ANSI_COLOR_YELLOW "Seleccione una opcion: " ANSI_COLOR_RESET);
                scanf(" %c", &ops);
                switch (ops)
                {
                case 'a':
                    cambiarUmbral(&zonas, cont);
                    break;
                case 'b':
                    restaurar_configuracion_default(&zonas, &cont);
                    break;
                case 'c': 
                    printf(ANSI_COLOR_MAGENTA "\tVolviendo al menu principal...\n" ANSI_COLOR_RESET);
                    break;
                default:
                    printf(ANSI_COLOR_RED "\tOpcion no valida\n" ANSI_COLOR_RESET);
                    break;
                }
            } while (ops!='c');
            
            break;
        case 5:
            printf("\n" ANSI_COLOR_RED "=========================================" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_RED "         Saliendo del programa... [ADIOS]    " ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_RED "=========================================" ANSI_COLOR_RESET "\n");
            break;
        default:
            printf(ANSI_COLOR_RED "Opcion no valida. Intente de nuevo.\n" ANSI_COLOR_RESET);
            break;
        }
    } while (opp!=5);
    
    if (zonas != NULL) {
        for (int i = 0; i < cont; i++) {
            if (zonas[i].historiales != NULL) {
                free(zonas[i].historiales);
            }
        }
        free(zonas);
    }
    
    return 0;
}