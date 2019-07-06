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

const float STRETCH_2D = -0.211324865405187F; //(1/Math.sqrt(2+1)-1)/2;
const float SQUISH_2D = 0.366025403784439F; //(Math.sqrt(2+1)-1)/2;
const float NORM_2D = 1.0F / 47.0F;

float inverseFeatureSize;
int octaves;
float persistence;
float percentage;

uint8_t perm[256]; 
uint8_t perm2D[256]; 

float gradients2D[] = {
             5,  2,    2,  5,
            -5,  2,   -2,  5,
             5, -2,    2, -5,
            -5, -2,   -2, -5,
};

struct Contribution2* lookup2D[64];

// methods
void static_construct(void);

inline int fast_floor_f(float x);

inline int fast_floor_d(double x);

void setup_noise_seed(long seed);

void setup_noise_time(void);

void setup_noise_lots(long seed, float featureSize, int octaves, float persistence, 
	float percentage);
	
int evaluate_white_cutoff(float x, float y, float whiteCutoff);

float evaluate_2d_config(float x, float y);

float evaluate(float x, float y);

void construct_contribution2(struct Contribution2* pointer, float multiplier, int xsb, int ysb);

void delete_fucking_everything(void);

#endif