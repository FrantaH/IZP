/*
 * Soubor:  proj1.c
 * Datum:   listopad 2016
 * Autor:   Frantisek Horazny, xhoraz02@stud.fit.vutbr.cz
 * Projekt: iteracni vypocty
 * Popis:   program pocita logaritmy nebo exponencionalni fci (podle arg)
 */

 // práce se vstupem/výstupem
#include <stdio.h>

// obecné funkce jazyka C
#include <stdlib.h>

// testování znaků
#include <ctype.h>

//vyuzivani logickych hodnot true false
#include <stdbool.h>

//matematicka knihovna obsahuje log a pow napr
#include <math.h>

//prace s retezci (argumenty)
#include <string.h>

double taylor_log(double x, long int n);
double cfrac_log(double x, long int n);
double taylor_pow(double x, double y, long int n);
double taylorcf_pow(double x, double y, long int n);

/**
 * vypocet exponencionalni fce pomoci taylor_log()
 * @param a je zaklad exponencionalni fce
 * @param x je mocnina
 * @param n je pocet iteraci
 * @return vraci double celkovy vysledek
 */
double taylor_pow(double a, double x, long int n)
{
    bool uprava_x=false;
    if (x<0)
    {
        uprava_x=true;
        x=x*(-1);
    }
    if (a<=0) return NAN;;
    double mocnina=1.0;
    int faktorial = 1;
    double log = taylor_log(a,n);
    double soucet = 1.0;
    for (long i=1;i<(n+1);i++)
    {
        mocnina = mocnina * x * log;
        faktorial = faktorial * i;
        if (faktorial==0)       //podminka pokud faktorial pretece (tato hodnota pretece prvni)
        {
            fprintf(stderr,"prilis mnoho iteraci, vysledek ktery vidite je pro %d iteraci\n",i);
            if (uprava_x)
            {
                return 1/soucet;
            }
            else
            return soucet;
        }
        soucet = soucet + mocnina / faktorial;

    }
    if (uprava_x)
        {
            return 1/soucet;
        }
    else
    return soucet;
}

/**
 * vypocet exponencionalni fce pomoci cfrac_log()
 * @param a je zaklad exponencionalni fce
 * @param x je mocnina
 * @param n je pocet iteraci
 * @return vraci double celkovy vysledek
 */
double taylorcf_pow(double a, double x, long int n)
{

    bool uprava_x=false;
    if (x<0)
    {
        uprava_x=true;
        x=x*(-1);
    }
    double mocnina=1.0;
    int faktorial = 1;
    double soucet = 1.0;
    double log = cfrac_log(a,n);
    for (long i=1;i<(n+1);i++)
    {
        mocnina = mocnina * x * log;
        faktorial = faktorial * i;
        if (faktorial==0)
        {
            fprintf(stderr,"prilis mnoho iteraci, vysledek ktery vidite je pro %d iteraci\n",i);
            if (uprava_x)
            {
                return 1/soucet;
            }
            else
            return soucet;
        }
        soucet = soucet + mocnina / faktorial;
    }
    if (a<=0) soucet=NAN;
    if (uprava_x)
    {
        return 1/soucet;
    }
    return soucet;
}

/**
 * vypocet logaritmu pomoci taylorova polynomu, pro 0-1 pomoci odcitani,
 * 1 a vic pomoci scitani
 * @param x je cislo ktere logaritmujeme, n je pocet iteraci
 * @return vraci double vysledek logaritmu
 */
double taylor_log(double x, long int n)
{
	double rozdil =0.0;
	double mocnina=1.0;
	double jmenovatel=1.0;
	double posledniClen;
    if (x>0.0&&x<1.0)           //logarimtus pro hodnotu 0-1
    {
        double y=1.0-x;
        for (long i=1;i<(n+1);i++)
        {
            mocnina = mocnina * y;
            posledniClen = mocnina / i;
            rozdil = rozdil - posledniClen;
            //printf("rozdil=%g,iterace=%i \n",rozdil,i);
        }
    }
    else if (x>=1.0)             ////logarimtus pro hodnotu 1 a vice
    {
        for (long i=1;i<(n+1);i++)
        {
            jmenovatel=(x-1)/x;
            mocnina = mocnina * jmenovatel;
            posledniClen = mocnina/i;
            rozdil=rozdil+posledniClen;             //rozdil je v tomto pripade soucet ale to nevadi
            //printf("rozdil=%g,iterace=%i \n",rozdil,i);
        }
    }
    else if (x==0.0)
    {
        rozdil = -INFINITY;
    }
    else if (x<0.0)
    {
        rozdil = NAN;
    } else printf("taylorcf_pow(%g,%.12g) = %.12g\n",rozdil,x,NAN);

           //if (x<=0) fprintf(stderr,"neni mozne logaritmovat zaporne cislo");
    return rozdil;
}

