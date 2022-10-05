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
	
	//On creer des bloque de taille 200
	for(int i=0;i<10;i++){
		tabP[i] = mem_alloc(200);
	}

	//On libere ces bloque en partant de la fin puis on alloue des bloques de taille 150
	for(int i=9;i>=0;i--){
		mem_free(tabP[i]);
		tabP[i] = mem_alloc(150);
	}

    printf("\n");
    mem_show(&afficher_zone);
	
	return 0;
}
