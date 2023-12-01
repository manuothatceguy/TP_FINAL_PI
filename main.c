#include <stdio.h>
#include <stdlib.h>


int
main(void){
    int num;
    printf("ingrese un numero entero\n");
    int rta=scanf("%d",&num);
    if (rta!=1){
        printf("ERROR!\n");
    }else{
        printf("el numero ingresado es %d\n",num);
    }
    return 0;
}