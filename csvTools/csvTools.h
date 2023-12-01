#ifndef CSVTOOLS_H
#define CSVTOOLS_H


#include <stdio.h>
#include <string.h>

/**
 * @param fp archivo CSV
 * @param row fila a extraer del archivo (arrancando por la 0)
 * 
 * 
 * 
*/
char ** getRow(FILE * fp, unsigned int row, int * colNbr);

#endif