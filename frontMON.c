#include <stdio.h>
#include <stdlib.h>
#include "bikeADT/bikeADT.h" // despues arreglar estilo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "csvTools/csvTools.h"    // tambien revisar estiloo!!!!!!!!!!!!!!!!

#define CANT_COLS_STATIONS_CSV 4

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
        
        char ** row;
        int stationCount = 0;
        elemVec * stations;

        char filtro[] = {1,1,0,0}; // Porque solamente se necesitan las columnas del nombre y del id

        int fils = 0;
        char *** filter = toMatrix(stations,CANT_COLS_STATIONS_CSV,filtro,&fils);

        elemVec stationNameId[fils];

        

        fclose(bikes);
        fclose(stations);
    }
    
    
    return 0;
}
