#include <stdio.h>
#include "functions.h"
#include "noise_gen.h"

void test_noise_gen() {
	static_construct();
	setup_noise_lots((long)(80117114112108101F * ((double)rand() / (double)RAND_MAX)), 
            1, 20, 0.45F, 1);
            
    float whiteCutoff = -0.2F;
    int cumulativeSectionsDeployed = 0;
    
    for (int j = 0; j < 89; j++) {
    	for (int i = 0; i < 16; i++) {
    		if (evaluate_white_cutoff(cumulativeSectionsDeployed - 1, i, whiteCutoff) == 1) {
    			printf("Face is added");
    		}
    	}
    	
    	cumulativeSectionsDeployed++
    }
    
    int extraReps = 200;
    int counter = 0;
    
    while (counter < extraReps) {
    	for (int i = 0; i < 16; i++) {
    		if (evaluate_white_cutoff(cumulativeSectionsDeployed - 1, i, whiteCutoff) == 1) {
    			printf("Face is added");
    		}
    	}
    	
    	counter++;
    	cumulativeSectionsDeployed++;
    }
    
    printf("Process complete, check error log (where-ever that is).");
    delete_fucking_everything();
}

int main() {
    int a = 3;
    int b = 4;

    int c = sum(a, b);

    printf("Hello, world!\n");
    
    printf("%d", c);

    test_noise_gen();
    
    return 0;
}