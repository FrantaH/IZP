/*
 * Soubor:  proj1.c
 * Datum:   Rijen 2016
 * Autor:   Frantisek Horazny, xhoraz02@stud.fit.vutbr.cz
 * Projekt: Prace s textem
 * Popis:   Program provadi neco - uspesne
 */

 // práce se vstupem/výstupem
#include <stdio.h>

// obecne funkce jazyka C
#include <stdlib.h>

// testovani znaku
#include <ctype.h>

//vyuzivani logickych hodnot true false
#include <stdbool.h>

/**
 * prevadi retezec, skladajici se pouze z cisel na nezaporny int
 * @param prevadeny retezec retezec
 * @return -1 pokud retezec neobsahuje pouze cisla jinak vraci int vysledek
 */
int get_num(char* str)
{
    int result=0;
    for (int i=0;str[i]!='\0';i++)
        if (str[i]<'0'||str[i]>'9')
        return -1;
        else
        {
            result=(result*10)+(str[i]-'0');
        }
return result;
}

/**
 * Zjisti argumenty z argv a zmeni argumenty definovane v fci main
 * @param argc Počet argumentu.
 * @param argv Pole textovych retezcu s argumenty.
 * @return Vraci true pokud nastala chyba, false v pripade ok stavu
 */
bool zjisti_argumenty(int argc,int *arg_s,int *arg_n,bool *arg_x,int *arg_S,bool *arg_r,char* argv[])
{
    for (int i=1;i<argc;i++)
    {
        if (argv[i][0]=='-'&&argv[i][1]=='s' &&argv[i][2]=='\0'&&argc>2)
           {
               *arg_s = get_num(argv[(i+1)]);
                if (*arg_s==-1) return true;
                i++;
           }
        else if (argv[i][0]=='-'&&argv[i][1]=='n' &&argv[i][2]=='\0'&&argc>2)
            {
                *arg_n = get_num(argv[(i+1)]);
                if(*arg_n==-1) return true;
                i++;
            }
        else if (argv[i][0]=='-'&&argv[i][1]=='x' &&argv[i][2]=='\0'&&argc==2)
            {
                *arg_x = true;
            }
        else if (argv[i][0]=='-'&&argv[i][1]=='S' &&argv[i][2]=='\0'&&argc==3)
            {
                *arg_S = get_num(argv[(i+1)]);
                i++;
                if (*arg_S==-1) return true;
            }
        else if (argv[i][0]=='-'&&argv[i][1]=='r' &&argv[i][2]=='\0'&&argc==2)
            {
                *arg_r = true;
            }
        else return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////
/**
 * Hlavni program.
 */
int main(int argc,char* argv[])
{
    int arg_s = 0;
    int arg_n = -2;
    bool arg_x = false;
    int arg_S = 0;
    bool arg_r = false;

    if (zjisti_argumenty(argc,&arg_s,&arg_n,&arg_x,&arg_S,&arg_r,argv))
        printf("chybne zadani argumentu\n");
    else
    {
    if (arg_x)                  //argument x
    {
        int znak=0;
        while ((znak=getchar())!=EOF)
        {
            printf("%02x",znak);
        }
        printf("\n");
    }
    else if (arg_r)             //argument reverse
    {
        char c='0';
        char znak[2];
        int i=0;
        while ((c=getchar())!=EOF)
        {
            if (!isblank(c)&&isprint(c))    //bily?
            {
                znak [i]=c;
                if (i==1)
                {
                    printf("%c",(int)strtol(znak,(char **)NULL,16));   //prevod pole o znacich na cislo vypsane znaky
                    i=-1;
                }
                i++;
            }
        }
        if (c==EOF&&i==1)           //pokud je lichy pocet (i==1) prehodi znaky, doplni 0 a vypise
                {
                    znak[1]=znak[0];
                    znak[0]='0';
                    printf("%c",(int)strtol(&znak[1],(char **)NULL,16));
                }
    }
    else if (arg_S>0)           //argument -S
    {
        bool muzu_radku=false;
        int c;
        char kontrolni_pole[arg_S];
        int i=0;
        while ((c=getchar())!=EOF)
        {
            if (isblank(c)||isprint(c))             //||!isprint(c)
            {
                if ((i+1)<arg_S)
                {
                    kontrolni_pole[i]=c;
                    i++;
                }
                else if ((i+1)==arg_S)
                {
                    for (int x=0;(x+1)<arg_S;x++)
                    printf("%c",kontrolni_pole[x]);
                    printf("%c",c);
                    muzu_radku=true;
                    i++;
                }
                else
                    printf("%c",c);
            }
            else if (muzu_radku)
            {
                printf("\n");
                i=0;
                muzu_radku=false;
            }
            else
            {
                i=0;
            }
        }
    }
    else                        //ostatni argumenty + bez argumentu
    {
    int index_znaku=0;
    char znaky[16];
    int radky=0;
    int c;
    int N=0;
    int skip=arg_s;
    while ((c=getchar())!=EOF)
    {
        if (skip>0)             //argument -s
        {
            int pomocny_index=0;
            for (skip=arg_s;skip>0;skip--)
            {
                c=getchar();
                if (pomocny_index>16)
                {
                    radky++;
                    pomocny_index=0;
                }
                else pomocny_index++;
            }
        }
        if (N==arg_n)           //argument -n
        {
            break;
        }
        if (!isprint(c))
        {
            znaky[index_znaku]='.';
        }
        else
        {
            znaky[index_znaku]=c;
        }
        if (index_znaku==0)
        {
            printf("%08x  ",((16*radky)+arg_s));
            printf("%02x ",c);
            N++;
            index_znaku++;
        }
        else if (index_znaku<8)                      //vypis prvnich 8 znaku
        {
            printf("%02x ",c);
            N++;
            index_znaku++;
        }
        else if (index_znaku==8)                //vypis 9. znaku s mezerou navic
        {
            printf(" %02x ",c);
            N++;
            index_znaku++;
        }
        else if (index_znaku>8&&index_znaku<15)         //vĂ˝pis ĹĄesti znaku z duvodu vyuĹžĂ­vĂĄnĂ­ idex_znaku pozdiji
        {
            printf("%02x ",c);
            N++;
            index_znaku++;
        }
        else if (index_znaku==15)           //vĂ˝pis 16. znaku + vypsĂĄnĂ­ pomocnĂŠho pole na konec oĂĄdku
        {
            printf("%02x ",c);
            N++;
            printf(" |");
            for (int x=0;x<(index_znaku+1);x++)
                printf("%c",znaky[x]);
            printf("|\n");            //novĂ˝ rĂĄdek a napsĂĄnĂ­ pozice dalĹĄĂ­ho znaku (poslednĂ­ho na minulĂŠ oĂĄdce)
            radky++;
            index_znaku=0;
        }
    }

    int pocet_mezer;
    if (index_znaku>0&&index_znaku<8)
    {
        pocet_mezer=((16-index_znaku)*3)+2;
    }
    else if (index_znaku>=8)
    {
        pocet_mezer=((16-index_znaku)*3)+1;
    }
    else if (index_znaku==0)
        return 0;
    else
        printf("chyba v dopocitavani mezer\n");

    for (int i=pocet_mezer;i>0;i--)
        printf(" ");

    printf("|");

    for (int x=0;x<index_znaku;x++)
        printf("%c",znaky[x]);

    for (int x=16;x>index_znaku;x--)
        printf(" ");

        printf("|\n");
    }
    }
    return 0;
}
