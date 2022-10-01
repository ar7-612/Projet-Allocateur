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

mem_fit_function_t *mem_fit;

/**
 * Exit if one of the marker around p is invalid. Doesn't detect all overflow
**/
void checkOverflow (void * p){
	if(p==NULL){
		return;
	}
	void * bloqueSup = p + ((mem_busy_block_t*)p)->size + sizeof(mem_busy_block_t);
	//Si pas le dernier bloque
	if(bloqueSup < mem_space_get_addr() + mem_space_get_size()){
		if(((mem_busy_block_t*)bloqueSup)->markerPre != MARKER_PRE){
			fprintf(stderr,"Overflow detected\n");
			exit(-3);
		}
	}
	if(((mem_busy_block_t*)p)->markerPost != MARKER_POST){
		fprintf(stderr,"Overflow detected\n");
		exit(-3);
	}
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
	//On cree le premier bloque de fb. 
	mem_free_block_t bloqueVide = {MARKER_PRE,mem_space_get_size() - sizeof(mem_free_block_t) - sizeof(mem_free_block_t) - sizeof(mem_busy_block_t),NULL,MARKER_POST};
    //printf("%lu \n",mem_space_get_size());
	//On le stoque en debut de memoire en laissant la place pour les bloques fictif
    *((mem_free_block_t*) (debutMem+sizeof(mem_free_block_t)+sizeof(mem_busy_block_t)))=bloqueVide;
    //On cree les bloques fictif.
    mem_free_block_t ficitfVide = {MARKER_PRE,0,(mem_free_block_t*) (debutMem+sizeof(mem_free_block_t)+sizeof(mem_busy_block_t)),MARKER_POST};
    mem_busy_block_t fictifPlein = {MARKER_PRE,0,NULL,MARKER_POST};

	//On place les bloques fictif en memoire
	*((mem_free_block_t*) debutMem) = ficitfVide;
    *((mem_free_block_t*)(debutMem + sizeof(mem_free_block_t))) = fictifPlein;

	//Et finalement on initialise la fonction de recherche
	mem_set_fit_handler(mem_first_fit);
}

/**
 * alligne sur 32 bits
**/
size_t aligned32 (size_t size){
	if((size & 0x11) != 0){
		size_t tmp = 4;
		size = (size & ~0x11) + tmp;
	}
	return size;
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
        nextVide->size = prevVideSize - size - sizeof(*newPlein);
        nextVide->next = bloqueVidePrec->next->next;
		nextVide->markerPre = MARKER_PRE;
		nextVide->markerPost = MARKER_POST;
		
        newPlein->size = size;
    }
    newPlein->next = bloquePleinSuiv;
    bloqueVidePrec->next = nextVide;
	
	checkOverflow(newPlein);
    return ((void*)newPlein + sizeof(*newPlein));
}

//-------------------------------------------------------------
// mem_calloc
//-------------------------------------------------------------
/**
 * allocate a bloc of the count*size Bytes? and initialise all this memorie to zero
**/
void *mem_calloc(size_t count, size_t size){
	//Version simple
	void* p = mem_alloc(count*size);
	if(p==NULL){
		return NULL;
	}
	for(size_t i=0;i<count*size;i++){
		((char*)p)[i]=0;
	}
	return p;
}

//-------------------------------------------------------------
// mem_realloc
//-------------------------------------------------------------
/**
 * Reallocate a bloc of the given size.
**/
void *mem_realloc(void *ptr, size_t size){
	//Version simple
	void* newP=NULL;
	if(size==0){
		mem_free(ptr);
		return NULL;
	}
	newP = mem_alloc(size);//Toujours une nouvelle allocation
	if(newP==NULL){
		return NULL;
	}
	for(int i=0;i<size;i++){//Parcours de cases en trop
		((char*)newP)[i]=((char*)ptr)[i];
	}
	mem_free(ptr);
	return newP;

}

