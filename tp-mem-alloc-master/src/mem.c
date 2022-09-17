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

//-------------------------------------------------------------
// Utilitaires (remplacer par Macros ou inline?)
//-------------------------------------------------------------

/**
 * Return a pointer to the first free block
 * or NULL if there is none
**/
fb* get_fb(){
	return (fb_t*) *mem_space_get_addr();
}

/**
 * Return a pointer to the first alocated block
 * or NULL if there is none
**/
ab* get_ab(){
	void * debutMem = mem_space_get_addr();
	if(*debutMem==NULL || *debutMem!=(debutMem+sizeof(fb_t*)){
		return (ab_t*) (debutMem+sizeof(fb_t*));
	}
	if((*debutMem)->size==mem_space_get_size() - sizeof(fb_t) - sizeof(fb_t*)){
		return NULL;
	}
	return (ab_t*) debutMem + sizeof(fb_t) + sizeof(fb_t*) + (*debutMem)->size;
}

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
/**
 * Initialize the memory allocator.
 * If already init it will re-init.
**/
void mem_init() {
	void * debutMem = mem_space_get_addr();
	//On cree le profier bloque de fb. 
	fb_t bloqueVide = {mem_space_get_size() - sizeof(fb_t) - sizeof(fb_t*),NULL};
	//On le stoque en debut de memoire en laissant la place pour un pointeur
	*(debutMem+sizeof(fb_t*))=bloqueVide;
	//On memorise son adresse dans l'espace qu'on a laisse au debut de la memoire
	*debutMem = debutMem + (fb_t*);
	//Et finalement on initialise la fonction de recherche
	mem_set_fit_handler(mem_first_fit);
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
/**
 * Allocate a bloc of the given size.
**/
void *mem_alloc(size_t size) {
	//TODO: implement
	assert(! "NOT IMPLEMENTED !");
    return NULL;
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
void mem_show(void (*print)(void *, size_t, int free)) {
    //TODO: implement
	assert(! "NOT IMPLEMENTED !");
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_set_fit_handler(mem_fit_function_t *mff) {
	//TODO: implement
	assert(! "NOT IMPLEMENTED !");
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
mem_free_block_t *mem_first_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    //TODO: implement
	assert(! "NOT IMPLEMENTED !");
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
