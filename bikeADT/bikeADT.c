#include "bikeADT.h"
#include "../checkErrno/checkErrno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// typedef struct elemVec{  //estructura que define los elementos del vector.
//     size_t stationID; // ID de la estacion
//     char * name; // nombre de la estacion
// }elemVec;

typedef struct bikeCDT{
    size_t stationCount;    // Cantidad de estaciones    
    tStation * stations;    // Vector de estaciones se agrega a medida que se agreguen viajes
    tDay days[NUM_DAYS];    // Vector de struct day 
} bikeCDT;

static int compareStations(tStation s1, tStation s2){
    return s1.id - s2.id;
}

/**
 * @param stationsIpt estaciones a agregar
 * @param stationsOpt destino de las estaciones
 * @param orderMatrix si es nombre-id o id-nombre
 * @param stationNbr cantidad de estaciones a agregar
 * @brief traduce la matriz de char *** al formato de tStation*
*/
static int matrixToElemVec(char *** stationsIpt, tStation * stationsOpt, enum order orderMatrix, size_t stationNbr){
    stationsOpt = malloc(stationNbr*sizeof(tStation));
    int nameIdx;
    int idIdx;

    if(orderMatrix == 1){
        nameIdx = 1;
        idIdx = 0;
    }
    else{
        nameIdx = 0;
        idIdx = 1;
    }
    
    for(int i = 0; i < stationNbr; i++){
        errno = 0;
        stationsOpt[i].name = malloc((strlen(stationsIpt[i][nameIdx]) + 1)*sizeof(char));
        if(checkErrno(stationsOpt[i].name)){
            return 0;
        }
        strcpy(stationsOpt[i].name,stationsIpt[i][nameIdx]);
        stationsOpt[i].id = atol(stationsIpt[i][idIdx]);
    }
    // Ordeno para hacer búsqueda binaria
    qsort(stationsOpt, stationNbr, sizeof(tStation), compareStations); 
    return 1;
}

bikeADT newBikeADT(char *** stations, size_t stationNbr, enum order orderMatrix){
    bikeADT new = calloc(1,sizeof(bikeCDT));
    new->stationCount = stationNbr;
    if(matrixToElemVec(stations,new->stations,orderMatrix, stationNbr) == 0){
        return NULL; // Si falla la asignación de memoria dinámica, retorno null
    }
    return new;
}

/**
 * @param stations Vector de estaciones
 * @param id ID de la estacion a buscar 
 * @param dim Cantidad de estaciones en el vector stations
 * @return 1 si existe la estacion, 0 de lo contrario
 * @brief Como los IDs se encuentran ordenados, realiza busqueda binaria para verificar que existan ambas estaciones 
*/
static tStation * binarySearch(tStation * stations, unsigned int id, size_t dim){
    if(dim == 0)
        return NULL;
    if(stations[(int)(dim/2)].id > id){
        return binarySearch(stations,id,(int)(dim/2));
    } else if(stations[(int)(dim/2)].id < id){
        return binarySearch(stations+(int)(dim/2),id,(int)(dim/2));
    } else{
        return stations;
    }
}

static int strToTime(struct tm *d, char * date, char * format){
    return sscanf(date,format,&d->tm_year, &d->tm_mon, &d->tm_mday, &d->tm_hour, &d->tm_min, &d->tm_sec);
}


/**
 * @param date1 Fecha a comparar 1
 * @param date2 Fecha a comparar 2
 * @return >0 si date1 > date2, =0 si date1 = date2, <0 si date1 < date2
 * @brief Compara las fechas date1 y date2
*/
static int compareDates(char * date1, char * date2, char * formato){
    struct tm d1;
    struct tm d2;
    // Conversión de char * al struct tm de la librería time.h
    strToTime(&d1,date1,formato);
    strToTime(&d2,date2,formato);
    // Conversión del struct tm a un time_t (necesario para difftime)
    time_t a = mktime(&d1);
    time_t b = mktime(&d2);
    return difftime(a,b);
}

/**
 * @param trips Vector de estaciones
 * @param stationFrom ID de la estacion de origen
 * @param stationTo ID de la estacion de destino
 * @param startDate fecha y hora en la que se inició el viaje
 * @param endDate fecha y hora en la que se finalizó el viaje
 * @brief Agrega recursivamente a la lista ordenada cronologicamente un trip  
*/
static TList addTripRec(TList trips, unsigned int stationTo, char * startDate, char * endDate){
    char c;
    if( trips == NULL || ((c=compareDates(startDate, trips->dateStart, "&d-&d-&d &d:&d:&d")) < 0)){
        errno = 0;
        TList aux = malloc(sizeof(tTrip));
        if(checkErrno(aux)){
            return NULL;
        }
        aux->dateStart = startDate;
        aux->dateEnd = endDate;
        aux->destName = stationTo;
        aux->tail = trips;
        return aux;
    } else // Si son iguales agrego despues (orden de procesado)
    {
        addTripRec(trips->tail, stationTo, startDate, endDate);
        return trips;
    }
}

int addTrip(bikeADT bikes, unsigned int stationFrom, unsigned int stationTo, char * startDate, char * endDate, char isMember){
    tStation * foundStationFrom = binarySearch(bikes->stations, stationFrom, bikes->stationCount);
    tStation * foundStationTo = binarySearch(bikes->stations, stationTo, bikes->stationCount);
    if(foundStationFrom == NULL || foundStationTo == NULL ){
        return 0;
    }

    foundStationFrom->trips = addTripRec(foundStationFrom->trips,stationTo,startDate,endDate);

    // for(int i = 0; i < bikes->stationCount; i++){
    //     if(bikes->stations[i].id == stationFrom){
    //         bikes->stations[i].trips = addTripRec(bikes->stations[i].trips,stationTo,startDate,endDate);
    //         if(isMember){
    //             bikes->stations[i].memberTripCount++;
    //         }
    //         else{
    //             bikes->stations[i].notMemberTripCount++;
    //         }
            
    //     }
    // }
    // bikes->stations->trips = addTripRec(bikes->stations, stationTo, startDate, endDate);
    // Falta el índice de la estación!!!!!!!!!!!!!!
    
    if( isMember ){
        foundStationFrom->memberTripCount++;
    } else {
        foundStationFrom->notMemberTripCount++;
    }
    return 1;
}