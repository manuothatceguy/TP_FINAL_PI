#include "bikeADT.h"
#include "checkErrno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#define NUM_DAYS 7
#define BLOCK 10

typedef struct station{
    char * name;                // Nombre de la estación
    int id;                  // Id de la estación

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

static int comparator(const void* p, const void* q)
{
   int l = ((tStation*)p)->id;
   int r = ((tStation*)q)->id;
   return (l - r);
}

static bikeADT addStations(bikeADT bikes, stationInput * stations, size_t num){
    bikes->stations = calloc(num,sizeof(tStation));
    bikes->stationCount = num;
    int len;
    int i ;
    for(i = 0; i < bikes->stationCount; i++){
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
    printf("%d",i);
    qsort((void*)(bikes->stations), bikes->stationCount, sizeof(bikes->stations[0]), comparator); 

    return bikes;
}

bikeADT newBikeADT(stationInput * stations, size_t numOfStations){
    bikeADT new = calloc(1,sizeof(bikeCDT));
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
    int medio = dim/2;
    if(stations[medio].id > id){
        return binarySearch(stations,id,(int)(dim/2));
    } else if(stations[(int)(dim/2)].id < id){
        return binarySearch(stations+medio+1,id,dim - medio - 1);
    } else{
        return stations + medio;
    }
}

static void strToTime(struct tm *d, char * date, char * format){
    d->tm_isdst=-1; // necesario para que funcione la libreria time.h
    sscanf(date,format,&d->tm_year, &d->tm_mon, &d->tm_mday, &d->tm_hour, &d->tm_min, &d->tm_sec);
    return;
}

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


    strToTime(&startDateTm,startDateStr,"&d-&d-&d &d:&d:&d");
    
    time_t startDate = mktime(&startDateTm);

    strToTime(&endDateTm,endDateStr,"&d-&d-&d &d:&d:&d");
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
        checkOldest(foundStationFrom,foundStationTo->name,startDate);
    }

    return;
}

static int tripOrder(const void* p, const void* q){
    int l = ((struct tripCounter*)p)->allTrips;
    int r = ((struct tripCounter*)q)->allTrips;
    if((r - l) == 0){
        return strcmp(((struct tripCounter*)p)->stationName,((struct tripCounter*)q)->stationName);
    }
    return (r - l);
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
    qsort((void*)retArray,bikes->stationCount,sizeof(retArray[0]),tripOrder);
    return retArray;
}

struct oldestTrip * getOldestTrips(bikeADT bikes){
    struct oldestTrip * retArray = malloc((bikes->stationCount) * sizeof(struct oldestTrip));
    for(int i = 0; i < (int)(bikes->stationCount); i++){
        retArray[i].stationFrom = malloc(strlen(bikes->stations[i].name) + 1);
        strcpy(retArray[i].stationFrom,bikes->stations[i].name);
        if(bikes->stations[i].memberTripCount != 0 || bikes->stations[i].notMemberTripCount != 0){    
            retArray[i].stationTo = malloc(strlen(bikes->stations[i].oldest.stationTo) + 1);
            strcpy(retArray[i].stationTo,bikes->stations[i].oldest.stationTo);
            retArray[i].dateTime = bikes->stations[i].oldest.dateTime;
        } else{
            retArray[i].stationTo = malloc();
        }

    }
    return retArray;
}

tDay * tripsPerDay(bikeADT bikes){
    
    errno = 0;
    tDay * toReturn = malloc(NUM_DAYS*(sizeof(toReturn[0])));
    
    if(checkErrno(toReturn)){
        return NULL;
    }

    for(int i=0;i<NUM_DAYS;i++){
        toReturn[i]=bikes->days[i];
    }

    return toReturn;
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