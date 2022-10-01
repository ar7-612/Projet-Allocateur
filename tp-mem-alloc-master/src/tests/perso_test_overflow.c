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

void afficher_zone_libre(void *adresse, size_t taille, int free) {
    if (free)
        afficher_zone(adresse, taille, 1);
}

void afficher_zone_occupee(void *adresse, size_t taille, int free) {
    if (!free)
        afficher_zone(adresse, taille, 0);
}

void printRet(void * p){
    if(p==NULL){
        printf("Pas de place!\n");
    }
    mem_show(&afficher_zone);
}

void *alloc_max(size_t estimate) {
    void *result;
    static size_t last = 0;

    //mem_show(afficher_zone);
    //printf("==================\n");

    while ((result = mem_alloc(estimate)) == NULL) {
        estimate--;
    }
    debug("Alloced %zu bytes at %p\n", estimate, relative_adr(result));
    if (last) {
        // Idempotence test
        if (estimate != last){
            //mem_show(afficher_zone);
        }
        assert(estimate == last);
    } else {
        last = estimate;
    }
    return result;
}

void printMem(void* tabP[16]){
	/*printf("%lu : %u \n%lu : %u \n%lu : %u \n\n",
	(unsigned long)(tabP[0]), *((unsigned int*)(tabP[0])),
	(unsigned long)(tabP[1]), *((unsigned int*)(tabP[1])),
	(unsigned long)(tabP[2]), *((unsigned int*)(tabP[2])));*/
	for(unsigned long i = (unsigned long)(tabP[0]); i < (unsigned long)(tabP[2]) + sizeof(int); i++){
		printf("%3d ",*((char*)i));
	}
	printf("\n");
}

void avant(void* tabP[16]){
	mem_init();
	tabP[0] = mem_alloc(sizeof(char));
	tabP[1] = mem_alloc(sizeof(char));
	tabP[2] = mem_alloc(sizeof(char));
	
	*((char*)(tabP[0])) = 1;
	*((char*)(tabP[1])) = 2;
	*((char*)(tabP[2])) = 3;
	printMem(tabP);
	*((unsigned int*)(tabP[1]-1)) = (unsigned int) -1;
	printMem(tabP);
	mem_free(tabP[1]);
}

void arriere(void* tabP[16]){
	mem_init();
	tabP[0] = mem_alloc(sizeof(char));
	tabP[1] = mem_alloc(sizeof(char));
	tabP[2] = mem_alloc(sizeof(char));
	
	*((char*)(tabP[0])) = 1;
	*((char*)(tabP[1])) = 2;
	*((char*)(tabP[2])) = 3;
	printMem(tabP);
	*((unsigned int*)(tabP[1]+1)) = (unsigned int) -1;
	printMem(tabP);
	mem_free(tabP[1]);
}

int main(int argc, char *argv[]) {
    void* tabP[16];
	arriere(tabP);
	//avant(tabP);
    return 0;
}
