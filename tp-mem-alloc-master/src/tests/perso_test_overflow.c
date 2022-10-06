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
	
	*((char*)(tabP[0])) = 10;
	*((char*)(tabP[1])) = 20;
	*((char*)(tabP[2])) = 30;
	printMem(tabP);
	*((unsigned int*)(tabP[1]-1)) = (unsigned int) -1;
	printMem(tabP);
	//mem_free(tabP[1]);
}

void arriere(void* tabP[16]){
	mem_init();
	tabP[0] = mem_alloc(sizeof(char));
	tabP[1] = mem_alloc(sizeof(char));
	tabP[2] = mem_alloc(sizeof(char));
	
	*((char*)(tabP[0])) = 10;
	*((char*)(tabP[1])) = 20;
	*((char*)(tabP[2])) = 30;
	printMem(tabP);
	*((unsigned int*)(tabP[1]+1)) = (unsigned int) -1;
	printMem(tabP);
	//mem_free(tabP[1]);
}

int main(int argc, char *argv[]) {
    //Commentez les mem_free pour observer l'etat de la memoire
	void* tabP[16];	
	//arriere(tabP);
	avant(tabP);//Non detecter (alignement)
    return 0;
}
