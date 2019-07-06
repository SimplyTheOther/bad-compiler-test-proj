#include "noise_gen.h"
#include <stddef.h>
#include <stdio.h>

void construct_contribution2(struct Contribution2* pointer, float multiplier, int xsb, int ysb) {
	pointer->dx = -xsb - (multiplier * SQUISH_2D);
	pointer->dy = -ysb - (multiplier * SQUISH_2D);
	pointer->xsb = xsb;
	pointer->ysb = ysb;
}

void static_construct() {
	//int base2D_1[] = { 1, 1, 0, 1, 0, 1, 0, 0, 0 };
	//int base2D_2[] = { 1, 1, 0, 1, 0, 1, 2, 1, 1 };
	int base2D[9][9] = { { 1, 1, 0, 1, 0, 1, 0, 0, 0 }, 
                        { 1, 1, 0, 1, 0, 1, 2, 1, 1 } };
	
	int p2D[] = { 0, 0, 1, -1, 0, 0, -1, 1, 0, 2, 1, 1, 1, 2, 2, 0, 1, 2, 0, 2, 1, 0, 0, 0 };
	int lookupPairs2D[] = { 0, 1, 1, 0, 4, 1, 17, 0, 20, 2, 21, 2, 22, 5, 23, 5, 26, 4, 39, 3, 42, 4, 
                            43, 3 };
	
	int lengthOfP2D = sizeof(p2D) / sizeof(p2D[0]);
	struct Contribution2* contributions2D[lengthOfP2D / 4];

    for (int i = 0; i < lengthOfP2D; i += 4) {
        int baseSet[] = base2D[p2D[i]];
        struct Contribution2* previous = NULL;
        struct Contribution2* current = NULL;

		int lengthOfBaseSet = sizeof(baseSet) / sizeof(baseSet[0]);

        for (int k = 0; k < lengthOfBaseSet; k += 3) {
            current = malloc(sizeof(struct Contribution2));//new Contribution2(baseSet[k], baseSet[k + 1], baseSet[k + 2]);
			construct_contribution2(current, baseSet[k], baseSet[k + 1], baseSet[k + 2]);

            if (previous == NULL) {
                contributions2D[i / 4] = current;
            } else {
                previous->next = current;
            }

            previous = current;
        }

        struct Contribution2* next = malloc(sizeof(struct Contribution2));//new Contribution2(p2D[i + 1], p2D[i + 2], p2D[i + 3]);
        construct_contribution2(next, p2D[i + 1], p2D[i + 2], p2D[i + 3]);
        current->next = next;
    }

	int lengthOfLookupPairs2D = sizeof(lookupPairs2D) / sizeof(struct Contribution2*);
    for (int i = 0; i < lengthOfLookupPairs2D; i += 2) {
        lookup2D[lookupPairs2D[i]] = contributions2D[lookupPairs2D[i + 1]];
    }
}

inline int fast_floor_f(float x) {
    int xi = (int)x;

    return x < xi ? xi - 1 : xi;
}

inline int fast_floor_d(double x) {
    int xi = (int)x;

    return x < xi ? xi - 1 : xi;
}

void setup_noise_seed(long seed) {
    uint8_t source[256];

    for (int i = 0; i < 256; i++) {
        source[i] = (uint8_t)i;
    }

    seed = seed * 6364136223846793005L + 1442695040888963407L;
    seed = seed * 6364136223846793005L + 1442695040888963407L;
    seed = seed * 6364136223846793005L + 1442695040888963407L;

    for (int i = 255; i >= 0; i--) {
        seed = seed * 6364136223846793005L + 1442695040888963407L;
        int r = (int)((seed + 31) % (i + 1));

        if (r < 0) {
            r += (i + 1);
        }

        perm[i] = source[r];
        perm2D[i] = (uint8_t)(perm[i] & 0x0E);
        source[r] = source[i];
    }
}

/* void setup_noise_time(void) {

}*/

