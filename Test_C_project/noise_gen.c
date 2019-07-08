#include "noise_gen.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

const float STRETCH_2D = -0.211324865405187F; //(1/Math.sqrt(2+1)-1)/2;
const float SQUISH_2D = 0.366025403784439F; //(Math.sqrt(2+1)-1)/2;
const float NORM_2D = 1.0F / 47.0F;

static float gradients2D[] = {
             5,  2,    2,  5,
            -5,  2,   -2,  5,
             5, -2,    2, -5,
            -5, -2,   -2, -5,
};

static float inverseFeatureSize;
static int octaves;
static float persistence;
static float percentage;

static uint8_t perm[256]; 
static uint8_t perm2D[256]; 

static struct Contribution2* lookup2D[64];

static struct Contribution2List* nodesToDelete; 

void init_array_to_null(struct Contribution2* array[], int length) {
    for (int i = 0; i < length; i++) {
        array[i] = NULL;
    }
}

void construct_contribution2(struct Contribution2* pointer, float multiplier, int xsb, int ysb) {
    pointer->dx = -(float)xsb - (multiplier * SQUISH_2D);
    pointer->dy = -(float)ysb - (multiplier * SQUISH_2D);
    pointer->xsb = xsb;
    pointer->ysb = ysb;
    pointer->next = NULL; // should never be assigned before construction
    //printf("Seem to have successfully called construct_contribution2 \n");
}

