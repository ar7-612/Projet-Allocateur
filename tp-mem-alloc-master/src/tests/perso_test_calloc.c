#include "../mem_space.h"
#include "../mem.h"
#include "../mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %7s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           (unsigned long)adresse, (unsigned long)taille);
}

int main(int argc, char *argv[]) {
    mem_init();

    int* tab;
    
    printf("\n");
    mem_show(&afficher_zone);
    
	tab = mem_calloc(100,sizeof(int));
	
	printf("\n");
    mem_show(&afficher_zone);
	
	for(int i=0;i<100;i++){
		if(tab[i]!=0){
			printf("Zone non nul !\n");
			return 1;
		}
	}
    
	mem_free(tab);
	
	printf("\n");
    mem_show(&afficher_zone);
	
    return 0;
}
