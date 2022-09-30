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

int main(int argc, char *argv[]) {
    //Marche jusqu'a 100000 avec gdb
    printf("%ld = %ld + %ld\n",sizeof(mem_busy_block_t),sizeof(size_t),sizeof(void*));
    mem_init();
    mem_alloc(5000);
    mem_show(&afficher_zone);
    mem_alloc(5000);
    mem_show(&afficher_zone);
    printf("2nd alloc \n");
    mem_alloc(100000);
    mem_show(&afficher_zone);
    return 0;
}
