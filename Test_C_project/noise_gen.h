#ifndef NOISEGEN_H_INCLUDED
#define NOISEGEN_H_INCLUDED

#include <stdint.h>

struct Contribution2 {
	float dx;
	float dy;
	int xsb;
	int ysb;
	struct Contribution2* next;	
};

struct Contribution2List {
	struct Contribution2* elem;
	struct Contribution2List* next;
};

// methods
void static_construct(void);

static inline int fast_floor_f(float x) {
    int xi = (int)x;

    return x < xi ? xi - 1 : xi;
}

static inline int fast_floor_d(double x) {
    int xi = (int)x;

    return x < xi ? xi - 1 : xi;
}

void setup_noise_seed(long seed);

void setup_noise_time(void);

void setup_noise_lots(long seed, float featureSize, int octaves, float persistence, 
	float percentage);
	
int evaluate_white_cutoff(float x, float y, float whiteCutoff);

float evaluate_2d_config(float x, float y);

float evaluate(float x, float y);

void construct_contribution2(struct Contribution2* pointer, float multiplier, int xsb, int ysb);

void debug_check_all_lookupPairs(void);

void delete_fucking_everything(void);

void debug_check_all_next_chains(void);

void init_array_to_null(struct Contribution2* array[], int length);

#endif