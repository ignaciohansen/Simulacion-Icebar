#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define EMPLEADOS_BARRA_PRIMER_PISO 3
#define EMPLEADOS_BARRA_HIELO 4

struct tm T;

int main()
{

    iniciarCondicionesIniciales();

    int i = buscarMinimoTPSPrimerPiso();
    int j = buscarMinimoTPSCamaraHielo();
    printf("Hello %d;\n",EMPLEADOS_BARRA_PRIMER_PISO);
    return 0;
}


int buscarMinimoTPSPrimerPiso()
{

    return 0;
}

int buscarMinimoTPSCamaraHielo()
{

    return 0;
}


void iniciarCondicionesIniciales()
{
    inicializarTiempo();
    return;
}

void inicializarTiempo()
{
    T.tm_sec = 0;
    T.tm_min = 0;
    T.tm_hour = 0;
    T.tm_mday = 1;
    T.tm_mon = 0;
    T.tm_year = 2018;
    T.tm_wday = 0;
    T.tm_yday = 0;
    T.tm_isdst = 0;
}
