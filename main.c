#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//Variables de Control
#define EMPLEADOS_BARRA_PRIMER_PISO 4
#define EMPLEADOS_BARRA_HIELO 4
#define PORCENTAJE_DESCUENTO_CLASICO 60.00
#define PORCENTAJE_DESCUENTO_ESPECIAL 30.00

#define PRECIO_TRAGO_CLASICO 120.00
#define PRECIO_TRAGO_ESPECIAL 160.00

#define CANTIDAD_TRAGOS_PROMO 2.00

#define TIEMPO_FINAL 20160.00

#define MINUTOS_SEMANA 10080.00

#define SALIDA_BARRA_PRIMER_PISO 1
#define SALIDA_BARRA_HIELO 2
#define LLEGADA 3

#define HIGH_VALUE TIEMPO_FINAL+1

//Tiempos
float TF;
float T;
float TPSN[EMPLEADOS_BARRA_PRIMER_PISO];
float TPSM[EMPLEADOS_BARRA_HIELO];
float TPLL;

//Tiempos Ociosos
float ITON[EMPLEADOS_BARRA_PRIMER_PISO];
float ITOM[EMPLEADOS_BARRA_HIELO];
float STON[EMPLEADOS_BARRA_PRIMER_PISO];
float STOM[EMPLEADOS_BARRA_HIELO];

//Acumuladores
float SPSN = 0;
float SPSM = 0;
float STAN = 0;
float STAM = 0;
int NTN = 0;
int NTM = 0;

//Variables de estado
int NSN = 0;
int NSM = 0;
float GPE = 0;
float GPC = 0;

//Resultados
float PECN = 0;
float PECM = 0;
float PTAN = 0;
float PTAM = 0;
float GTEV = 0;
float GTCV = 0;
float PTON[EMPLEADOS_BARRA_PRIMER_PISO];
float PTOM[EMPLEADOS_BARRA_HIELO];


//Forward declaration
float tiempoAtencion();
float intervaloArribos();
float intervaloPrimerPisoACamaraHielo();

int main()
{

    iniciarCondicionesIniciales();

    while (T <= TF)
    {
        printf("\nTIEMPO:%f\n",T);
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
   }

    calcularResultados();
    imprimirResultados();

    printf("\nOK");

    return 0;
}


void calcularResultados()
{
    printf("\nCalculando resultados\n");

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
    printf("\nImprimiendo resultados\n");

    printf("PECN:%f\nPECM:%f\nPTAN:%f\nPTAM:%f\nGTEV:%f\nGTCV:%f\n",
           PECN,PECM,PTAN,PTAM,GTEV,GTCV);

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++)
    {
        printf("PTON %d: %f\n",i,PTON[i]);
    }
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)
    {
        printf("PTOM %d: %f\n",i,PTOM[i]);
    }
    printf("Cantidad de empleados en la barra del primer piso:%d\nCantidad de empleados en la barra de hielo:%d\n",EMPLEADOS_BARRA_PRIMER_PISO,EMPLEADOS_BARRA_HIELO);
    printf("Porcentaje descuento tragos clasico:%f\n",PORCENTAJE_DESCUENTO_CLASICO);
    printf("Porcentaje descuento tragos especiales:%f\n",PORCENTAJE_DESCUENTO_ESPECIAL);

    printf("Cantidad de clientes atendidos en la barra del primer piso:%d\n",NTN);
    printf("Cantidad de clientes atendidos en la barra de la camara de hielo:%d\n",NTM);
    return;
}

