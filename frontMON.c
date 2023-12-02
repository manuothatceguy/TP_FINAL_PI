#include <stdio.h>
#include <stdlib.h>
#include "bikeADT/bikeADT.h" // despues arreglar estilo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "csvTools/csvTools.h"    // tambien revisar estiloo!!!!!!!!!!!!!!!!

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
        int stationsNum = 0;
        int tripNum = 0;
        char *** stationsFilter = toMatrix(stations,CANT_COLS_STATIONS_CSV,filtroStations,&stationsNum);
        char *** tripsFilter = toMatrix(bikes,5,filtroTrips,&tripNum);
        bikeADT bikesMon = newBikeADT(stationsFilter,stationsNum,0);
        for(int i = 0; i < tripNum; i++){
            addTrip(bikesMon,atoi(tripsFilter[i][ID_START]),atoi(tripsFilter[i][ID_END]),tripsFilter[i][START_DATE],tripsFilter[i][END_DATE],*tripsFilter[i][MEMBER_STATUS]);
        }

        

        fclose(bikes);
        fclose(stations);
    }
    
    
    return 0;
}
