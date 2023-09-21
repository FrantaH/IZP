/** @file proj3.h
 *  @date 15.12.2016
 *  @author Frantisek Horazny
 *  Hlavickovy soubor s dokumentaci a prototypy fci.
 */

#ifndef _PROJ3_H
#define _PROJ3_H

/** @brief Definice objektu
 */
struct obj_t {
    int id;     /**< Identifikator objektu*/
    float x;    /**< Souradnice X*/
    float y;    /**< Souradnice Y */
};

/** @brief Definice shluku
 */
struct cluster_t {
    int size;           /**< velikost shluku - pocet objektu v shluku */
    int capacity;       /**< kapacita - maximalni pocet objektu v shluku */
    struct obj_t *obj;  /**< pole objektu - ukazatel na prvni polozku v poli */
};

/** @defgroup praceSShluky
 * tato skupina fci pracuje s jednotlivimy clustery a objekty a obsahuje fce:
 * zajistujici alokace, realokace, cisteni, pridavani objektu a spojovani clusteru
 */

 /** @defgroup praceSPolemShluku
 * tato skupina fci pracuje s polem shluku - odstraneni shluku z pole, vypocet vzdalenosti shluku
 * vyhledani nejblizsich sousedu, serazeni shluku a vypis.
 */

 /** @defgroup main
 * V teto skupine jsou dve hlavni fce a to load_clusters() a main()
 */


/** @addtogroup praceSShluky */
/*\{*/
/**
 *Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 *Ukazatel NULL u pole objektu znamena kapacitu 0.
 *@pre c neni NULL
 *@post bylo alokovane pole pro cap objektu v shluku *c
 *@param *cukazatel na cluster
 *@param cap kapacita kterou chceme alokovat
*/
void init_cluster(struct cluster_t *c, int cap);

/**
 *Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *@pre c neni NULL
 *@post *c ma kapacitu 0
 *@param *c ukazatel na cluster ktery chceme vycistit(uvolnit)
 */
void clear_cluster(struct cluster_t *c);

/// Chunk of cluster objects. Value recommended for reallocation.
extern const int CLUSTER_CHUNK;

/**
 *Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *@pre c neni NULL
 *@pre new_cap je vetsi nez kapacita shluku *c
 *@post shluk *c ma novou kapacitu new_cap
 *@param *c ukazatel na cluster urceny ke zmene
 *@param new_cap nova kapacita
 *@return vrací ukazatel na novy shluk s novou velikosti
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 *Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 *nevejde.
 *@pre c neni NULL
 *@post shluk *c byl zvetsen na kapacitu puvodni+10 pokud se objekt nevesel
 *@post objekt byl pridan do shluku *c
 *@param *c ukazatel na cluster do ktereho vlkadame
 *@param obj objekt ktery ukladame
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 *Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 *Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 *Shluk 'c2' bude nezmenen.
 *@pre c1 neni NULL
 *@pre c2 neni NULL
 *@post *c1 je serazena podle id a obsahuje objekty *c1 i *c2
 *@param *c1 ukazatel na cluster do ktereho pripisujeme
 *@param *c2 ukazatel na cluster ktery kopirujeme
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/*\}*/
/** @addtogroup praceSPolemShluku */
/*\{*/

/**
 *Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 *(shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 *pocet shluku v poli.
 *@pre carr neni NULL
 *@pre narr > 1
 *@pre idx < narr
 *@post *carr obsahuje o polozku mene (vsechny polozky byly posunuty)
 *@param *carr pole clusteru ktere menime
 *@param narr pocet polozek pole *carr
 *@param idx index na kterem se nachazi cluster ktery mazeme
 *@return vraci novy pocet polozek v poli *carr (narr-1)
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 *Pocita Euklidovskou vzdalenost mezi dvema objekty.
 *@pre *o1 neni NULL
 *@pre *o2 neni NULL
 *@param *o1 ukazatel na prvni objekt
 *@param *o2 ukazatel na druhy objekt
 *@return vzdalenost mezi objekty
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 *Pocita vzdalenost dvou shluku.
 *@pre c1 neni NULL
 *@pre c2 neni NULL
 *@param *c1 ukazatel na prvni shluk
 *@param *c2 ukazatel na druhy shluk
 *@return vzdalenost mezi dvěma nejvzdálenějšíma objektama v shlucích c1 a c2
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 *Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 *hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 *'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 *adresu 'c1' resp. 'c2'.
 *@pre carr neni NULL
 *@pre narr > 1
 *@post *c1 nastaveno na index 1. souseda
 *@post *c2 nastaveno na index 2. souseda
 *@param *carr pole shluku
 *@param narr pocet shluku v poli *carr
 *@param *c1 ukazatel na cislo int
 *@param *c2 ukazatel na cislo int
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 *Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 *@pre c neni NULL
 *@post cluster *c byl serazen podle id
 *@param *c ukazatel na shluk urcen pro serazeni
*/
void sort_cluster(struct cluster_t *c);

/**
 *Tisk shluku 'c' na stdout.
 *@pre c neni NULL
 *@post cluster *c byl vytisknut na stdout
 *@param *c ukazatel na shluk urcen pro tisknuti
*/
void print_cluster(struct cluster_t *c);

/**
 *Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 *Tiskne se prvnich 'narr' shluku.
 *@pre carr neni NULL
 *@pre narr > 0
 *@post byly vytisknuty clustery v *carr na stdout
 *@param *carr pole shluku
 *@param narr pocet shluku v poli ktere chceme vytisknout
*/
void print_clusters(struct cluster_t *carr, int narr);

/*\}*/
/** @addtogroup main */
/*\{*/

/**
 *Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 *jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 *polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 *kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 *V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
 *@pre filename neni NULL nebo prazdne
 *@post v poli *arr je tolik shluku kolik je objektu, kazdy obsahuje objekt
 *@param *filename - pole, obsahujici jmeno souboru k otevreni
 *@param **arr - ukazatel na pole, obsahujici jednotlive clustery
 *@return int pocet nactenych objektu(shluku)
*/
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * z argv nacte jmeno souboru pro otevreni a pokud existuje dalsi argument
 * tak nacte pocet konecnych shluku ktere byly zadany. spousti jednotlive funkce tak
 * aby vznikl pozadovany pocet shluku a nasledne je vypise a uvolni alokovane misto
 *@param argc - pocet argumentu pri spousteni programu
 *@param *argv[] pole argumentu programu
 *@return int 0 pokud program skoncil uspesne, neco jineho kdyz neuspesne
*/
int main(int argc, char *argv[]);

/*\}*/

#endif
