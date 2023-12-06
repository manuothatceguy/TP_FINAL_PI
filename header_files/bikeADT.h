/**
 * ARCHIVO DE ENCABEZADO PARA EL ADT 
 * PARA PROCESAR DATOS DE ALQUILERES DE BICICLETAS
 * 
 * Autores: Manuel Othatceguy 64671, 
 *          Santiago Sanchez Marostica 64056,
 *          Gregorio Tiscornia 64087
*/
#include <time.h>
#include <stdio.h>

#ifndef BIKE_ADT_H
#define BIKE_ADT_H

typedef struct bikeCDT * bikeADT;

typedef struct elem{  //estructura que define los elementos del vector.
    size_t stationID; // ID de la estacion
    char * name; // nombre de la estacion
} stationInput;

/**
 * @param stations Vector con las estaciones válidas.
 * @param numOfStations Cantidad de estaciones
 * @return Un ADT vacío que espera llenarse o NULL si fallo.
 * @brief Esta función debe ejecutarse al iniciar con este ADT.
*/
bikeADT newBikeADT(stationInput * stations, size_t numOfStations); // recibe un vector de estaciones validas

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
void addTrip(bikeADT bikes, unsigned int stationFrom, unsigned int stationTo, char * startDate, char * endDate, char isMember);

struct tripCounter{
    char * stationName;
    size_t memberTrips;
    size_t nonMemberTrips;
    size_t allTrips;
};



/**
 * QUERY 1
 * @param bikes el TAD previamente inicializado con la función newBikeADT
 * @return vector de struct tripCounter ordenado descendientemente por cantidad total de viajes por station
*/
struct tripCounter * getTotalTrips(bikeADT bikes);

struct oldestTrip{
    char * stationFrom;
    char * stationTo;   // Nombre de la estación de llegada
    time_t dateTime;    // Tiempo de salida. Después se convierte con gmtime() 
};

/**
 * QUERY 2
 * @param bikes TAD creado previamente
 * @param dim de salida para la cantidad de elementos
 * @return vector de struct oldestTrip o NULL si fallo
*/
struct oldestTrip * getOldestTrips(bikeADT bikes, int * dim);

typedef struct day{
    size_t started;     // Cantidad de viajes iniciados en este día
    size_t ended;       // Cantidad de viajes terminados en este día
} tDay;

/**
 * QUERY 3
 * @param bikes TAD
 * @return vector de 7 tDay con idx 0 = DOMINGO
*/
tDay * tripsPerDay(bikeADT bikes);

/**
 * @return cantidad de stations guardadas
*/
size_t getTotalStations(bikeADT bikes);

/**
 * @param bikes TAD a liberar
 * @brief libera la memoria reservada
*/
void freeBikes(bikeADT bikes);

#endif
