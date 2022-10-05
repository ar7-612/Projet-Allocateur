#include "../mem_space.h"
#include "../mem.h"
#include "../mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %7s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           (unsigned long)adresse, (unsigned long)taille);
	adresse += sizeof(mem_free_block_t);
	if((size_t) adresse != (size_t) adresse / 8 * 8){
		printf("Oups...\n");
		exit(-10);
	}
}

int main(int argc, char *argv[]) {
    mem_init();

    int nbTest = 16;
    int* tabP[16];
    int tabFree[16]={1,3,2,5,4,6,9,8,10,15,13,12,11,7,14,0};
    
    mem_show(&afficher_zone);
	printf("\n");
    for(int i=0;i<nbTest;i++){
        tabP[i]=mem_alloc(sizeof(int));
		*tabP[i]=i;
    }
    mem_show(&afficher_zone);
    for(int i=0;i<nbTest;i++){
        mem_free(tabP[tabFree[i]]);
		mem_show(&afficher_zone);
		printf("\n");

    }
    
    return 0;
}
