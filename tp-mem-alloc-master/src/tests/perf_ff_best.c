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
    adresse = adresse + sizeof(mem_busy_block_t);
    if((unsigned long)adresse != ((unsigned long)adresse/64)*64){
        printf("yo\n");
    }
}

int main(int argc, char *argv[]) {
	mem_init();
    mem_set_fit_handler(mem_best_fit);
    int* tabP[NB_ALLOC_MAX];
	
	printf("%lu\n",sizeof(mem_free_block_t));
	
	//On creer un trou de taille 1200 puis des trous de taille 200
	tabP[0] = mem_alloc(1000);
    mem_alloc(100);
	for(int i=0;i<10;i++){
		tabP[i+1] = mem_alloc(200);
		mem_alloc(100);
	}
	for(int i=0;i<11;i++){
		mem_free(tabP[i]);
	}
	
    printf("\n");
    mem_show(&afficher_zone);


	for(int i=0;i<8;i++){
		mem_alloc(101);
	}

    printf("\n");
    mem_show(&afficher_zone);
	
	return 0;
}
