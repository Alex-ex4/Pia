# Documentación

--- Funcionalidades --- DESCARGAR LA ESTRUCT AL ARCHIVO LUEGO DE CERRAR Y EN LA SIG. EJECUCION CARGAR DESDE EL ARCHIVO
1) Registro de zona. Poner un evento primario y registrar el umbral predeterminado (LAZARO)
2) Ver temperatura Acual. (EDSON)
       - Funcion de busqueda de zona por nombre 
       - Funcion ON//OFF segun el umbral
3) Activar ventilador manualmente (LAZARO)
       - (X2) Funcion para buscar zona por noombre
       - Registrarlo como evento para el ultimo elemento de la lista de eventos
4) ver historial de eventos POR ZONA (FRAN)
       - Struct anidado
5) Simular monitoreo en tiempo real (FRAN)
       - Funcion para generar aleatoriamente( usando sleep y delay)
          - Cambiar de día por cada ejecución
6) Buscar eventos por rango de temperatura o fecha. (FRAN)
       - (X3) funcion para buscar zona por nombre
          - Pedir temp minima y maxima y de ahi buscar
7) Generar reporte estasistico (LAZARO)
       - Recorre cada zona y dentro de cada zona su historial, dentro de cada historial cacula todo el pedo
8) Exportar todo el historial a un CSV (LAZARO)
       - Recorre cada zona y dentro de cada zona su historial, y guarda su historial
9) condifurar umbral de temperatura por zona (EDSON)
        - permite al usuario actualizar el umbral manualmente
        - (X4) Funcion de buscar zona por nombre
        - (X2) Funcion de ON/OFF umbral automaticamente.
10) Restaurar configuracion por defecto por zona. (FRAN)
        - (X5) Funcion para buscar zona por nombre
        - Restaura umbral y limpia su historial de eventos INDIVIDUAL.

11) Validaciones y así
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
        En cada ejecucion actualiza la temperatura, y el umbral/ventilador se actualiza solo con la funcion actualizarVentilador
        [ *arr]: Es un puntero que apunta al primer elemento de zonas

    - void actualizarVentilador( Zona *z)
           Genera temperatura aleatoria en un intervalo de umbral de  [-5,+5] 
           Actualiza el ventilador (ON / OFF)
    - Zona* buscarZonaPorNombre(Zona *arr, int cont, char *nombreBuscado)
           Busca el nombre de una zona en la memoria de la estructura
    - void cambiarUmbral(Zona *arr, int cont)
            Permite al usuario cambiar el umbral manualmente
            Posterior a lo anterior, el ventilador se actualiza automaticamente
    

---ESTRUCTURA---
