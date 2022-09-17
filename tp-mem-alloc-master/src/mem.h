//------------------------------------------------------------------------------
// Projet : TP CSE (malloc)
// Cours  : Conception des systèmes d'exploitation et programmation concurrente
// Cursus : Université Grenoble Alpes - UFRIM²AG - Master 1 - Informatique
// Année  : 2022-2023
//------------------------------------------------------------------------------

#ifndef MEM_H
#define MEM_H

#include <stddef.h>


struct fb_s;
struct ab_s;
typedef struct fb_s {
	size_t size;
	struct fb_s* next;
} fb_t;
typedef struct ab_s {
	size_t size;
	struct ab_s* next;
} ab_t;

/* -----------------------------------------------*/
/* Interface utilisateur de votre allocateur      */
/* -----------------------------------------------*/
void *mem_alloc(size_t);
void mem_free(void *);
size_t mem_get_size(void *);

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free));

/* Enable logging for debugging */
void mem_set_logging(int enabled);

#endif //MEM_H
