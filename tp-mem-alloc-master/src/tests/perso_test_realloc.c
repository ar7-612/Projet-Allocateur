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

int main(int argc, char *argv[]) {
    mem_init();
    /*
    mem_show(&afficher_zone);
    void* p = mem_alloc(16);//alloc_max(mem_space_get_size());
    mem_show(&afficher_zone);
    mem_free(p);
    mem_show(&afficher_zone);
    */
    

    int nbTest = 16;
    int* tabP[16];
    int tabFree[16]={1,3,2,5,4,6,9,8,10,15,13,12,11,7,14,0};
    
    printf("\n");
    mem_show(&afficher_zone);
    for(int i=0;i<nbTest;i++){
        tabP[i]=mem_alloc(sizeof(unsigned int));
		*tabP[i]=i;
    }
	
	printf("\n");
    mem_show(&afficher_zone);
	
	for(int i=0;i<nbTest;i++){
		printf("%d ",*tabP[tabFree[i]]);
        tabP[tabFree[i]]=mem_realloc(tabP[tabFree[i]],sizeof(unsigned int)*10);
    }
	
    printf("\n");
    mem_show(&afficher_zone);
    for(int i=0;i<nbTest;i++){
		printf("%d ",*tabP[tabFree[i]]);
        mem_free(tabP[tabFree[i]]);
    }
    printf("\n");
    mem_show(&afficher_zone);
    
    return 0;
}
