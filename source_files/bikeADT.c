#include "bikeADT.h"
#include "checkErrno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#define NUM_DAYS 7
#define LEN_DATE_Q2 17 // El formato de fecha que se retorna en el query 2 tiene una longitud de 17 caracteres 
#define BLOCK 10

// typedef struct trip{
//     time_t dateStart;   // Fecha en la que se comenzó a utilizar la bicicleta
//     time_t dateEnd;     // Fecha en la que se regreso la bicicleta
//     char * destName;    // Nombre del destino donde se dejo la bicicleta
//     struct trip * tail; // Puntero al siguiente viaje
// } tTrip;

// typedef struct trip * TList;

typedef struct station{
    char * name;                // Nombre de la estación
    size_t id;                  // Id de la estación

    struct oldestTrip oldest;
    // TList trips;                // Lista de viajes con origen en esta estación ordenados cronologicamente
    size_t memberTripCount;     // Cantidad de viajes de usuarios que son miembros con origen en esta estación
    size_t notMemberTripCount;  // Cantidad de viajes de usuarios que no son miembros con origen en esta estación
} tStation;

typedef struct bikeCDT{
    size_t stationCount;    // Cantidad de estaciones    
    tStation * stations;    // Vector de estaciones se agrega a medida que se agreguen viajes
    tDay days[NUM_DAYS];    // Vector de struct day 
} bikeCDT;

static int compareStations(tStation s1, tStation s2){
    return s1.id - s2.id;
}

// /**
//  * @param stationsIpt estaciones a agregar
//  * @param stationsOpt destino de las estaciones
//  * @param orderMatrix si es nombre-id o id-nombre
//  * @param stationNbr cantidad de estaciones a agregar
//  * @brief traduce la matriz de char *** al formato de tStation*
// */
// static int matrixToElemVec(char *** stationsIpt, tStation * stationsOpt, enum order orderMatrix, size_t stationNbr){
//     stationsOpt = malloc(stationNbr*sizeof(tStation));
//     int nameIdx;
//     int idIdx;

//     if(orderMatrix == 1){
//         nameIdx = 1;
//         idIdx = 0;
//     }
//     else{
//         nameIdx = 0;
//         idIdx = 1;
//     }
    
//     for(int i = 0; i < stationNbr; i++){
//         errno = 0;
//         stationsOpt[i].name = malloc((strlen(stationsIpt[i][nameIdx]) + 1)*sizeof(char));
//         if(checkErrno(stationsOpt[i].name)){
//             return 0;
//         }
//         strcpy(stationsOpt[i].name,stationsIpt[i][nameIdx]);
//         stationsOpt[i].id = atol(stationsIpt[i][idIdx]);
//     }
//     // Ordeno para hacer búsqueda binaria
//     qsort(stationsOpt, stationNbr, sizeof(tStation), (int (*)(const void *, const void *))compareStations); 
//     return 1;
// }

static bikeADT addStations(bikeADT bikes, stationInput * stations, size_t num){
    bikes->stations = calloc(num,sizeof(tStation));
    bikes->stationCount = num;
    int len;
    for(int i = 0; i < bikes->stationCount; i++){
        bikes->stations[i].id = stations[i].stationID;
        len = 0;
        while(stations[i].name[len]){
            if(len % BLOCK == 0){
                errno = 0;
                bikes->stations[i].name = realloc(bikes->stations[i].name,sizeof(char)*(len + BLOCK));
                if(checkErrno(bikes->stations[i].name)){
                    return NULL;
                }
            }
            bikes->stations[i].name[len] = stations[i].name[len];
            len++;
        }
        errno = 0;
        bikes->stations[i].name = realloc(bikes->stations[i].name,(len+1)*sizeof(char));
        if(checkErrno(bikes->stations[i].name)){
            return NULL;
        }
        bikes->stations[i].name[len] = '\0';
    }

    qsort(bikes->stations, bikes->stationCount, sizeof(tStation), (int (*)(const void *, const void *))compareStations); 

    return bikes;
}

bikeADT newBikeADT(stationInput * stations, size_t numOfStations){
    bikeADT new = calloc(1,sizeof(bikeCDT));
    // new->stationCount = stationNbr;
    // if(matrixToElemVec(stations,new->stations,orderMatrix, stationNbr) == 0){
    //     return NULL; // Si falla la asignación de memoria dinámica, retorno null
    // }
    
    return addStations(new,stations,numOfStations);
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
    d->tm_isdst=-1; // necesario para que funcione la libreria time.h
    return sscanf(date,format,&d->tm_year, &d->tm_mon, &d->tm_mday, &d->tm_hour, &d->tm_min, &d->tm_sec);
}

// /**
//  * @param trips Vector de estaciones
//  * @param stationFrom ID de la estacion de origen
//  * @param stationTo ID de la estacion de destino
//  * @param startDate fecha y hora en la que se inició el viaje
//  * @param endDate fecha y hora en la que se finalizó el viaje
//  * @brief Agrega recursivamente a la lista ordenada cronologicamente un trip  
// */
// static TList addTripRec(TList trips, char * stationTo, size_t stationToId, size_t stationFrom, time_t startDate, time_t endDate, int * flag){
//     double c;
//     if( trips == NULL || ((((c=difftime(startDate,trips->dateStart))) < 0) && (stationToId != stationFrom))){ // para charlar
//         errno = 0;
//         TList aux = malloc(sizeof(tTrip));
//         if(checkErrno(aux)){
//             return NULL;
//         }
//         aux->dateStart = startDate;
//         aux->dateEnd = endDate;
        