void salidaBarraPrimerPiso(int puestoBarraPrimerPiso)
{
    printf("Salida barra primer piso\n");

    SPSN += (TPSN[puestoBarraPrimerPiso] - T) * NSN;
    SPSM += (TPSN[puestoBarraPrimerPiso] - T) * NSM;

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
    printf("Salida barra camara hielo\n");
    SPSN += (TPSM[puestoBarraCamaraHielo] - T) * NSN;
    SPSM += (TPSM[puestoBarraCamaraHielo] - T) * NSM;

    T = TPSM[puestoBarraCamaraHielo];

    NSM --;

    if(NSM >= EMPLEADOS_BARRA_HIELO)
    {
        float TA = tiempoAtencion();

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
    printf("Llegada\n");
    SPSN += (TPLL - T) * NSN;
    SPSM += (TPLL - T) * NSM;

    T = TPLL;

    float IA = intervaloArribos();

    TPLL = T + IA;

    float R = rand() % 100;

    if(R<=75)llegadaBarraCamaraHielo(0);
    else llegadaBarraPrimerPiso();

    return;
}

void llegadaBarraCamaraHielo(float tiempoLlegadaDesdePrimerPiso)
{
    printf("Llegada camara hielo\n");
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
    printf("Llegada barra primer piso\n");
      NSN ++;

    if(NSN <= EMPLEADOS_BARRA_PRIMER_PISO)
    {
       int puestoBarraPrimerPiso= buscarPuestoPrimerPiso();

       float TA = tiempoAtencion();

       TPSN[puestoBarraPrimerPiso] = T + TA;

       STAN += TA;

       STON[puestoBarraPrimerPiso] += T - ITON[puestoBarraPrimerPiso];

       compra();
    }
}

int determinarProximoEvento(int puestoBarraPrimerPiso,int puestoBarraHielo)
{
    printf("Determinando proximo evento\n");

    if(TPSN[puestoBarraPrimerPiso] <= TPSM[puestoBarraHielo])
    {

        if(TPSN[puestoBarraPrimerPiso] > TPLL) return LLEGADA;
        else return SALIDA_BARRA_PRIMER_PISO;
    }
    else
    {
        if(TPLL < TPSM[puestoBarraHielo]) return LLEGADA;
        else return SALIDA_BARRA_HIELO;
    }
}

int buscarMinimoTPSPrimerPiso()
{
    printf("Buscando minimo TPS primer piso\n");
    int j = 0;

   for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++)
   {
       if(TPSN[i]<=TPSN[j]) j = i;
   }
   printf("Minimo TPS en puesto:%d\n",j);
   return j;
}

int buscarMinimoTPSCamaraHielo()
{
    printf("Buscando minimo TPS camara hielo\n");

    int j = 0;

   for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)
   {
       if(TPSM[i]<=TPSM[j]) j = i;
   }
   printf("Minimo TPS en puesto:%d\n",j);
   return j;
}

int buscarPuestoCamaraHielo()
{
 printf("Buscando puesto camara hielo\n");
   int j = 0;

   for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)
   {
       if(STOM[i]>STOM[j]) j = i;
   }
   printf("Asignado a puesto:%d\n",j);
   return j;
}

int buscarPuestoPrimerPiso()
{
printf("Buscando puesto primer piso\n");
   int j = 0;

   for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++)
   {
       if(STON[i]>STON[j]) j = i;
   }
   printf("Asignado a puesto:%d\n",j);
   return j;
}

//TODOc
float tiempoAtencion()
{
    float R = (rand()%100) / 100.00;
    float TA = 3.0 + 3.377 * pow(R,1/0.8595);
    printf("Tiempo de atencion:%f\n",TA);
    return TA;
}

float intervaloArribos()
{
    float R = (rand()%100) / 100.00;
    float IA = 2.9114 + 6.9886 * pow(R,1/0.74772);
    printf("Intervalo entre Arribos:%f\n",IA);
    return IA;
}

//TODO
float intervaloPrimerPisoACamaraHielo()
{
    float R = (rand()%100) / 100.00;
    float IPH = 20.1 + 19.9 * pow(R,1/0.92297);
    printf("Intervalo entre primer piso y camara de hielo:%f\n",IPH);
    return IPH;
}

void compra()
{
    float R = rand() % 100;

    if(R>30)
    {
        R = rand() % 100;

        if(R <= 80)
        {
            float OPC = (PRECIO_TRAGO_CLASICO * CANTIDAD_TRAGOS_PROMO) * (PORCENTAJE_DESCUENTO_CLASICO / 100.00);
            GPC += OPC;

            printf("Comprando promocion clasica a:%f\n",OPC);
        }
        else
        {
            float OPE =  (PRECIO_TRAGO_ESPECIAL * CANTIDAD_TRAGOS_PROMO) * (PORCENTAJE_DESCUENTO_ESPECIAL /100.00);
            GPE += OPE;

            printf("Comprando promocion especial a:%f\n",OPE);

        }
    }
    else printf("Comprando un solo trago\n");

    return;
}



void iniciarCondicionesIniciales()
{
printf("Inicio de condiciones\n");
    T = 0;
    TF = TIEMPO_FINAL;
    TPLL = 0;
    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) TPSN[i]=HIGH_VALUE;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)TPSM[i]=HIGH_VALUE;

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) ITON[i]=0;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)ITOM[i]=0;

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) STON[i]=HIGH_VALUE;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)STOM[i]=HIGH_VALUE;

    for(int i=0;i<EMPLEADOS_BARRA_PRIMER_PISO;i++) PTON[i]=0;
    for(int i=0;i<EMPLEADOS_BARRA_HIELO;i++)PTOM[i]=0;


    return;
}