void static_construct() {
    int base2D_1[] = { 1, 1, 0, 1, 0, 1, 0, 0, 0 };
    int base2D_2[] = { 1, 1, 0, 1, 0, 1, 2, 1, 1 };
    /* int base2D[9][9] = { { 1, 1, 0, 1, 0, 1, 0, 0, 0 }, 
                        { 1, 1, 0, 1, 0, 1, 2, 1, 1 } };*/
	
    int p2D[] = { 0, 0, 1, -1, 0, 0, -1, 1, 0, 2, 1, 1, 1, 2, 2, 0, 1, 2, 0, 2, 1, 0, 0, 0 };
    int lookupPairs2D[] = { 0, 1, 1, 0, 4, 1, 17, 0, 20, 2, 21, 2, 22, 5, 23, 5, 26, 4, 39, 3, 42, 4, 
                            43, 3 };
	
    int lengthOfP2D = sizeof(p2D) / sizeof(p2D[0]);
    struct Contribution2* contributions2D[lengthOfP2D / 4];
    init_array_to_null(contributions2D, lengthOfP2D / 4);

    printf("real length of P2D: 24, calc length: %d \n", lengthOfP2D);

    for (int i = 0; i < lengthOfP2D; i += 4) {
        int lengthOfBase2Ds = sizeof(base2D_1) / sizeof(int);
        int baseSet[lengthOfBase2Ds];

        if (p2D[i] == 0) {
            for (int i = 0; i < lengthOfBase2Ds; i++) {
                baseSet[i] = base2D_1[i];
            }
        } else {
            for (int i = 0; i < lengthOfBase2Ds; i++) {
                baseSet[i] = base2D_2[i];
            }
        }
        //baseSet = (p2D[i] == 0 ? base2D_1 : base2D_2);
        struct Contribution2* previous = NULL;
        struct Contribution2* current = NULL;

        int lengthOfBaseSet = sizeof(baseSet) / sizeof(baseSet[0]);
        printf("Real length of baseSet: 9, calc length: %d \n", lengthOfBaseSet);

        for (int k = 0; k < lengthOfBaseSet; k += 3) {
            current = malloc(sizeof(struct Contribution2));//new Contribution2(baseSet[k], baseSet[k + 1], baseSet[k + 2]);
            construct_contribution2(current, (float)baseSet[k], baseSet[k + 1], baseSet[k + 2]);

            printf("Size of contribution2: %d \n", sizeof(struct Contribution2));

            if (previous == NULL) { // every 4th iteration of i
                contributions2D[i / 4] = current;

                printf("contributions2D as initially genned: %f, %f, %d, %d \n", 
                    contributions2D[i / 4]->dx, contributions2D[i / 4]->dy, 
                    contributions2D[i / 4]->xsb, contributions2D[i / 4]->ysb);
                printf("previous == null for i=%d, k=%d \n", i, k);
            } else {
                if (previous->next != NULL) {
                    printf("previous->next != null! Probably an issue!!! \n");

                    printf("previous->next: addr: %d, dx: %f, dy: %f, xsb: %d, ysb: %d, next != null: %d \n",
                        &(previous->next), previous->next->dx, previous->next->dy, previous->next->xsb,
                        previous->next->ysb, previous->next->next != NULL ? 1 : 0);
                    printf("compared to current: addr: %d, dx: %f, dy: %f, xsb: %d, ysb: %d, next != null: %d \n",
                        &current, current->dx, current->dy, current->xsb, current->ysb, 
                        current->next != NULL ? 1 : 0);
                }

                previous->next = current;

                //printf("As previous != null, previous->next=current \n");
            }

            previous = current;
        }

        struct Contribution2* next = malloc(sizeof(struct Contribution2));//new Contribution2(p2D[i + 1], p2D[i + 2], p2D[i + 3]);
        
        // debug
        if (next == NULL)
            printf("Next is null \n");
        
        construct_contribution2(next, (float)p2D[i + 1], p2D[i + 2], p2D[i + 3]);
        printf("Did the weird init thing with values: %f, %d, %d \n", (float)p2D[i + 1], p2D[i + 2], 
            p2D[i + 3]);

        if (current == NULL)
            printf("Current is null \n");

        current->next = next;
    }

    int lengthOfLookupPairs2D = sizeof(lookupPairs2D) / sizeof(int);
    printf("Real length of lookup pairs 2d: 24, calc length: %d \n", lengthOfLookupPairs2D);
    for (int i = 0; i < lengthOfLookupPairs2D; i += 2) {
        lookup2D[lookupPairs2D[i]] = contributions2D[lookupPairs2D[i + 1]];
    }

    nodesToDelete = malloc(sizeof(struct Contribution2List));
    int contributions2DLength = lengthOfP2D / 4;
    struct Contribution2List* nodeToDeleteH = nodesToDelete;
    //struct Contribution2List* temp = NULL;
    for (int i = 0; i < contributions2DLength - 1; i++) {
        nodeToDeleteH->elem = contributions2D[i];

        nodeToDeleteH->next = malloc(sizeof(struct Contribution2List));
        nodeToDeleteH = nodeToDeleteH->next;
    }
    nodeToDeleteH->elem = contributions2D[contributions2DLength - 1];
    nodeToDeleteH->next = NULL;
}

void debug_check_all_lookupPairs() {
    // this seems to work - must be the "next" chains that fail
    int lengthOfLookup2D = sizeof(lookup2D) / sizeof(struct Contribution2*);

    for (int i = 0; i < lengthOfLookup2D; i++) {
        struct Contribution2* pointer = lookup2D[i];

        if (pointer == NULL) {
            //fprintf(stderr, "POINTER NULL FOR LOOKUP2d[%d]!!! \n", i);
            printf("POINTER NULL FOR LOOKUP2d[%d]!!! \n", i);
        } else {
            float dx = pointer->dx;
            float dy = pointer->dy;
            int xsb = pointer->xsb;
            int ysb = pointer->ysb;
            struct Contribution2* next = pointer->next;

            int nextIsNull = 0;

            if (next != NULL) {
                nextIsNull = 1;
            }

            printf("dx: %f, dy: %f, xsb: %d, ysb: %d, next != null: %d \n", dx, dy, xsb, ysb, 
                nextIsNull);
        } 
    }
}

