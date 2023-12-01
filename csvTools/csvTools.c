#include "csvTools.h"
#include <errno.h>
#include <stdlib.h>

#define MAXCHAR 100
#define BLOCK 10

static int checkErrno(void * ptr){
    if(ptr == NULL || errno == ENOMEM){
        errno = ENOMEM;
        return 1;
    }
    return 0;
}
                            
char *** toMatrix(FILE * fp, unsigned int colNbr, char colsToFilter[], unsigned int * rowsNew){ // colNbr es la dimension de colsToFilter

    char row[MAXCHAR];
    char *token;
    int isFirstRow = 1; //Flag para no copiar la primera fila con los títulos
    char*** retMatrix = NULL;
    int len = 0;    // Longitud del token copiado
    int copy = 0;   // Cantidad de filas de la matriz destino
    int col = 0;    // Indice de la columna de la matriz fuente.
    int newColDim = 0;  // Indice de la columna de la matriz destino.
    for(int i = 0; i < colNbr; i++){
        if(colsToFilter[i])
            newColDim++;
    }
    int newColIdx;
    while (!feof(fp))
    {   
        col = 0;
        newColIdx = 0;
        fgets(row, MAXCHAR, fp);

        token = strtok(row, ";");
        int valid;

        while(token != NULL)
        {
            valid = (colsToFilter[col])==1;
            if(isFirstRow != 1){
                if(copy % BLOCK == 0){
                    errno = 0;
                    retMatrix = realloc(retMatrix,(copy+BLOCK)*sizeof(char**));
                    if(checkErrno((void*)retMatrix))
                        return NULL;
                    for(int i = copy; i < copy+BLOCK; i++){
                        errno = 0;
                        retMatrix[i] = realloc(retMatrix[i],(newColDim)*sizeof(char*));
                        if(checkErrno(retMatrix[i])){
                            return NULL;
                        }
                    }
                }

                len = 0;

                if(valid){
                    while(*token != '\0' && *token != '\n'){
                        if(len % BLOCK == 0){
                        errno = 0;
                        retMatrix[copy][newColIdx] = realloc(retMatrix[copy][newColIdx],(len + BLOCK));
                        if(checkErrno((void*)retMatrix[copy][newColIdx]))
                            return NULL;
                        }
                        retMatrix[copy][newColIdx][len++] = *token;
                        token++;
                    }
                    errno=0;
                    retMatrix[copy][newColIdx] = realloc(retMatrix[copy][newColIdx],(len)*sizeof(char));
                    if(checkErrno((void*)retMatrix[copy][newColIdx])){
                        return NULL;
                    }
                    retMatrix[copy][newColIdx++][len] = '\0';
                }
                col++;
            }
            token = strtok(NULL, ";");
        }
        isFirstRow = 0; // apago flag
        
    }
    *rowsNew = copy;    // Así se sabe hasta donde acceder, ya que la matriz no tiene marca de fin.
    return retMatrix;
}