/**
 * pocita logaritmus pomoci zretezenych zlomku
 * @param x je cislo ktere logaritmujeme, n je pocet iteraci
 * @return vraci double hodnotu vysledku logaritmu
 */
double cfrac_log(double x, long int n)
{
    if (x>0.0)
    {
        double z = (x-1)/(x+1);
        double predchoziClen = 1.0;

        for (;n>0;n--)
            {
                predchoziClen = ((2*n-1)-(n*n*z*z)/predchoziClen);
                    //printf("predchoziClen=%g,iterace=%i \n",predchoziClen,n);
            }
        return ((2*z)/predchoziClen);
    }
    else if (x==0.0)
    {
        return -INFINITY;
    }
    else
        return NAN;
}

/**
 * zkontroluje jestli je pole err prazdne pokud neni vypise chybu s chybnymi znakz
 * @param pole znaku
 * @return void
 */
void kontrolaStrt(char *err,bool *kontrola)
{
    if (err[0]!='\0')
    {
        fprintf(stderr,"chybne znaky v argumentech, kde ma byt jen cislo je i neco jineho:%s\n",err);
        *kontrola=true;
    }
    //printf("error %s \n",err[0]);
}

/**
 * Zjisti argumenty z argv a zmeni promenne argumentu definovane v fci main
 * @param argc Počet argumentů.
 * @param argv Pole textových řetězců s argumenty.
 * @return Vraci true pokud nastala chyba, false v pripade ok stavu
 */
bool zjisti_argumenty(int argc,bool *arg_log,double *zaklad_pow,long int *iterace,double *used_num,char* argv[])
{
    char *err;
    bool kontrola=false;
    for (int i=1;i<argc;i++)
    {
        if (argc==4&&(strcmp(argv[1],"--log")==0))  //log
            {
            *arg_log = true;
            *used_num=strtod(argv[2],&err);
            kontrolaStrt(err,&kontrola);
            *iterace=strtol(argv[3],&err,10);
            kontrolaStrt(err,&kontrola);
            if (kontrola)
            {
                return true;
            }else {return false;}
            }

        else if (argc==5&&(strcmp(argv[1],"--pow")==0))     //pow
            {
            *zaklad_pow=strtod(argv[2],&err);
            kontrolaStrt(err,&kontrola);
            *used_num=strtod(argv[3],&err);
            kontrolaStrt(err,&kontrola);
            *iterace=strtol(argv[4],&err,10);
            kontrolaStrt(err,&kontrola);
            if (kontrola)
            {
                return true;
            }
            else {return false;}
            }
        else return true;
    }
    return true;
}

/////////////////////////////////////////////////////////////////
/**
 * Hlavní program.
 */
int main(int argc,char* argv[])
{
    bool arg_log = false;
    double zaklad_pow = NAN;
    long int iterace;
    double used_num;


    if (zjisti_argumenty(argc,&arg_log,&zaklad_pow,&iterace,&used_num,argv)||iterace<1)
    {
        fprintf(stderr,"nastala chyba pri vypoctu argumentu\n");
        return 1;
    }

    if (arg_log)
    {
       /* if (used_num<=0.0)
        {
            fprintf(stderr,"neni mozne logaritmovat zaporne cislo\n");
            return 0;
        }*/
        printf("       log(%g) = %.12g\n",used_num,log(used_num));
        printf(" cfrac_log(%g) = %.12g\n",used_num,cfrac_log(used_num,iterace));
        printf("taylor_log(%g) = %.12g\n",used_num,taylor_log(used_num,iterace));
      //  printf("z cisla %.12g\n",used_num);
      //  printf("pocet iteraci %i\n",iterace);
    }

    else if (zaklad_pow!=NAN)
    {

        printf("         pow(%g,%.12g) = %.12g\n",zaklad_pow,used_num,pow(zaklad_pow,used_num));
        printf("  taylor_pow(%g,%.12g) = %.12g\n",zaklad_pow,used_num,taylor_pow(zaklad_pow,used_num,iterace));
        printf("taylorcf_pow(%g,%.12g) = %.12g\n",zaklad_pow,used_num,taylorcf_pow(zaklad_pow,used_num,iterace));

        //printf("%f,%f,%i",zaklad_pow,used_num,iterace);

    }

    else
         fprintf(stderr,"chyba, nebyl nalazen zadny argument");

    return 0;
}
