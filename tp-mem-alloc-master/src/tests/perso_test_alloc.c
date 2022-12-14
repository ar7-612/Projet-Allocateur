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

void printRet(void * p){
    if(p==NULL){
        printf("Pas de place!\n");
    }
    mem_show(&afficher_zone);
	printf("\n");
}

int main(int argc, char *argv[]) {
    mem_init();

    size_t tab[4] = {5000,5000,100000,20000};

    for(int i=0;i<4;i++){
        printRet(mem_alloc(tab[i]));
    }

    mem_init();

    printRet(mem_alloc(127900));
    printRet(mem_alloc(1));

    return 0;
}