void setup_noise_lots(long seed, float pFeatureSize, int pOctaves, float pPersistence, 
	float pPercentage) {
    setup_noise_seed(seed);

    inverseFeatureSize = 1 / pFeatureSize;
    octaves = pOctaves;
    persistence = pPersistence;
    percentage = pPercentage;
}

int evaluate_white_cutoff(float x, float y, float whiteCutoff) {
    return (evaluate_2d_config(x, y) > whiteCutoff ? 1 : 0);
}

float evaluate_2d_config(float x, float y) {
    float colourSum = 0;
    float amplitude = 1;
    float frequency = 0.25F;
    float maxAmplitude = 0;

    for (int octave = 0; octave < octaves; octave++) {
        colourSum += evaluate(x * inverseFeatureSize * frequency, y * inverseFeatureSize * frequency) 
        * amplitude;

        maxAmplitude += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }

    return (colourSum / maxAmplitude);
}

float evaluate(float x, float y) {
    printf("New Evaluate call started");

    float stretchOffset = (x + y) * STRETCH_2D;
    float xs = x + stretchOffset;
    float ys = y + stretchOffset;

    int xsb = fast_floor_f(xs);
    int ysb = fast_floor_f(ys);

    float squishOffset = (xsb + ysb) * SQUISH_2D;
    float dx0 = x - (xsb + squishOffset);
    float dy0 = y - (ysb + squishOffset);

    float xins = xs - xsb;
    float yins = ys - ysb;

    float inSum = xins + yins;

    int hash =
        (int)(xins - yins + 1) |
        (int)(inSum) << 1 |
        (int)(inSum + yins) << 2 |
        (int)(inSum + xins) << 4;

    printf("hash: {hash}, bounds: {lookup2D.Length}");

    if (hash < 0 || hash > 63) {
        printf("Hash would be out of bounds, extra info incoming:");
        printf("x: {x}, y: {y}, stretchOffset: {stretchOffset}, xs: {xs}, ys: {ys}");
        printf("xsb: {xsb}, ysb: {ysb}, squishOffset: {squishOffset}");
        printf("dx0: {dx0}, dy0: {dy0}, xins: {xins}, yins: {yins}, inSum: {inSum}");
        printf("hash comp 1: {(int)(xins -yins + 1)}, 2: {(int)(inSum) << 1}");
        printf("3: {(int)(inSum + yins) << 2}, 4: {(int)(inSum + xins) << 4}, hash is bitwise or");
    }

    // this seems to be the issue TODO: remove when fixed
    // issue seems to be that hash is -2 (negative in general)
    struct Contribution2* c = lookup2D[hash];

    float value = 0.0F;

    while (c != NULL) {
        float dx = dx0 + c->dx;
        float dy = dy0 + c->dy;
        float attn = 2 - dx * dx - dy * dy;

        if (attn > 0) {
            int px = xsb + c->xsb;
            int py = ysb + c->ysb;

            //printf("perm bounds: {perm.Length}, perm2D bounds: {perm2D.Length}. perm index attempt: {px & 0xFF}");
            //printf("perm2D index attempt: {(perm[px & 0xFF] + py) & 0xFF}");

            uint8_t i = perm2D[(perm[px & 0xFF] + py) & 0xFF];
            //printf("gradients2D bounds: {gradients2D.Length}, attempted gradients2D indices: {i}, {i + 1}");
            float valuePart = gradients2D[i] * dx + gradients2D[i + 1] * dy;

            attn *= attn;
            value += attn * attn * valuePart;
        }

        c = c->next;
    }

    //printf("Evaluate call returning");
    return value * NORM_2D;
}

void delete_fucking_everything() {
    int lookup2DLength = sizeof(lookup2D) / sizeof(struct Contribution2*);

    for (int i = 0; i < lookup2DLength; i++) {
        struct Contribution2* atIndex = lookup2D[i];

        free(atIndex->next);
        free(atIndex);
    }
}