//-------------------------------------------------------------
// mem_get_size
//-------------------------------------------------------------
size_t mem_get_size(void * zone) {
    return ((mem_busy_block_t*)zone)->size;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
/**
 * Free an allocaetd bloc.
**/
void mem_free(void *zone) {

    zone = zone - sizeof(mem_busy_block_t);
    void * debutMem = mem_space_get_addr();

    //Verfivie que la zone est en memoire
    if((zone < debutMem + sizeof(mem_busy_block_t) + sizeof(mem_free_block_t))
     ||(zone > debutMem + mem_space_get_size())){
		return;
        //fprintf(stderr,"Ahem. Non. (segmentation fault)\n");
        //exit(-1);
    }

    //Recherche des bloques nessessaire
    mem_free_block_t* bloqueFreePrec = (mem_free_block_t*)(debutMem);
    mem_free_block_t* bloqueFreeSuiv = bloqueFreePrec->next;
    mem_busy_block_t* bloquePleinPrec = (mem_busy_block_t*)(debutMem + sizeof(mem_free_block_t));
    mem_busy_block_t* bloquePleinSuiv = bloquePleinPrec->next;

    while(bloqueFreeSuiv!=NULL && (void*)bloqueFreeSuiv < zone){
        bloqueFreePrec=bloqueFreeSuiv;
        bloqueFreeSuiv=bloqueFreeSuiv->next;
    }
    while(bloquePleinSuiv!=NULL && (void*)bloquePleinSuiv != zone){
        bloquePleinPrec=bloquePleinSuiv;
        bloquePleinSuiv=bloquePleinSuiv->next;
    }
	if(bloquePleinSuiv!=zone){
		fprintf(stderr,"Euuuu, j'ai bien peur que cette zone soit deja libre... (double free or coruption)\n");
        exit(-2);
	}
    bloquePleinSuiv=bloquePleinSuiv->next;
	
	//On verifie l'integrite des 3 bloques memoires
	checkOverflow(zone);
	checkOverflow(bloquePleinPrec);
	checkOverflow(bloquePleinSuiv);

    //Si pas de fusion
    bloquePleinPrec->next = bloquePleinSuiv;
    bloqueFreePrec->next = zone;
    ((mem_free_block_t*)zone)->next = bloqueFreeSuiv;

    //Si fusion apres
    if(zone + sizeof(mem_free_block_t) + ((mem_free_block_t*)zone)->size == bloqueFreeSuiv){
        ((mem_free_block_t*)zone)->size += sizeof(mem_free_block_t) + bloqueFreeSuiv->size;
        ((mem_free_block_t*)zone)->next = bloqueFreeSuiv->next;
    }
    //Si fusion avant
    if((void*)bloqueFreePrec + sizeof(mem_free_block_t) + bloqueFreePrec->size == zone){
        bloqueFreePrec->size += sizeof(mem_free_block_t) + ((mem_free_block_t*)zone)->size;
        bloqueFreePrec->next = ((mem_free_block_t*)zone)->next;
    }

    
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
            print(free, free->size + sizeof(mem_busy_block_t), 1);
            free = free->next;
        }
        else
        {
            // Sinon on affiche le bloc occupé actuel et on passe au suivant
            print(busy, busy->size + sizeof(mem_free_block_t), 0);
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


mem_free_block_t *general_fit(mem_free_block_t *first_free_block, size_t wanted_size, int strategie(int,int)){
	mem_free_block_t* free = first_free_block;
    mem_free_block_t* suiv = free->next;
	mem_free_block_t* butAdr = NULL;
	size_t but;
    while(suiv!=NULL){
        if(suiv->size>=wanted_size){
            if(butAdr==NULL || strategie(suiv->size,but)){
				butAdr=free;
				but=suiv->size;
			}
        }
        free=suiv;
        suiv=suiv->next;
     
    }
    return butAdr;
}

mem_free_block_t *mem_first_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    mem_free_block_t* free = first_free_block;
    mem_free_block_t* suiv = free->next;
    while(suiv!=NULL){
        if(suiv->size>=wanted_size){
            return(free);
        }
        free=suiv;
        suiv=suiv->next;
     
    }
    return NULL;
}

//-------------------------------------------------------------
mem_free_block_t *mem_best_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    int f(int a,int b){return a<b;}
	return general_fit(first_free_block,wanted_size,f);
}

//-------------------------------------------------------------
mem_free_block_t *mem_worst_fit(mem_free_block_t *first_free_block, size_t wanted_size) {
    int f(int a,int b){return a>b;}
	return general_fit(first_free_block,wanted_size,f);
}