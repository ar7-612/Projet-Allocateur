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
	
	for(int j=0;j<10;j++){
		tabP[0] = mem_alloc(64);
		tabP[1] = mem_alloc(64);
		tabP[2] = mem_alloc(64);
		mem_free(tabP[0]);
		mem_free(tabP[1]);
		tabP[3] = mem_alloc(100);
	}
	
	
	
    printf("\n");
    mem_show(&afficher_zone);
    
    return 0;
}
