#include <stdio.h>
#include <stdlib.h>
#include "bikeADT.h" 
#include "csvTools.h"  
#include "htmlTable.h"
#include "checkErrno.h"

#define CANT_COLS_STATIONS_CSV 4
#define CANT_COLS_BIKESMON_CSV 5
#define MAXCHARS 250 // Para el sprintf. Tope arbitrario.
#define COLS_QUERY_1 4
#define COLS_QUERY_2 3
#define COLS_QUERY_3 3
#define DAYS_IN_WEEK 7
#define COLS_STATIONS_FILTER 2
#define COLS_BIKES_FILTER 5
#define IS_MEMBER '1'
#define BLOCK 10
#define CONVERSION(x) (((x)+6)%7)  // Debido al formato del time.h para que el primero sea el LUNES

enum posInCsv {START_DATE = 0, ID_START, END_DATE, ID_END, MEMBER_STATUS};
enum stationPos {ID = 0, NAME};

stationInput * matrixToInput(char ***stations, int num);

int main(int argc, char const *argv[])
{
    if(argc < 2 || argc > 4){
        fprintf(stderr,"Error de argumentos\n");
        return 1;
    }

    else{
        FILE * bikes, * stations;
        bikes = fopen(argv[1],"r");
        stations = fopen(argv[2],"r");

        if( bikes == NULL || stations == NULL){
         fprintf(stderr, "Error al abrir archivos\n");
         return 2;
        }

        puts("Abri los archivos"); // abre bien
        char filtroStations[] = {1,1,0,0}; // Porque solamente se necesitan las columnas del nombre y del id
        unsigned int stationsNum = 0;
        puts("Estoy por arrancar a filtrar");
        char *** stationsFilter = toMatrix(stations,CANT_COLS_STATIONS_CSV,filtroStations,&stationsNum); 
        for(int i=0; i < stationsNum; i++){
            for(int j = 0; j < COLS_STATIONS_FILTER; j++){
                printf("%s\t", stationsFilter[i][j]);
            }
            puts("\n");
        }
        puts("Filtré");
        stationInput * stns =  matrixToInput(stationsFilter,stationsNum);
        freeMatrix(stationsFilter,stationsNum,COLS_STATIONS_FILTER);
        puts("Creé el vector");
        bikeADT bikesMon = newBikeADT(stns,stationsNum);

        if(bikesMon == NULL){
            fprintf(stderr, "Error al crear TAD\n");
            return 3;
        }

        puts("Se creó el TAD vacío");

        // Agregar viajes
        char line[MAXCHARS];
        char * token;
        char startDate[MAXCHARS];
        char endDate[MAXCHARS];
        int isFirstRow = 1, col;
        int idStart;
        int idEnd;
        char member;

        while((fgets(line,MAXCHARS,bikes))){
            col=0;
            if(!isFirstRow){
                token = strtok(line,";");
                strcpy(startDate,token);
                for(col = 1; col < CANT_COLS_BIKESMON_CSV; col++){
                    token = strtok(NULL,";");
                    switch (col)
                    {
                    case 1:
                        idStart = atoi(token);
                        break;
                    case 2:
                        strcpy(endDate,token);
                        break;
                    case 3:
                        idEnd = atoi(token);
                        break;
                    default:
                        member = (*token == IS_MEMBER); //para que guarde 0 y 1 (y no 48 y 49)
                        break;
                    }
                }
                addTrip(bikesMon,idStart,idEnd,startDate,endDate,member);
            } 
            isFirstRow=0;
        }
        

        puts("Agregamos los trips");
        struct tripCounter * query1 = getTotalTrips(bikesMon);
        if(query1 == NULL){
            fprintf(stderr,"Error al realizar query 1\n");
            return 4;
        }
        puts("Se creó el query 1");
        char * colNamesQ1[] = {"bikeStation","memberTrips","casualTrips","allTrips"};
        htmlTable tableForQ1 = newTable("query1.html",COLS_QUERY_1,colNamesQ1[0],colNamesQ1[1],colNamesQ1[2],colNamesQ1[3]);
        char table[CANT_COLS_STATIONS_CSV][MAXCHARS];
        FILE * query1csv;
        query1csv = fopen("query1.csv","w");
        fprintf(query1csv,"%s;%s;%s;%s\n",colNamesQ1[0],colNamesQ1[1],colNamesQ1[2],colNamesQ1[3]);
        for(int i = 0; i < stationsNum; i++){
            sprintf(table[0],"%s",query1[i].stationName);
            sprintf(table[1],"%ld",query1[i].memberTrips);
            sprintf(table[2],"%ld",query1[i].nonMemberTrips);
            sprintf(table[3],"%ld",query1[i].allTrips);
            fprintf(query1csv,"%s;%s;%s;%s\n",table[0],table[1],table[2],table[3]);
            addHTMLRow(tableForQ1,table[0],table[1],table[2],table[3]);
        }
        fprintf(query1csv,"\0");
        fclose(query1csv);
        closeHTMLTable(tableForQ1);
        puts("Se creó el HTML del query 1");

        struct oldestTrip * query2 = getOldestTrips(bikesMon);
        if(query2 == NULL){
            fprintf(stderr,"Error al realizar query 2\n");
            return 5;
        }
        puts("Se creó el query 2");
        htmlTable tableForQ2 = newTable("query2.html",COLS_QUERY_2,"bikeStation","bikeEndStation","oldestDateTime");
        FILE * query2csv;
        query2csv = fopen("query2.csv","w");
        for(int i = 0; i < stationsNum; i++){
            addHTMLRow(tableForQ2,query2->stationFrom,query2->stationTo,query2->dateTime);
        }
        closeHTMLTable(tableForQ2);
        puts("Se creó el HTML del query 2");
        tDay * query3 = tripsPerDay(bikesMon);
        if(query3 == NULL){
            fprintf(stderr,"Error al realizar query 3\n");
            return 6;
        }
        puts("Se creó el query 3");
        htmlTable tableForQ3 = newTable("query3.html",COLS_QUERY_3,"weekDay","startedTrips","endedTrips");
        char * days[DAYS_IN_WEEK] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
        for(int i = 0; i < DAYS_IN_WEEK; i++){
            char started[MAXCHARS] = {0};
            char ended[MAXCHARS] = {0};
            sprintf(started,"%ld",query3[CONVERSION(i)].started);
            sprintf(ended,"%ld",query3[CONVERSION(i)].ended);
            addHTMLRow(tableForQ3,days[i],started,ended);
        }
        puts("Se creó el HTML del query 3");
        for(int i = 0; i < stationsNum; i++){
            free(query1[i].stationName);
            free(query2[i].stationFrom);
            free(query2[i].stationTo);
        }
        free(query1);
        free(query2);

        
        closeHTMLTable(tableForQ3);
        freeBikes(bikesMon);
        fclose(bikes);
        fclose(stations);
        puts("Memoria liberada");
    }
    puts("OK!!");
    return 0;
}

stationInput * matrixToInput(char ***stations, int num){
    errno = 0;
    stationInput * retArray = malloc(num*sizeof(stationInput));
    if(checkErrno(retArray)){
        printf("Error");
        return NULL;
    }
    for(int i = 0; i < num; i++){
        retArray[i].stationID = atol(stations[i][ID]);
          errno = 0;
         retArray[i].name = malloc(sizeof(char) * (strlen(stations[i][NAME]) + 1));
         if(checkErrno(retArray[i].name)){
             return NULL;
        }
        strcpy(retArray[i].name,stations[i][NAME]);
    }
    return retArray;
}