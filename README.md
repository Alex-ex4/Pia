# Documentación

--- Funcionalidades ---
1) Registro de zona.
2) Ver temperatura Acual.
       - Funcion de busqueda de zona por nombre
       - Funcion ON//OFF segun el umbral
4) Activar ventilador manualmente
       - (X2) Funcion para buscar zona por noombre
       - Funcion para apagar o prender ventilador
5) ver historial de eventos POR ZONA 
       - Struct anidado
6) Simular monitoreo en tiempo real
       - Funcion para generar aleatoriamente( usando sleep y delay)
7) Buscar eventos por rango de temperatura o fecha.
       - Despues
8) Generar reporte estasistico
       - Recorre cada zona y dentro de cada zona su historial, dentro de cada historial cacula todo el pedo
9) Exportar todo el historial a un CSV
       - Recorre cada zona y dentro de cada zona su historial, y guarda su historial
10) condifurar umbral de temperatura por zona
        - permite al usuario actualizar el ventilador manualmente
        - (X3) Funcion de buscar zona por nombre
        - (X2) Funcion de ON/OFF umbral automaticamente.
11) Restaurar configuracion por defecto por zona.
        - (X4) Funcion para buscar zona por nombre
        - Restaura umbral y limpia su historial de eventos INDIVIDUAL.

12) Validaciones
- Funcion
  
----CONSTANTES----
    ARCHIVO "archivo.bin"
    HISTORIAL "historial.csv"

---VARIABLES---
    int opp 
        opcion para el menu principal

    char ops
        opcion para los menus secundarios

    int cont=0
        contador para el numero de zonas
        
    Zonas *zonas[]
        lista de zonas con memoria dinamica

---FUNCIONES---
    -void escribirArchivo(Zona *zonas, int cont);
        Escribe en el archivo la lista de todas las zonas
        Sobreescribe
    
    -void agregarArchivo(Zona zona);
        Agrega al final del archivo el registro de una zona

    -void listaZonas()
        Muestra las zonas existentes registradas en el archivo binario

    -float generarTemp(minimo, maximo)
        Devuelve una temperatura alatoria desde un minimo hasta un maximo
    
    -void registrarZona(Zona **zonas)
        Registra una nueva zona en el archivo binario 
        Agrega a una lista de zonas con realloc
        Aumenta el contador en 1
    
    -void temperaturaActual( Zona **zonas, int *cont)
        Muestra la temperatura/Estado del ventilador.
        Actuliza la temperatura y apaga o prende el ventilador ( Esta seccion seria deseable de convertir en una funcion)
        [ *arr]: Es un puntero que apunta al primer elemento de zonas

    -Creo que seria rentable crear una funcion que actualize tambien los datos del archivo binario y ponerla al final de cada funcion

---ESTRUCTURA---
