#include "front.h"

stationInput * matrixToInput(char ***stations, int num, int id_pos, int name_pos){
    errno = 0;
    stationInput * retArray = malloc(num*sizeof(stationInput));
    if(checkErrno(retArray)){
        printf("Error");
        return NULL;
    }
    for(int i = 0; i < num; i++){
        retArray[i].stationID = atol(stations[i][id_pos]);
        errno = 0;
        retArray[i].name = malloc(sizeof(char) * (strlen(stations[i][name_pos]) + 1));
        if(checkErrno(retArray[i].name)){
            return NULL;
        }
        strcpy(retArray[i].name,stations[i][name_pos]);
    }
    return retArray;
}

