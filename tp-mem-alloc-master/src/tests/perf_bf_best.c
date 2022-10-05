#include "../mem_space.h"
#include "../mem.h"
#include "../mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_ALLOC_MAX 256

void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %7s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           (unsigned long)adresse, (unsigned long)taille);
}

int main(int argc, char *argv[]) {
	mem_init();
    mem_set_fit_handler(mem_best_fit);
    int* tabP[NB_ALLOC_MAX];
	
	printf("%lu\n",sizeof(mem_free_block_t));
	
	//On creer des trous de taille 200 puis de taille 150
	tabP[0] = mem_alloc(300);
	for(int i=0;i<10;i++){
		tabP[i] = mem_alloc(200);
		mem_alloc(300);
	}
	for(int i=0;i<10;i++){
		tabP[i+10] = mem_alloc(150);
		mem_alloc(300);
	}
	for(int i=0;i<20;i++){
		mem_free(tabP[i]);
	}
	
	//On Place des bloque de taille 150 puis 200
	for(int i=0;i<10;i++){
		mem_alloc(150);
	}
	for(int i=0;i<10;i++){
		mem_alloc(200);
	}
	
    printf("\n");
    mem_show(&afficher_zone);
	
	return 0;
}
