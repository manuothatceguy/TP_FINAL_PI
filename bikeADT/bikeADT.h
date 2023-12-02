/**
 * ARCHIVO DE ENCABEZADO PARA EL TIPO ABSTRACTO DE DATO 
 * PARA PROCESAR DATOS DE ALQUILERES DE BICICLETAS
 * 
 * Autores: Manuel Othatceguy 64671, 
 *          Santiago Sanchez Marostica 64056,
 *          Gregorio Tiscornia 64087
*/

#include <stdio.h>

#ifndef BIKE_ADT_H
#define BIKE_ADT_H

typedef struct bikeCDT * bikeADT;

enum order{NAME_ID = 0, ID_NAME};

/**
 * @param stations matriz con las estaciones válidas, se espera que sea de la forma de una matriz de strings, con un id y un nombre por estación.
 * @param stationNbr cantidad de estaciones
 * @param orderMatrix si order es 0, la 1ra columna es el id, la 2da el nombre. 1 si es viceversa.
 * @return Un ADT vacío que espera llenarse o NULL si fallo.
 * @brief Esta función debe ejecutarse al iniciar con este ADT.
*/
bikeADT newBikeADT(char *** stations, size_t stationNbr, enum order orderMatrix); // recibe un vector de estaciones validas

/**
 * @param bikes el ADT previamente inicializado con la función newBikeADT
 * @param stationFrom ID de la estación de origen del viaje
 * @param stationTo ID de la estación de destino del viaje
 * @param startDate fecha y hora en la que se inició el viaje
 * @param endDate fecha y hora en la que se finalizó el viaje
 * @param isMember si el usuario es miembro del sistema o no.
 * @return 1 si se agregó, 0 si no se agregó
 * @brief Agrega un viaje, si alguna de las estaciones no existe las agrega. Se espera que se utilice esta función para la carga masiva de datos.
*/
int addTrip(bikeADT bikes, unsigned int stationFrom, unsigned int stationTo, char * startDate, char * endDate, char isMember);

struct tripCounter{
    char * stationName;
    size_t memberTrips;
    size_t nonMemberTrips;
    size_t allTrips;
};

/**
 * QUERY 1
 * @param bikes el ADT previamente inicializado con la función newBikeADT
 * @return vector de struct tripCounter
*/
struct tripCounter * getTotalTrips(bikeADT bikes);

/**
 * @param bikes memoria reservada a liberar
 * @brief libera la memoria reservada
*/
void freeBikes(bikeADT bikes);
#endif