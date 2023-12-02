#include <stdio.h>
#include <stdlib.h>
#include "bikeADT/bikeADT.h" // despues arreglar estilo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "csvTools/csvTools.h"    // tambien revisar estiloo!!!!!!!!!!!!!!!!
#include "CTable/htmlTable.h"

#define CANT_COLS_STATIONS_CSV 4
#define CANT_COLS_BIKESMON_CSV 5

enum posInCsv {START_DATE = 0, ID_START, END_DATE, ID_END, MEMBER_STATUS};

int main(int argc, char const *argv[])
{
    if(argc < 2 || argc > 4){
        fprintf(stderr,"Error de argumentos");
        exit(1);
    }
    else{
        FILE * bikes, * stations;
        bikes = fopen(argv[1],"r");
        stations = fopen(argv[2],"r");

        if( bikes == NULL || stations == NULL){
         fprintf(stderr, "Error al abrir archivos");
         exit(2);
        }
        
        char filtroStations[] = {1,1,0,0}; // Porque solamente se necesitan las columnas del nombre y del id
        char filtroTrips[] = {1,1,1,1,1}; // Porque necesito todas las columnas, en NYC no necesitaría el tipo de bicicleta, por ejemplo.
        
        unsigned int stationsNum = 0;
        unsigned int tripNum = 0;

        char *** stationsFilter = toMatrix(stations,CANT_COLS_STATIONS_CSV,filtroStations,&stationsNum);
        char *** tripsFilter = toMatrix(bikes,5,filtroTrips,&tripNum);
        bikeADT bikesMon = newBikeADT(stationsFilter,stationsNum,0);
        if(bikesMon == NULL){
            fprintf(stderr, "Error al crear TAD");
            exit(4);
        }
        
        int j;

        for(int i = 0; i < tripNum; i++){
            j = addTrip(bikesMon,atoi(tripsFilter[i][ID_START]),atoi(tripsFilter[i][ID_END]),tripsFilter[i][START_DATE],tripsFilter[i][END_DATE],*tripsFilter[i][MEMBER_STATUS]);
            if(j == 0)
                fprintf(stderr,"Viaje no agregado");
        }

        struct tripCounter * query1 = getTotalTrips(bikesMon);
        
        htmlTable tableForQ1 = newTable("query1.html",4,"bikeStation","memberTrips","casualTrips","allTrips");
        for(int i = 0; i < tripNum; i++){
            char table[4][50]; // ojo que aca hay "magic numbers "  mejor poner defines y usarlos ahi
            sprintf(table[0],"%s",query1[i].stationName);
            sprintf(table[1],"%ld",query1[i].memberTrips);
            sprintf(table[2],"%ld",query1[i].nonMemberTrips);
            sprintf(table[3],"%ld",query1[i].allTrips);
            addHTMLRow(table[0],table[1],table[2],table[3]);
        }
        closeHTMLTable(query1);
        freeBikes(bikesMon);
        fclose(bikes);
        fclose(stations);
    }
    return 0;
}
