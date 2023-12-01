#include "csvTools.h"
#include <errno.h>
#include <stdlib.h>

#define MAXCHAR 1000
#define BLOCK 10

static int checkErrno(void * ptr){
    if(ptr == NULL || errno == ENOMEM){
        errno = ENOMEM;
        return 1;
    }
    return 0;
}
                            
char *** toMatrix(FILE * fp, int * colNbr){

    char row[MAXCHAR];
    char *token;
    int isFirstRow = 1;
    char *** retMatrix = NULL;
    int dim;
    int copy = 0;

    while (!feof(fp))
    {
        
        fgets(row, MAXCHAR, fp);
        int col = 0;
        token = strtok(row, ";");

        while(token != NULL)
        {
            if(isFirstRow != 1){
                if(copy % BLOCK == 0){
                    errno = 0;
                    retMatrix = realloc(retMatrix,(copy+BLOCK)*sizeof(char**));
                    if(checkErrno((void*)retMatrix))
                        return NULL;
                }
                
                dim = 0;

                while(*token != '\0' && *token != '\n'){
                    if(dim % BLOCK == 0 ){
                        errno = 0;
                        retMatrix[copy][col] = realloc(retMatrix[copy][col],(dim + BLOCK));
                        if(checkErrno((void*)retMatrix[copy][col]))
                            return NULL;
                    }
                }
            }
            token = strtok(NULL, ";");
        }
        isFirstRow = 0; // apago flag
        
    }

}