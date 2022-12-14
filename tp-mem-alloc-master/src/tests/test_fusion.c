//------------------------------------------------------------------------------
// Projet : TP CSE (malloc)
// Cours  : Conception des systèmes d'exploitation et programmation concurrente
// Cursus : Université Grenoble Alpes - UFRIM²AG - Master 1 - Informatique
// Année  : 2022-2023
//------------------------------------------------------------------------------

#include "../mem_space.h"
#include "../mem.h"
#include "../mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ALLOC (1 << 10)
#define NB_TESTS 5

void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %7s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           (unsigned long)adresse, (unsigned long)taille);
}

// adresse par rapport au début de la mémoire
void *relative_adr(void *adr) {
    return (void *)((char *)adr - (char *)mem_space_get_addr());
}

void my_free(void **mem) {
    if (*mem != NULL) {
        mem_free(*mem);
        debug("Freed %p\n", relative_adr(*mem));
        *mem = NULL;
    }
}

static void *checked_alloc(size_t s) {
    void *result;

    assert((result = mem_alloc(s)) != NULL);
    debug("Alloced %zu bytes at %p\n", s, relative_adr(result));
    return result;
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

static void alloc5(void **ptr) {
    ptr[0] = checked_alloc(MAX_ALLOC);
    ptr[1] = checked_alloc(MAX_ALLOC);
    ptr[2] = checked_alloc(MAX_ALLOC);
    ptr[3] = checked_alloc(MAX_ALLOC);
    ptr[4] = alloc_max(mem_space_get_size() - 4 * MAX_ALLOC);
}

static void free5(void **ptr) {
    for (int i = 0; i < 5; i++) {
        my_free(&ptr[i]);
    }
}

int main(int argc, char *argv[]) {
    printf("%zd", sizeof(void *));
    void *ptr[5];

    mem_init();
    fprintf(stderr,
            "Test réalisant divers cas de fusion (avant, arrière et double\n"
            "Définir DEBUG à la compilation pour avoir une sortie un peu plus "
            "verbeuse."
            "\n");
    for (int i = 0; i < NB_TESTS; i++) {
        debug("Fusion avant\n");
        alloc5(ptr);
        my_free(&ptr[2]);
        my_free(&ptr[1]);

        //mem_show(afficher_zone);
        //printf("==================\n");

        ptr[1] = checked_alloc(2 * MAX_ALLOC);
        free5(ptr);

        debug("Fusion arrière\n");
        alloc5(ptr);
        my_free(&ptr[1]);
        my_free(&ptr[2]);
        ptr[1] = checked_alloc(2 * MAX_ALLOC);
        free5(ptr);

        debug("Fusion avant/arrière\n");
        alloc5(ptr);
        my_free(&ptr[1]);
        my_free(&ptr[3]);
        my_free(&ptr[2]);
        ptr[1] = checked_alloc(3 * MAX_ALLOC);
        free5(ptr);
    }

    // TEST OK
    printf("TEST OK!!\n");
    return 0;
}
