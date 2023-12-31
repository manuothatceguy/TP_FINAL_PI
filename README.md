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
> Flags incluidas en la compilación: -Wall -pedantic -std=c99 -fsanitize=address

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
- Para borrar los ejecutables (de tipo .o) y archivos generados (de tipo query(NUM).html y query(NUM).csv)
```
make clean
```

## Como ejecutar el programa
Una vez compilado, se debe ejecutar de la siguiente manera:

```
./bikeSharing(CIUDAD) (path de bikes(CIUDAD).csv) (path de stations(CIUDAD).csv)
```

Siendo (CIUDAD) una de las siguientes opciones:
- MON
- NYC

Los archivos producidos por los ejecutables se guardarán en la misma carpeta que los mismos.

### IMPORTANTE
Si se escribe MON, se esperan archivos con la estructura (pero no el contenido) definida en el enunciado que le corresponden a Montreal, vale lo mismo para Nueva York.

Es decir:

| Archivo | Estructura |
| --------| -----------|
| bikesMON.csv | started_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member |
| stationsMON.csv | pk;name;latitude;longitude |
| bikesNYC.csv | started_at;start_station_id;ended_at;end_station__id;rideable_type;member_casual |
| stationsNYC.csv | station_name;latitude;longitude;id |

### Sobre la carpeta "archivos_test"
Incluimos los archivos `.csv` que armamos nosotros para poder verificar nuestros resultados, dado que varios de los archivos son casi imposibles de manipular en software de hoja de cálculo o base de datos por su gran extensión. Para eso, los archivos que incluimos tienen la misma información para ambas estructuras de ciudad (MON o NYC):

- El archivo bikes tiene 2 viajes
- El archivo stations tiene 9 estaciones

Se espera que haya 2 viajes saliendo de la estación "Obelisco", uno de "member" y otro de "casual". Además, esos viajes deben salir un sábado y un domingo, para así llegar un martes y un lunes, respectivamente.

Probando los ejecutables, se obtuvieron los resultados esperados.
