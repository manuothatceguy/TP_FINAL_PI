#ifndef CSVTOOLS_H
#define CSVTOOLS_H

#include <stdio.h>
#include <string.h>

/**
**  Implementación reducida de una biblioteca de funciones para manipular archivos csv en C
**  Solo incluye las funciones toMatrix y freeMatrix.
*/

/**
 *  @param fp archivo csv para pasar a matrizx 
 *  @param colNbr cantidad de columnas del archivo fuente
 *  @param colsToFilter vector con 0 o 1 si se agrega a la matriz destino esa columna o no
 *  @param rows parámetro de salida: cantidad de filas de la matriz (la cantidad de columnas se sabe por el vector colsToFilter)
 *  @return matriz resultante de filtrar el archivo csv
 *  @brief dado un archivo csv, se lo filtra y se transforma a una matriz de char*
*/
char *** toMatrix(FILE * fp, unsigned int colNbr, char colsToFilter[], unsigned int * rows);

/**
 *  @param m matriz a liberar 
 *  @param rowNbr cantidad de filas
 *  @param colNbr cantidad de columnas
 *  @brief libera la memoria reservada por la matriz
*/
void freeMatrix(char *** matrix, unsigned int rowNbr, unsigned int colNbr);

#endif
