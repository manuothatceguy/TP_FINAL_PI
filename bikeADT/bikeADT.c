#include "bikeADT.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_DAYS 7
enum days{MON=0,TUE,WED,THU,FRI,SAT,SUN};

typedef struct trip{
    char * dateStart;   // Fecha en la que se comenzó a utilizar la bicicleta
    char * dateEnd;     // Fecha en la que se regreso la bicicleta
    char * destName;    // Nombre del destino donde se dejo la bicicleta
    struct trip * tail; // Puntero al siguiente viaje
} tTrip;

typedef struct trip * TList;

typedef struct station{
    char * name;                // Nombre de la estación
    size_t id;                  // Id de la estación

    TList trips;                // Lista de viajes con origen en esta estación ordenados ascendente por fecha
    size_t memberTripCount;     // Cantidad de viajes de usuarios que son miembros con origen en esta estación
    size_t notMemberTripCount;  // Cantidad de viajes de usuarios que no son miembros con origen en esta estación
} tStation;

typedef struct day{
    size_t started;     // Cantidad de viajes iniciados en este día
    size_t ended;       // Cantidad de viajes terminados en este día
} tDay;

typedef struct elemVec{  //estructura que define los elementos del vector.
    size_t stationID; // ID de la estacion
    char * name; // nombre de la estacion
}elemVec;

typedef struct bikeCDT{
    elemVec *validStations;
    size_t stationCount;    // Cantidad de estaciones    
    tStation * stations;    // Vector de estaciones
    tDay days[NUM_DAYS];    // Vector de struct day 
} bikeCDT;

enum order{NAME_ID = 0, ID_NAME};

static void matrixToElemVec()

bikeADT newBikeADT(char *** stations, size_t stationNbr, int order){
    bikeADT new = calloc(1,sizeof(bikeCDT));
    new->stationCount = stationNbr;
    matrixToElemVec(stations,new->validStations,order);
}
