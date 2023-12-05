#include "front.h"

#define CANT_COLS_STATIONS_CSV 4
#define CANT_COLS_BIKESMON_CSV 5

#define COLS_STATIONS_FILTER 2
#define COLS_BIKES_FILTER 5


enum posInCsv {START_DATE = 0, ID_START, END_DATE, ID_END, MEMBER_STATUS};
enum stationPos {ID = 0, NAME};

int main(int argc, char const *argv[])
{
    if(argc < 2 || argc > 4){
        fprintf(stderr,"Error de argumentos\n");
        return 1;
    }

    FILE * bikes, * stations;
    bikes = fopen(argv[1],"r");
    stations = fopen(argv[2],"r");

    if( bikes == NULL || stations == NULL){
        fprintf(stderr, "Error al abrir archivos\n");
        return 2;
    }    

    char filtroStations[] = {1,0,0,1};


    return 0;
}
