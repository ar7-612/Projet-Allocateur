//------------------------------------------------------------------------------
// Projet : TP CSE (malloc)
// Cours  : Conception des systèmes d'exploitation et programmation concurrente
// Cursus : Université Grenoble Alpes - UFRIM²AG - Master 1 - Informatique
// Année  : 2022-2023
//------------------------------------------------------------------------------

#include "mem.h"
#include "mem_space.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>

//-------------------------------------------------------------
// Utilitaires (remplacer par Macros ou inline?)
//-------------------------------------------------------------

mem_fit_function_t *mem_fit;
//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
/**
 * Initialize the memory allocator.
 * If already init it will re-init.
**/
void mem_init() {
	void * debutMem = mem_space_get_addr();
	//On cree le premier bloque de fb. 
	mem_free_block_t bloqueVide = {mem_space_get_size() - sizeof(mem_free_block_t) - sizeof(mem_free_block_t) - sizeof(mem_busy_block_t),NULL};
    //printf("%lu \n",mem_space_get_size());
	//On le stoque en debut de memoire en laissant la place pour les bloques fictif
    *((mem_free_block_t*) (debutMem+sizeof(mem_free_block_t)+sizeof(mem_busy_block_t)))=bloqueVide;
    //On cree les bloques fictif.
    mem_free_block_t ficitfVide = {0,(mem_free_block_t*) (debutMem+sizeof(mem_free_block_t)+sizeof(mem_busy_block_t))};
    mem_busy_block_t fictifPlein = {0,NULL};

	//On place les bloques fictif en memoire
	*((mem_free_block_t*) debutMem) = ficitfVide;
    *((mem_free_block_t*)(debutMem + sizeof(mem_free_block_t))) = fictifPlein;

	//Et finalement on initialise la fonction de recherche
	mem_set_fit_handler(mem_first_fit);
}



//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
/**
 * Allocate a bloc of the given size.
**/

void * mem_alloc(size_t size) {
    void * debutMem = mem_space_get_addr();
	mem_free_block_t* bloqueVidePrec = mem_fit(((mem_free_block_t*)debutMem),size);//On resois le precedant de celui qu'on allou
    if(bloqueVidePrec==NULL){
        printf("Pas la place!\n");
        return NULL;
    }
    //On cherche les bloques plein avant et apres le bloque libre
    mem_busy_block_t* bloquePleinPrec = (mem_busy_block_t*)((void*)bloqueVidePrec + bloqueVidePrec->size + sizeof(mem_free_block_t));
    mem_busy_block_t* bloquePleinSuiv = bloquePleinPrec->next;
    while(bloquePleinSuiv!=NULL && bloquePleinSuiv < bloqueVidePrec->next){
        bloquePleinPrec=bloquePleinSuiv;
        bloquePleinSuiv=bloquePleinSuiv->next;
    }
    //On donne l'adresse du prochain bloque memoire au bloque precedant
    bloquePleinPrec->next = bloqueVidePrec->next;
    
    //On creer le nouveau bloque
    mem_busy_block_t* newPlein;
    mem_free_block_t* nextVide;

    newPlein = bloqueVidePrec->next;

    size_t prevVideSize = bloqueVidePrec->next->size;


    if(prevVideSize - size <= sizeof(mem_free_block_t)){
        //on regarde si on a la place de rajouter un bloque free aprés
        nextVide = bloqueVidePrec->next->next;
        newPlein->size = prevVideSize;
    } else {
        nextVide = (mem_free_block_t*)((void*)newPlein + size + sizeof(*newPlein));
        nextVide->size = prevVideSize - size - sizeof(*newPlein) - sizeof(mem_free_block_t);
        nextVide->next = bloqueVidePrec->next->next;
        newPlein->size = size;
    }
    newPlein->next = bloquePleinSuiv;
    bloqueVidePrec->next = nextVide;

    return ((void*)newPlein + sizeof(*newPlein));
}

//-------------------------------------------------------------
// mem_get_size
//-------------------------------------------------------------
size_t mem_get_size(void * zone)
{
    //TODO: implement
	assert(! "NOT IMPLEMENTED !");
    return 0;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
/**
 * Free an allocaetd bloc.
**/
void mem_free(void *zone) {
    //TODO: implement
	assert(! "NOT IMPLEMENTED !");
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void * zone, size_t size, int free)) {
    void * debutMem = mem_space_get_addr();

    // On recupere un pointeur sur les deux files
    mem_free_block_t* free = ((mem_free_block_t*)debutMem)->next;
    mem_busy_block_t* busy = ((mem_busy_block_t*)(debutMem + sizeof(mem_free_block_t)))->next;

    // Tant qu'il reste des blocs à traiter
    while (free != NULL || busy != NULL) {

        // Si il n'y a plus de bloc occupé ou que le bloc libre actuel est avant
        if (busy == NULL || (free != NULL && free < busy)) {

            // On affiche le bloc libre actuel et on passe au bloc libre suivant
            print(free, free->size, 1);
            free = free->next;
        }
        else
        {
            // Sinon on affiche le bloc occupé actuel et on passe au suivant
            print(busy, busy->size, 0);
            busy = busy->next;
        }
    }
}



//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_set_fit_handler(mem_fit_function_t *mff) {
	mem_fit = mff;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
mem_free_block_t *mem_first_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    mem_free_block_t* free = first_free_block;
    mem_free_block_t* suiv = free->next;
    while(suiv!=NULL){
        if(suiv->size>=wanted_size){
            printf("fin memfit \n");
            return(free);
        }
        free=suiv;
        suiv=suiv->next;
     
    }
    printf("fin memfit err\n");
    return NULL;
}
//-------------------------------------------------------------
mem_free_block_t *mem_best_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    //TODO: implement
	assert(! "NOT IMPLEMENTED !");
	return NULL;
}

//-------------------------------------------------------------
mem_free_block_t *mem_worst_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    //TODO: implement
	assert(! "NOT IMPLEMENTED !");
	return NULL;
}
