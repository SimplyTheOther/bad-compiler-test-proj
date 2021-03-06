#include "functions.h"
#include "noise_gen.h"
#include <stdio.h>
#include <stdlib.h>

void test_noise_gen() {
    printf("Started noise gen! \n");

    static_construct(); // completes, apparently
    setup_noise_lots((long)(80117114112108101.0F * ((double)rand() / (double)RAND_MAX)), 1, 20, 0.45F,
      1); // completes, apparently

    // debug
    debug_check_all_lookupPairs();
    debug_check_all_next_chains();

    float whiteCutoff = -0.2F;
    int cumulativeSectionsDeployed = 0;

    for (int j = 0; j < 89; j++) {
        for (int i = 0; i < 16; i++) {
            if (evaluate_white_cutoff((float)(cumulativeSectionsDeployed - 1), (float)i, whiteCutoff)
                == 1) {
                printf("Face is added \n");
            }
        }

        cumulativeSectionsDeployed++;
    }

    int extraReps = 200;
    int counter = 0;

    while (counter < extraReps) {
        for (int i = 0; i < 16; i++) {
            if (evaluate_white_cutoff((float)(cumulativeSectionsDeployed - 1), (float)i, whiteCutoff)
                == 1) {
                printf("Face is added \n");
            }
        }

        counter++;
        cumulativeSectionsDeployed++;
    }

    printf("Process complete, check error log (where-ever that is). \n");
    delete_fucking_everything();
}

void test_noise_gen_trace_table() {
	printf("Started noise gen \n");
	
	static_construct();
	setup_noise_lots((long)(80117114112108101.0F * ((double)rand() / (double)RAND_MAX)), 1, 20, 0.45F,
      1);
	  
	float x;
	float y;
	int shouldContinue;
	
	do {
		printf("Input x: ");
		scanf("%f", &x);
		printf("Input y: ");
		scanf("%f", &y);
		
		// call actual function
		evaluateWithAllValuesWrittenToConsole(x, y);
		
		printf("Continue for another iteration? Write 1 for yes and 0 for no. \n"); 
		scanf("%d", &shouldContinue);
	} while (shouldContinue == 1);
	  
	printf("Process complete. \n");
    delete_fucking_everything();
}

int main() {
    int a = 3;
    int b = 4;

    int c = sum(a, b);

    printf("Hello, world!\n");

    printf("%d \n", c);
	
	test_noise_gen_trace_table();

	// TODO: turn back on later
    //test_noise_gen();

    return 0;
}