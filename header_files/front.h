#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bikeADT.h" 
#include "htmlTable.h"
#include "checkErrno.h"

/**
 * ARCHIVO DE ENCABEZADO PARA LOS ARCHIVOS "bikeSharingMON.c" y "bikeSharingNYC.c"
*/

#define MAXCHARS 200                // Tope arbitrario.
#define COLS_QUERY_1 4
#define COLS_QUERY_2 3
#define COLS_QUERY_3 3
#define DAYS_IN_WEEK 7
#define BLOCK 10
#define CONVERSION(x) (((x)+1)%7)   // Debido al formato del time.h para que el primero sea el LUNES
#define LEN_DATE_Q2 17              // El formato de fecha que se retorna en el query 2 tiene una longitud de 17 caracteres 