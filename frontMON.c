#include <stdio.h>
#include <stdlib.h>
#include "bikeADT/bikeADT.h" // despues arreglar estilo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


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

        while((row = getRow(stations)) != NULL){
            if(stationCount != 0){
                addStation(row[1],atoi(row[0]));
            }
            stationCount++;
        }

        fclose(bikes);
        fclose(stations);
    }
    
    
    return 0;
}
