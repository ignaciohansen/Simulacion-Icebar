#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIEMPO_FINAL 10

#define MINUTOS_SEMANA 10080

#define EMPLEADOS_BARRA_PRIMER_PISO 3
#define EMPLEADOS_BARRA_HIELO 4

#define PORCENTAJE_DESCUENTO_CLASICO 60
#define PORCENTAJE_DESCUENTO_ESPECIAL 30

#define SALIDA_BARRA_PRIMER_PISO 1
#define SALIDA_BARRA_HIELO 2
#define LLEGADA 3

#define HIGH_VALUE 10000000000000000000

//Tiempos
int TF = TIEMPO_FINAL;
int T;
int TPSN[EMPLEADOS_BARRA_PRIMER_PISO];
int TPSM[EMPLEADOS_BARRA_HIELO];
int TPLL;

//Tiempos Ociosos
int ITON[EMPLEADOS_BARRA_PRIMER_PISO];
int ITOM[EMPLEADOS_BARRA_HIELO];
int STON[EMPLEADOS_BARRA_PRIMER_PISO];
int STOM[EMPLEADOS_BARRA_HIELO];

//Acumuladores
int SPSN = 0;
int SPSM = 0;
int STAN = 0;
int STAM = 0;
float GPE = 0;
float GPC = 0;
int NSN = 0;
int NSM = 0;
int NTN = 0;
int NTM = 0;

//Resultados
float PECN = 0;
float PECM = 0;
float PTAN = 0;
float PTAM = 0;
float GTEV = 0;
float GTCV = 0;
float PTON[EMPLEADOS_BARRA_PRIMER_PISO];
float PTOM[EMPLEADOS_BARRA_HIELO];

int main()
{

    iniciarCondicionesIniciales();

    //while (T <= TF)
   // {

        int puestoBarraPrimerPiso = buscarMinimoTPSPrimerPiso();
        int puestoBarraCamaraHielo = buscarMinimoTPSCamaraHielo();

        int evento = determinarProximoEvento(puestoBarraPrimerPiso,puestoBarraCamaraHielo);

        switch(evento)
        {
        case SALIDA_BARRA_PRIMER_PISO: salidaBarraPrimerPiso(puestoBarraPrimerPiso);
            break;
        case SALIDA_BARRA_HIELO: salidaBarraCamaraHielo(puestoBarraCamaraHielo);
            break;
        case LLEGADA: llegada();
            break;
        default: break;
        }
   //}

    calcularResultados();
    imprimirResultados();

    printf("OK");

    return 0;
}


void calcularResultados()
{
    PECN = (SPSN - STAN) / NTN;
    PECM = (SPSM - STAM) / NTM;

    PTAN = STAN / NTN;
    PTAM = STAM / NTM;

    GTEV = GPE / MINUTOS_SEMANA;
    GTCV = GPC / MINUTOS_SEMANA;

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++)
    {
        PTON[i] = (STON[i]*100)/T;
    }
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)
    {
        PTOM[i] = (STOM[i]*100)/T;
    }

    return;
}

void imprimirResultados()
{

    printf("PECN:%f\nPECM:%f\nPTAN:%f\nPTAM:%f\nGTEV:%f\nGTCV:%f\n",
           PECN,PECM,PTAN,PTAM,GTEV,GTCV);

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++)
    {
        printf("PTON %d: %f",i,PTON[i]);
    }
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)
    {
        printf("PTOM %d: %f",i,PTOM[i]);
    }
    return;
}

void salidaBarraPrimerPiso(int puestoBarraPrimerPiso)
{

    SPSN += (TPSN[puestoBarraPrimerPiso] - T) * EMPLEADOS_BARRA_PRIMER_PISO;
    SPSM += (TPSN[puestoBarraPrimerPiso] - T) * EMPLEADOS_BARRA_HIELO;

    T = TPSN[puestoBarraPrimerPiso];

    NSN --;

    if(NSN >= EMPLEADOS_BARRA_PRIMER_PISO)
    {
        int TA = tiempoAtencion();
        TPSN[puestoBarraPrimerPiso] = T + TA;

        STAN += TA;

        compra();
    }
    else
    {
        ITON[puestoBarraPrimerPiso] = T;

        TPSN[puestoBarraPrimerPiso] = HIGH_VALUE;
    }

    NTN ++;

    int IPH = intervaloPrimerPisoACamaraHielo();

    llegadaBarraCamaraHielo(IPH);

    return;
}

