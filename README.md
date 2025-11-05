# Documentación
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
