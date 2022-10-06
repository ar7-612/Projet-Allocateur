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

void nb_free(){
	void * debutMem = mem_space_get_addr();
    mem_free_block_t* free = ((mem_free_block_t*)debutMem)->next;
	int cpt=0;
    while (free != NULL) {
			cpt++;
            free = free->next;
		}
		printf("il y a %d block libre \n",cpt);
	return ;
}

void ex1(mem_fit_function_t fit){
	mem_init();
    mem_set_fit_handler(fit);
    int* tabP[NB_ALLOC_MAX];
	tabP[0] = mem_alloc(300);
	for(int i=0;i<10;i++){
		tabP[i] = mem_alloc(300);
		mem_alloc(300);
	}
	for(int i=0;i<10;i++){
		tabP[i+10] = mem_alloc(200);
		mem_alloc(300);
	}
	for(int i=0;i<20;i++){
		mem_free(tabP[i]);
	}

	for(int i=0;i<10;i++){
		mem_alloc(132);
	}
	return ;
}

void ex2(mem_fit_function_t fit){
	mem_init();
    mem_set_fit_handler(fit);
    int* tabP[NB_ALLOC_MAX];
		
	//On creer des trous de taille 200 puis de taille 150
	tabP[0] = mem_alloc(300);
	for(int i=0;i<10;i++){
		tabP[i] = mem_alloc(200);
		mem_alloc(300);
	}
	for(int i=0;i<10;i++){
		tabP[i+10] = mem_alloc(150);
		mem_alloc(300);
	}
	for(int i=0;i<20;i++){
		mem_free(tabP[i]);
	}
	
	//On Place des bloque de taille 150 puis 200
	for(int i=0;i<10;i++){
		mem_alloc(150);
	}
	for(int i=0;i<10;i++){
		mem_alloc(200);
	}
	
    // printf("\n");
    // mem_show(&afficher_zone);
	
	return ;
}

void ex3(mem_fit_function_t fit){
	mem_init();
    mem_set_fit_handler(fit);
    int* tabP[NB_ALLOC_MAX];
		
	//On creer des bloque de taille 200
	for(int i=0;i<10;i++){
		tabP[i] = mem_alloc(200);
	}

	//On libere ces bloque en partant de la fin puis on alloue des bloques de taille 150
	for(int i=9;i>=0;i--){
		mem_free(tabP[i]);
		tabP[i] = mem_alloc(150);
	}

    // printf("\n");
    // mem_show(&afficher_zone);
	
	return ;
}

int main(int argc, char *argv[]) {
	printf("test de perf dans le cas 1:\n");
	ex2(mem_best_fit);
	printf("best fit: ");
	nb_free();
	ex2(mem_worst_fit);
	printf("worst fit: ");
	nb_free();
	ex2(mem_first_fit);
	printf("first fit: ");
	nb_free();

	printf("\ntest de perf dans le cas 2:\n");
	ex3(mem_best_fit);
	printf("best fit: ");
	nb_free();
	ex3(mem_worst_fit);
	printf("worst fit: ");
	nb_free();
	ex3(mem_first_fit);
	printf("first fit: ");
	nb_free();

	printf("\ntest de perf dans le cas 3:\n");
	ex1(mem_best_fit);
	printf("best fit: ");
	nb_free();
	ex1(mem_worst_fit);
	printf("worst fit: ");
	nb_free();
	ex1(mem_first_fit);
	printf("first fit: ");
	nb_free();
	return 0;
}


