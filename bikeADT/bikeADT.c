#include "bikeADT.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_DAYS 7

typedef struct station{
    char * name;
    size_t id;
    
} tStation;

typedef struct day{
    size_t started;
    size_t ended;
} tDay;

typedef struct bikeCDT{
    size_t stationCount;
    tStation * stations;
    tDay days[NUM_DAYS];

} bikeCDT; 