void debug_check_all_next_chains() {
    int lengthOfLookup2D = sizeof(lookup2D) / sizeof(struct Contribution2*);

    for (int i = 0; i < lengthOfLookup2D; i++) {
        struct Contribution2* pointer = lookup2D[i];

        while (pointer != NULL) {
            float dx = pointer->dx;
            float dy = pointer->dy;
            int xsb = pointer->xsb;
            int ysb = pointer->ysb;
            struct Contribution2* next = pointer->next;

            printf("dx: %f, dy: %f, xsb: %d, ysb: %d, next != null: \n", dx, dy, xsb, ysb, 
            (next == NULL ? 0 : 1));

            pointer = next;
        }

        if (pointer == NULL) {
            //fprintf(stderr, "POINTER NULL FOR LOOKUP2d[%d]!!! next chain \n", i);
            printf("POINTER NULL FOR LOOKUP2d[%d]!!! next chain \n", i);
        } 
    }
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
    printf("New Evaluate call started \n");

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

	int lookup2DLength = sizeof(lookup2D) / sizeof(struct Contribution2*);
    printf("hash: %d, bounds: %d \n", hash, lookup2DLength);

    if (hash < 0 || hash > 63) {
    	printf("Hash would be out of bounds, extra info incoming: \n");
        fprintf(stderr, "Hash would be out of bounds, extra info incoming: \n");
        fprintf(stderr, "x: %f, y: %f, stretchOffset: %f, xs: %f, ys: %f \n", x, y, stretchOffset, xs, ys);
        fprintf(stderr, "xsb: %d, ysb: %d, squishOffset: %f \n", xsb, ysb, squishOffset);
        fprintf(stderr, "dx0: %f, dy0: %f, xins: %f, yins: %f, inSum: %f \n", dx0, dy0, xins, yins, inSum);
        int hashComp1 = (int)(xins - yins + 1);
        int hashComp2 = (int)(inSum) << 1;
        int hashComp3 = (int)(inSum + yins) << 2;
        int hashComp4 = (int)(inSum + xins) << 4;
        fprintf(stderr, "hash comp 1: %d, 2: %d \n", hashComp1, hashComp2);
        fprintf(stderr, "3: %d, 4: %d, hash is bitwise or \n", hashComp3, hashComp4);
    }

    // this seems to be the issue TODO: remove when fixed
    // issue seems to be that hash is -2 (negative in general)
    struct Contribution2* c = lookup2D[hash];

    printf("Managed to get past c = lookup2D[hash] \n");

    float value = 0.0F;

    while (c != NULL) {
        printf("Running loop body \n");
        // manages to run a few iterations before seg-faulting (5, 5, 5) - seems to be 5

        if (c == NULL) {
            printf("c is null, even though while says it isn't \n");
        } else {
            //printf("c isn't null, as while says \n");

            //printf("c's address: %d \n", &c);

            //printf("c->dy: %f \n", c->dy); -> also gives segfault - seems any c->anything
            //printf("c->xsb: %d \n", c->xsb); -> also gives segfault
            //printf("c->next address: %d \n", &(c->next)); // -> does not give segfault - address of 28

            //printf("Another try at dx: %f \n", (*c).dx);
        }
        //printf("c->dx = %f \n", c->dx); // this line: c->dx
        float dx = dx0 + c->dx; 
        //printf("dx = %f \n", dx); 
        float dy = dy0 + c->dy;
        //printf("dy = %f \n", dy);
        float attn = 2 - dx * dx - dy * dy;
        //printf("attn = %f \n", attn);

        printf("All dx, dy, attn are ok \n");

        if (attn > 0) {
            printf("attn > 0 \n");
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
        printf("c = c->next \n");
    }

    //printf("Evaluate call returning");
    return value * NORM_2D;
}

void delete_fucking_everything() {
    struct Contribution2List* head = nodesToDelete;
    struct Contribution2List* temp = NULL;

    while (head != NULL) {
        // dodgy but ok
        temp = head;
        head = head->next;
        free(temp->elem);
        free(temp);
        temp = NULL;
    }
}