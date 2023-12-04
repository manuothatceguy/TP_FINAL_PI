#include <stdio.h>
#include <stdlib.h>
#include "header_files/bikeADT.h" // despues arreglar estilo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "header_files/csvTools.h"    // tambien revisar estiloo!!!!!!!!!!!!!!!!
#include "header_files/htmlTable.h"

#define CANT_COLS_STATIONS_CSV 4
#define CANT_COLS_BIKESMON_CSV 5
#define MAXCHARS 50 // Para el sprintf. Tope arbitrario.
#define COLS_QUERY_1 4
#define COLS_QUERY_2 3
#define COLS_QUERY_3 3
#define DAYS_IN_WEEK 7
#define CONVERSION(x) ((x)+1)%7  // Debido al formato del time.h para que el primero sea el LUNES

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
            exit(3);
        }
        
        int j;

        for(int i = 0; i < tripNum; i++){
            j = addTrip(bikesMon,atoi(tripsFilter[i][ID_START]),atoi(tripsFilter[i][ID_END]),tripsFilter[i][START_DATE],tripsFilter[i][END_DATE],*tripsFilter[i][MEMBER_STATUS]);
            if(j == 0)
                fprintf(stderr,"Viaje no agregado");
        }

        struct tripCounter * query1 = getTotalTrips(bikesMon);
        if(query1 == NULL){
            fprintf(stderr,"Error al realizar query 1");
            exit(4);
        }
        
        htmlTable tableForQ1 = newTable("query1.html",COLS_QUERY_1,"bikeStation","memberTrips","casualTrips","allTrips");
        for(int i = 0; i < tripNum; i++){
            char table[CANT_COLS_STATIONS_CSV][MAXCHARS] = {{0}}; 
            sprintf(table[0],"%s",query1[i].stationName);
            sprintf(table[1],"%ld",query1[i].memberTrips); // PARA CORREGIR tira un warning del tipo de dato, noc q opinan (se soluciona casteando a long)
            sprintf(table[2],"%ld",query1[i].nonMemberTrips);
            sprintf(table[3],"%ld",query1[i].allTrips);
            addHTMLRow(tableForQ1,table[0],table[1],table[2],table[3]);
        }
        
        struct oldestTrip * query2 = getOldestTrips(bikesMon);
        if(query2 == NULL){
            fprintf(stderr,"Error al realizar query 2");
            exit(5);
        }
        
        htmlTable tableForQ2 = newTable("query2.html",COLS_QUERY_2,"bikeStation","bikeEndStation","oldestDateTime");
        for(int i = 0; i < stationsNum; i++){
            addHTMLRow(tableForQ2,query2->stationFrom,query2->stationTo,query2->dateTime);
        }
        
        tDay * query3 = tripsPerDay(bikesMon);
        if(query3 == NULL){
            fprintf(stderr,"Error al realizar query 3");
            exit(6);
        }
        
        htmlTable tableForQ3 = newTable("query3.html",COLS_QUERY_3,"weekDay","startedTrips","endedTrips");
        char * days[DAYS_IN_WEEK] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
        for(int i = 0; i < DAYS_IN_WEEK; i++){
            char started[MAXCHARS] = {0};
            char ended[MAXCHARS] = {0};
            sprintf(started,"%ld",query3[CONVERSION(i)].started);
            sprintf(ended,"%ld",query3[CONVERSION(i)].ended);
            addHTMLRow(tableForQ3,days[i],started,ended);
        }
        
        for(int i = 0; i < stationsNum; i++){
            free(query1[i].stationName);
            free(query2[i].stationFrom);
            free(query2[i].stationTo);
            free(query2[i].dateTime);
        }

        free(query1);
        free(query2);
        closeHTMLTable(tableForQ1);
        closeHTMLTable(tableForQ2);
        freeBikes(bikesMon);
        fclose(bikes);
        fclose(stations);
    }
    return 0;
}
