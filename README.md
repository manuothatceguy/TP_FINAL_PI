# TP FINAL PROGRAMACIÓN IMPERATIVA

## Descripción del proyecto
> Trabajo práctico especial de la materia Programación Imperativa (72.31).

Consta de un programa para procesar datos de alquileres de bicicletas para dos ciudades: Montreal y Nueva York a partir de la información en archivos con extensión `.csv`

## Integrantes del grupo

| Integrante | Legajo |
| ----------- | ----------- |
| Manuel Othatceguy | 64671 |
| Santiago Sanchez Marostica | 64056 |
| Gregorio Tiscornia | 64087 |

## Compilación con Makefile
- Para generar solo el ejecutable de Montreal:
```
make MON
```
- Para generar solo el ejecutable de New York:
```
make NYC
```
- Para generar ambos ejecutables
```
make all
```
- Para borrar los ejecutables y archivos generados (recomendable correr este comando antes de arrancar con el make)
```
make clean
```

## Como ejecutar el programa
Una vez compilado, se debe ejecutar de la siguiente manera:

```
make clean && make (CIUDAD) && ./bikeSharing(CIUDAD) (path de bikes(CIUDAD).csv) (path de stations(CIUDAD).csv)
```

Siendo (CIUDAD) una de las siguientes opciones:
- MON
- NYC

Los archivos producidos por los ejecutables serán generados en la misma carpeta que los mismos.

### IMPORTANTE
Si se escribe MON, se esperan archivos con la estructura (pero no el contenido) definida en la consigna que le corresponden a Montreal, vale lo mismo para Nueva York.

Es decir:

| Archivo | Estructura |
| --------| -----------|
| bikesMON.csv | started_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member |
| stationsMON.csv | pk;name;latitude;longitude |
| bikesNYC.csv | started_at;start_station_id;ended_at;end_station__id;rideable_type;member_casual |
| stationsNYC.csv | station_name;latitude;longitude;id |
