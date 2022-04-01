// router.c

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "dist_rand.h"

#define SIM_LEN 1000000

struct bozon{
    int state;
    float time;
    int interupted;
    int yodels_made;
    int perfect_yodels;
};

struct bozon *generate_bozons(int len){
   struct bozon *bozons = malloc(len * sizeof(struct bozon));
   for(int i = 0; i < len; i++){
       bozons[i].state = 0;
       bozons[i].time = 0;
       bozons[i].interupted = 0;
       bozons[i].perfect_yodels = 0;
       bozons[i].yodels_made = 0;
   }
   return bozons;
}  

void evaluate_state(struct bozon *b, double wake_chance, double mean_yodel, int *active){
    double rand = Uniform01();
    

    if(b->state == 0){
        if(rand < wake_chance){

            b->state = 1;
            b->interupted = 0;
            b->time = ceil(Exponential(mean_yodel));
            // b->yodels_made++;
            (*active)++;
        }   
    }
    else if(b->state == 1){
        b->state = 0;
        if(b->interupted == 0){
            b->perfect_yodels++;
        }
        (*active)--;
    }

}

int main(int argc, char* argv[]){
    int num_bozons = 1; 
    float mean_yodel = 10, percent_active = 10;
    time_t t;

    int active_yodelers = 0;
    int melodious_time = 0;
    int screaching = 0;
    int perfect_yodels = 0;
    int total_yodels = 0;

    srand((unsigned) time(&t));

    if(argc > 1){
        num_bozons = atoi(argv[1]);
    }

    if(argc > 2){
        percent_active = atof(argv[2]);
    }

    if(argc > 3){
        mean_yodel = atof(argv[2]);
    }

    struct bozon *bozons;
    bozons = generate_bozons(num_bozons);
    double wake_chance =  ((percent_active / 100) * SIM_LEN) / mean_yodel / SIM_LEN;

    for(int i = 0; i < SIM_LEN; i++){
       for(int b = 0; b < num_bozons; b++){
            bozons[b].time--;
            if(bozons[b].time <= 0){
                evaluate_state(&bozons[b], wake_chance, mean_yodel, &active_yodelers);

        // printf("%d\n", active_yodelers);

            }
            // printf("%f \n", bozons[b].time);

        }

        if(active_yodelers > 1){
            for(int b = 0; b < num_bozons; b++){
                if(bozons[b].state == 1){
                    bozons[b].interupted = 1;
                }
            }    
        }

        if(active_yodelers == 1){
            melodious_time ++;
        }
        else if(active_yodelers > 1){
            screaching++;
        }

        for(int b = 0; b < SIM_LEN; b++){
            perfect_yodels += bozons[b].perfect_yodels;
            total_yodels += bozons[b].yodels_made;
        }
    }

    printf("%d, %f%%\n", melodious_time, (melodious_time / (double)SIM_LEN) * 100);
    printf("%d, %f%%\n", screaching, (screaching / (double)SIM_LEN) * 100);
    printf("%d, %f%%\n", screaching, (perfect_yodels / (double)total_yodels) * 100);
    
}
