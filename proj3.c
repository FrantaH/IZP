/**
 * Soubor:  proj3.c
 * Datum:   prosinec 2016
 * Autor:   Frantisek Horazny, xhoraz02@stud.fit.vutbr.cz
 * Projekt: shlukova analyza
 * Popis:
 ****/

 /**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO

    struct obj_t *obj = malloc(cap*sizeof(struct obj_t));
  //  if (cap!=0)
       // debug ("malloc obj*\n");
    c->size = 0;
    c->capacity = cap;
    c->obj = obj;

}
/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.   //
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO

  /*  for (;c->size!=0;c->size--)
    {
        c->obj[c->size-1]
    }*/
    free(c->obj);
   ///debug ("uvolneno obj*\n");
    //init_cluster(c,0);
    //free(c);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO

    if (c->size>=c->capacity)
        resize_cluster(c,(c->capacity+CLUSTER_CHUNK));

    c->obj[c->size]=obj;
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO

    for (int i=0;c2->size!=i;i++)
        append_cluster(c1,c2->obj[i]);

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO

    while ((idx+1)!=narr)
    {
        carr[idx]=carr[(idx+1)];
        idx++;
    }


    if ((idx+1)==narr)
        clear_cluster(&carr[idx]);

    return (narr-1);
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO

    return sqrtf(((o1->x-o2->x)*(o1->x-o2->x))+((o1->y-o2->y)*(o1->y-o2->y)));

}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO

    float result=0.0;
    float x=0.0;
    for (int i=0;c1->size>i;i++)
    {
        for (int j=0;c2->size>j;j++)
        {
            x=obj_distance(&(c1->obj[i]),&(c2->obj[j]));
            if (x>result)
                result=x;
        }
    }
    return result;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    float lowest_dis = INFINITY;
    float dis;
    for (int i=0;narr>i+1;i++)                      ///careful
    {
        for (int j=i+1;narr>j;j++)
        {
            dis = cluster_distance(&carr[i],&carr[j]);
            if (lowest_dis>dis)
            {
                lowest_dis = dis;
                *c1=i;
                *c2=j;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO

    FILE *file;

    int objects;
    file = fopen(filename, "r");
    if (file)
    {

        int i=0;
        fscanf(file,"count=%d\n",&objects);
        *arr = malloc(objects * sizeof(struct cluster_t));
       /// debug ("malloc clust*\n");

        struct obj_t o;
        while (i!=objects)
        {
            init_cluster(&(*arr)[i],1);
            fscanf(file,"%d %f %f\n",&o.id,&o.x,&o.y);
            append_cluster(&(*arr)[i],o);
            i++;
        }

        fclose(file);
    }else{
    fprintf(stderr,"chyba v otevreni souboru");
    return -1;
    }
    return objects;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
    int starting_claster = load_clusters(argv[1],&clusters);
    if (starting_claster==(-1))
    {
        fprintf(stderr,"selhalo nacitani souboru");
        return 1;
    }
    int end_cluster_count = starting_claster;
    char *kontrola;
    if (argc==3)
    {
    end_cluster_count = (int)strtol(argv[2],&kontrola,10);
    if (kontrola[0]!='\0'){
        fprintf(stderr,"chyba v nacitani arg");
        return -1;
        }

    }
    int c1;
    int c2;
    while (starting_claster>end_cluster_count)
    {
        find_neighbours(clusters,starting_claster,&c1,&c2);
        merge_clusters(&clusters[c1],&clusters[c2]);
        starting_claster = remove_cluster(clusters,starting_claster,c2);
    }

    print_clusters(clusters,end_cluster_count);
    /*for (int i=0;i<starting_claster;i++)
    remove_cluster(clusters,starting_claster,i);
    */
    while (starting_claster>0)
    {
        starting_claster = remove_cluster(clusters,starting_claster,starting_claster-1);
    }


    free(clusters);
   /// debug ("clear clust*\n");

    return 0;
}
