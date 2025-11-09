#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "utilidades.h"
#include "validaciones.h"
#include "archivos.h"


void escribirArchivo(Zona *zonas, int cont);

void agregarArchivo(Zona zona);

void listaZonas(); 

void cargarZonasDesdeArchivo(Zona **zonas, int *cont);
void cargarHistorialDesdeArchivo(Zona *zonas, int cont);


#endif