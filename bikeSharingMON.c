#include "front.h"

#define CANT_COLS_STATIONS_CSV 4
#define CANT_COLS_BIKESMON_CSV 5

#define COLS_STATIONS_FILTER 2
#define COLS_BIKES_FILTER 5
#define IS_MEMBER '1'


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

        char filtroStations[] = {1,1,0,0}; // Porque solamente se necesitan las columnas del nombre y del id
        unsigned int stationsNum = 0;
        
        char *** stationsFilter = toMatrix(stations,CANT_COLS_STATIONS_CSV,filtroStations,&stationsNum); 
        fclose(stations);
        
        stationInput * stns =  matrixToInput(stationsFilter,stationsNum, ID, NAME);
        freeMatrix(stationsFilter,stationsNum,COLS_STATIONS_FILTER);
        
        bikeADT bikesMon = newBikeADT(stns,stationsNum);
        
        for(int i = 0; i < stationsNum; i++){
            free(stns[i].name);
        }
        free(stns);
        
        if(bikesMon == NULL){
            fprintf(stderr, "Error al crear TAD\n");
            return 3;
        }

        char line[MAXCHARS];
        char * token;
        char startDate[MAXCHARS];
        char endDate[MAXCHARS];
        int isFirstRow = 1, col;
        int idStart;
        int idEnd;
        char member;

        while((fgets(line,MAXCHARS,bikes))){ // preparamos los datos para luego ingresarlos al ADT
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
        
        fclose(bikes);
        struct tripCounter * query1 = getTotalTrips(bikesMon);
        if(query1 == NULL){
            fprintf(stderr,"Error al realizar query 1\n");
            return 4;
        }
        
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
        fprintf(query1csv,"%c",'\0');
        fclose(query1csv);
        closeHTMLTable(tableForQ1);

        for(int i = 0; i < getTotalStations(bikesMon); i++){
            free(query1[i].stationName);
        }
        free(query1);
        int dimq2;
        struct oldestTrip * query2 = getOldestTrips(bikesMon,&dimq2);
        if(query2 == NULL){
            fprintf(stderr,"Error al realizar query 2\n");
            return 5;
        }
        char * colNamesQ2[] = {"bikeStation","bikeEndStation","oldestDateTime"};
        htmlTable tableForQ2 = newTable("query2.html",COLS_QUERY_2,colNamesQ2[0],colNamesQ2[1],colNamesQ2[2]);
        FILE * query2csv;
        query2csv = fopen("query2.csv","w");
        fprintf(query2csv,"%s;%s;%s\n",colNamesQ2[0],colNamesQ2[1],colNamesQ2[2]);
        char timeStr[LEN_DATE_Q2];
        struct tm * tmPtr;
        char * formatQ2 = "%d/%m/%Y %H:%M";
        for(int i = 0; i < dimq2; i++){
            tmPtr = localtime(&(query2[i].dateTime));
            strftime(timeStr,sizeof(timeStr),formatQ2,tmPtr);
            addHTMLRow(tableForQ2,query2[i].stationFrom,query2[i].stationTo,timeStr);
            fprintf(query2csv,"%s;%s;%s\n",query2[i].stationFrom,query2[i].stationTo,timeStr);
        }
        closeHTMLTable(tableForQ2);
        fprintf(query2csv,"%c",'\0');
        fclose(query2csv);
        for(int i = 0; i < dimq2; i++){
            free(query2[i].stationFrom);
            free(query2[i].stationTo);
        }
        free(query2);
        
        tDay * query3 = tripsPerDay(bikesMon);
        if(query3 == NULL){
            fprintf(stderr,"Error al realizar query 3\n");
            return 6;
        }
        
        char * colsQuery3CSV[] = {"weekDay","startedTrips","endedTrips"};
        htmlTable tableForQ3 = newTable("query3.html",COLS_QUERY_3,colsQuery3CSV[0],colsQuery3CSV[1],colsQuery3CSV[2]);
        FILE * query3csv;
        query3csv = fopen("query3.csv","w");
        char * days[DAYS_IN_WEEK] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
        fprintf(query3csv,"%s;%s;%s\n",colsQuery3CSV[0],colsQuery3CSV[1],colsQuery3CSV[2]);
        for(int i = 0; i < DAYS_IN_WEEK; i++){
            char started[MAXCHARS] = {0};
            char ended[MAXCHARS] = {0};
            sprintf(started,"%ld",query3[CONVERSION(i)].started);
            sprintf(ended,"%ld",query3[CONVERSION(i)].ended);
            addHTMLRow(tableForQ3,days[i],started,ended);
            fprintf(query3csv,"%s;%ld;%ld\n",days[i],query3[CONVERSION(i)].started, query3[CONVERSION(i)].ended);
        }
        fprintf(query3csv,"%c",'\0');
        fclose(query3csv);
        free(query3);
        closeHTMLTable(tableForQ3);
        freeBikes(bikesMon);
    return 0;
}


