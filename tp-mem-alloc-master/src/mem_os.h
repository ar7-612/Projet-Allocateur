//------------------------------------------------------------------------------
// Projet : TP CSE (malloc)
// Cours  : Conception des systèmes d'exploitation et programmation concurrente
// Cursus : Université Grenoble Alpes - UFRIM²AG - Master 1 - Informatique
// Année  : 2022-2023
//------------------------------------------------------------------------------

#ifndef MEM_OS_H
#define MEM_OS_H

//include stdlib pour definition du type size_t
#include <stdlib.h>
#include <stdint.h>

//Definie la structure the bloc libre

struct mem_free_block_s;
struct mem_free_block_s {
	uint64_t markerPre;
	size_t size;
	struct mem_free_block_s* next;
	uint64_t markerPost;
};

#define MARKER_PRE ((uint64_t) 0x4242424242424242)
#define MARKER_POST ((uint64_t) 0x2424242424242424)

typedef struct mem_free_block_s mem_free_block_t;
typedef struct mem_free_block_s mem_busy_block_t;

/* -----------------------------------------------*/
/* Interface de gestion de votre allocateur       */
/* -----------------------------------------------*/
// Initialisation
void mem_init(void);

// Définition du type mem_fit_function_t
// type des fonctions d'allocation
typedef mem_free_block_t *(mem_fit_function_t)(mem_free_block_t *, size_t);

// Choix de la fonction d'allocation
// = choix de la stratégie de l'allocation
void mem_set_fit_handler(mem_fit_function_t *);

// Stratégies de base (fonctions) d'allocation
mem_fit_function_t mem_first_fit;
mem_fit_function_t mem_worst_fit;
mem_fit_function_t mem_best_fit;

#endif /* MEM_OS_H */
