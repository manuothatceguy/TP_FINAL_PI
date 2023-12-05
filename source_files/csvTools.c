#include "csvTools.h"
#include "checkErrno.h"
#include <errno.h>
#include <stdlib.h>

#define MAXCHAR 500
#define BLOCK 10
                            
char ***toMatrix(FILE *fp, unsigned int colNbr, char colsToFilter[], unsigned int *rows) {
    char row[MAXCHAR];
    char *token;
    int isFirstRow = 1;     // Flag para no copiar la primera fila con los títulos
    char ***retMatrix = NULL;
    size_t copy = 0;           // Cantidad de filas de la matriz destino
    int newColDim = 0;      // Indice de la columna de la matriz destino.

    for (size_t i = 0; i < colNbr; i++) {
        if(colsToFilter[i]){
            newColDim++;
        }
    }

    while (fgets(row, MAXCHAR, fp) != NULL) {
        if (isFirstRow != 1) {
            if (copy % BLOCK == 0) {
                errno = 0;
                retMatrix = realloc(retMatrix, (copy + BLOCK) * sizeof(char**));
                if (checkErrno((void *)retMatrix))
                    return NULL;

                for (size_t i = copy; i < copy + BLOCK; i++) {
                    errno = 0;
                    retMatrix[i] = calloc(newColDim, sizeof(char*));
                    if (checkErrno(retMatrix[i])){
                        return NULL;
                    }
                }
            }

            int col = 0;
            int newColIdx = 0;

            token = strtok(row, ";");

            while (token != NULL) {
                if (colsToFilter[col]) {
                    size_t len = strlen(token);

                    // Asegurarse de tener suficiente espacio en la memoria para la cadena
                    retMatrix[copy][newColIdx] = realloc(retMatrix[copy][newColIdx], (len + 1)*sizeof(char));
                    if (checkErrno((void *)retMatrix[copy][newColIdx])) {
                        return NULL;
                    }
                    
                    strcpy(retMatrix[copy][newColIdx], token);
                    newColIdx++;
                }

                col++;
                token = strtok(NULL, ";");
            }

            copy++;
        }

        isFirstRow = 0; // apago flag
    }

    for(int i = copy; i < ((copy/BLOCK)+1)*BLOCK; i++){ // Al usar block quedan reservadas mas filas de las que se utilizan
        free(retMatrix[i]);                             // liberamos cada fila extra
    }

    // No es necesario una vez liberada la zona de memoria no utilizada
    //
    // errno = 0;
    // retMatrix = realloc(retMatrix,(copy)*sizeof(char**));
    // if(checkErrno(retMatrix)){
    //     return  NULL;
    // }

    *rows = copy;
    return retMatrix;
}

void freeMatrix(char *** matrix, unsigned int rowNbr, unsigned int colNbr){
    for(int i = 0; i < rowNbr; i++){
        for(int j = 0; j < colNbr; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
    return;
}