void salidaBarraCamaraHielo(int puestoBarraCamaraHielo)
{
    SPSN += (TPSM[puestoBarraCamaraHielo] - T) * EMPLEADOS_BARRA_PRIMER_PISO;
    SPSM += (TPSM[puestoBarraCamaraHielo] - T) * EMPLEADOS_BARRA_HIELO;

    T = TPSM[puestoBarraCamaraHielo];

    NSM --;

    if(NSM >= EMPLEADOS_BARRA_HIELO)
    {
        int TA = tiempoAtencion();

        TPSM[puestoBarraCamaraHielo] = T + TA;

        STAM += TA;

        compra();
    }
    else
    {
        ITOM[puestoBarraCamaraHielo] = T;

        TPSM[puestoBarraCamaraHielo] = HIGH_VALUE;
    }

    NTM ++;
    return;
}

void llegada()
{
    SPSN += (TPLL - T) * EMPLEADOS_BARRA_PRIMER_PISO;
    SPSM += (TPLL - T) * EMPLEADOS_BARRA_HIELO;

    T = TPLL;

    int IA = intervaloArribos();

    float R = rand() % 100;

    if(R<=75)llegadaBarraCamaraHielo();
    else llegadaBarraPrimerPiso();

    return;
}

void llegadaBarraCamaraHielo(int tiempoLlegadaDesdePrimerPiso)
{
    NSM ++;

    if(NSM <= EMPLEADOS_BARRA_HIELO)
    {
       int puestoBarraCamaraHielo= buscarPuestoCamaraHielo();

       int TA = tiempoAtencion();

       TPSM[puestoBarraCamaraHielo] = T + tiempoLlegadaDesdePrimerPiso + TA;

       STAM += TA;

       STOM[puestoBarraCamaraHielo] += T + tiempoLlegadaDesdePrimerPiso - ITOM[puestoBarraCamaraHielo];

       compra();
    }
}

void llegadaBarraPrimerPiso()
{
      NSN ++;

    if(NSN <= EMPLEADOS_BARRA_PRIMER_PISO)
    {
       int puestoBarraPrimerPiso= buscarPuestoPrimerPiso();

       int TA = tiempoAtencion();

       TPSN[puestoBarraPrimerPiso] = T + TA;

       STAN += TA;

       STON[puestoBarraPrimerPiso] += T - ITOM[puestoBarraPrimerPiso];

       compra();
    }
}

int determinarProximoEvento(int puestoBarraPrimerPiso,int puestoBarraHielo)
{

    if(TPSN[puestoBarraPrimerPiso] <= TPSM[puestoBarraHielo])
    {
        if(TPSN[puestoBarraPrimerPiso] <= TPLL) return SALIDA_BARRA_PRIMER_PISO;
        else return LLEGADA;
    }
    else
    {
        if(TPLL < TPSM[puestoBarraHielo]) return LLEGADA;
        else return SALIDA_BARRA_HIELO;
    }
}

int buscarMinimoTPSPrimerPiso()
{
    int i =0;
    int j = 0;

   for(int k=0;k<EMPLEADOS_BARRA_PRIMER_PISO;k++)
   {
       if(TPSN[i]<=TPSN[j]) j = i;
   }
   return j;
}

int buscarMinimoTPSCamaraHielo()
{
    int i =0;
    int j = 0;

   for(int k=0;k<EMPLEADOS_BARRA_HIELO;k++)
   {
       if(TPSM[i]<=TPSM[j]) j = i;
   }
   return j;
}

int buscarPuestoCamaraHielo()
{
   int i =0;
   int j = 0;

   for(int k=0;k<EMPLEADOS_BARRA_HIELO;k++)
   {
       if(STOM[i]>STOM[j]) j = i;
   }
   return j;
}

int buscarPuestoPrimerPiso()
{
   int i =0;
   int j = 0;

   for(int k=0;k<EMPLEADOS_BARRA_PRIMER_PISO;k++)
   {
       if(STON[i]>STON[j]) j = i;
   }
   return j;
}

int tiempoAtencion()
{
    return 1;
}

int intervaloArribos()
{
    return 1;
}

int intervaloPrimerPisoACamaraHielo()
{
    return 1;
}

void compra()
{

    float R = rand() % 100;

    if(R>30)
    {
        R = rand() % 100;

        if(R <= 80)
        {
            float OPC = 240 * (PORCENTAJE_DESCUENTO_CLASICO / 100);
            GPC += OPC;
        }
        else
        {
            float OPE = 320 * (PORCENTAJE_DESCUENTO_ESPECIAL /100);
            GPE += OPE;
        }


    }

    return;
}



void iniciarCondicionesIniciales()
{

    T = 0;
    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) TPSN[i]=0;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)TPSM[i]=0;
    TPLL = 0;

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) ITON[i]=0;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)ITOM[i]=0;

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) STON[i]=0;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)STOM[i]=0;

     for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) PTON[i]=0;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)PTOM[i]=0;


    return;
}

