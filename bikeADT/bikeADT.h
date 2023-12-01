/**
 * ARCHIVO DE ENCABEZADO PARA EL TIPO ABSTRACTO DE DATO 
 * PARA PROCESAR DATOS DE ALQUILERES DE BICICLETAS
*/

#include <stdio.h>

#ifndef BIKE_ADT_H
#define BIKE_ADT_H

typedef struct bikeCDT * bikeADT;


typedef struct elemVec{  //estructura que define los elementos del vector.
    size_t stationID; // ID de la estacion
    char * name; // nombre de la estacion
}elemVec;

/**
 * @param validIDs arreglo de estaciones validas.
 * @return Un ADT vacío que espera llenarse
 * @brief Esta función debe ejecutarse al iniciar con este ADT.
*/
bikeADT newBikeADT(elemVec validIDs[]); // recibe un vector de estaciones validas

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

/**
 * @param bikes el ADT previamente inicializado con la función newBikeADT
 * @return matriz con la siguiente estructura: nombre_estacion | cant. de viajes de miembros | cant. de viajes de no miembros | cant. de viajes totales
 * 
 * 
 * 
 * 
 * 
*/
unsigned int ** getTotalTrips(bikeADT bikes);


#endif