//         errno = 0;
//         aux->destName = malloc(strlen(stationTo) + 1);
//         if(checkErrno(aux->destName)){
//             *flag = 1;
//         }
//         strcpy(aux->destName,stationTo);
//         aux->tail = trips;
//         return aux;
//     } else // Si son iguales agrego despues (orden de procesado)
//     {
//         addTripRec(trips->tail, stationTo, stationToId, stationFrom, startDate, endDate, flag);
//         return trips;
//     }
// }

/**
 * Solo agrega si start es menor estricto al tiempo guardado en stationFrom
 * @return 0 si ... 1 si ...
*/
static int checkOldest(tStation * stationFrom, char * destName, time_t start){ 
    if(difftime(start,stationFrom->oldest.dateTime) < 0){
        stationFrom->oldest.dateTime = start;
        free(stationFrom->oldest.stationTo); // Free de NULL no pasa nada, (esta en NULL si es el primer trip de esa estación)
        stationFrom->oldest.stationTo = NULL;
        int len = 0;
        while(destName[len]){
            if(len % BLOCK == 0){
                errno = 0;
                stationFrom->oldest.stationTo = realloc(stationFrom->oldest.stationTo,(len+BLOCK)*sizeof(char));
                if(checkErrno(stationFrom->oldest.stationTo)){
                    return 0;
                }
            }
            stationFrom->oldest.stationTo[len] = destName[len];
            len++;
        }
        errno = 0;
        stationFrom->oldest.stationTo = realloc(stationFrom->oldest.stationTo,(len+1)*sizeof(char));
        if(checkErrno(stationFrom->oldest.stationTo)){
            return 0;
        }
        stationFrom->oldest.stationTo[len] = '\0';
    }
    return 1;
}

void addTrip(bikeADT bikes, unsigned int stationFrom, unsigned int stationTo, char * startDateStr, char * endDateStr, char isMember){
    tStation * foundStationFrom = binarySearch(bikes->stations, stationFrom, bikes->stationCount);
    tStation * foundStationTo = binarySearch(bikes->stations, stationTo, bikes->stationCount);
    if(foundStationFrom == NULL || foundStationTo == NULL ){
        return;
    }
    struct tm startDateTm, endDateTm;
    int val;

    val = strToTime(&startDateTm,startDateStr,"&d-&d-&d &d:&d:&d");
    
    if(val != 6)
        return;
    
    time_t startDate = mktime(&startDateTm);

    val = strToTime(&endDateTm,endDateStr,"&d-&d-&d &d:&d:&d");
    
    if(val != 6) 
        return;
    
    time_t endDate = mktime(&endDateTm);
    
    
    if( isMember ){
        foundStationFrom->memberTripCount++;
    } else {
        foundStationFrom->notMemberTripCount++;
    }

    struct tm * ansStart = gmtime(&startDate); //completa los campos tm_wday y tm_yday no completados en el original
    bikes->days[ansStart->tm_wday].started++; // para el QUERY 3
    
    struct tm * ansEnd = gmtime(&endDate);
    bikes->days[ansEnd->tm_wday].ended++;
    
    if(strcmp(foundStationFrom->name,foundStationTo->name) != 0){ // Si el viaje no es circular... 
        if(checkOldest(foundStationFrom,foundStationTo->name,startDate)==0){
            return;
        }
    }

    return;
}

static int descOrderForTrips(struct tripCounter t1, struct tripCounter t2){
    return (int)(t1.allTrips - t2.allTrips);
}

struct tripCounter * getTotalTrips(bikeADT bikes){
    errno = 0;
    struct tripCounter * retArray = malloc(sizeof(struct tripCounter)*bikes->stationCount);
    if(checkErrno(retArray)){
        return NULL;
    }
    for(size_t i=0; i<bikes->stationCount; i++){
        retArray[i].memberTrips = bikes->stations[i].memberTripCount;
        retArray[i].nonMemberTrips = bikes->stations[i].notMemberTripCount;
        retArray[i].allTrips = retArray[i].memberTrips + retArray[i].nonMemberTrips;
        retArray[i].stationName = malloc(strlen(bikes->stations[i].name) + 1);
        if(checkErrno(retArray[i].stationName)){
            return NULL;
        }
        strcpy(retArray[i].stationName, bikes->stations[i].name);
    }
    qsort(retArray,sizeof(retArray[0]),bikes->stationCount,((int(*) (const void *, const void *))descOrderForTrips));
    return retArray;
}

struct oldestTrip * getOldestTrips(bikeADT bikes){
    struct oldestTrip retArray[(int)(bikes->stationCount)];
    for(int i = 0; i < (int)(bikes->stationCount); i++){
        retArray[i].stationFrom = malloc(strlen(bikes->stations[i].name) + 1);
        strcpy(retArray[i].stationFrom,bikes->stations[i].name);
        retArray[i].stationTo = malloc(strlen(bikes->stations[i].oldest.stationTo) + 1);
        retArray[i].dateTime = bikes->stations[i].oldest.dateTime;
    }
    return retArray;
}

tDay * tripsPerDay(bikeADT bikes){
    return bikes->days;
}

void freeBikes(bikeADT bikes){
    for(int i = 0 ; i < bikes->stationCount; i++){
        free(bikes->stations[i].name);
        free(bikes->stations[i].oldest.stationTo);
    }
    free(bikes->stations);
    free(bikes);
    return;
}

size_t getTotalStations(bikeADT bikes){
    return bikes->stationCount;
}