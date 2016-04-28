#include <stdlib.h>
#include "mraa.h"
#include "drive_module.h"
#include "navx_module.h"

#ifndef ALIGNMENT_MODULE_H
#define ALIGNMENT_MODULE_H

#define POS_SPEED 0.225
#define NEG_SPEED -0.09
#define ROTATE_SPEED_POS 0.225
#define ROTATE_SPEED_NEG -0.09

class Alignment_Module {
public:
    mraa_i2c_context i2c;
    int destination_x;
    int destination_y;

    int state;
    int waitcount;
    bool waiting;

    // * drive navx, video directions, optical encoder, lidar
    Alignment_Module(mraa_i2c_context ctx) {
        i2c = ctx;
        state = 0;
        waitcount = 0;
        waiting = false;
    }

    /*
        @return phase of alignment (1 = rotation, 2 = X, 3 = Y, 4 = X)
     */
    int align(Drive_Module * p_d1, int navx_result, int dest_x, int dest_y, bool valid_dest) {
        if (valid_dest) {
            destination_x = dest_x;
            destination_y = dest_y;
        }
        if (navx_result == -1) {
            return -1; //error cannot align without navx working
        }
        if(waiting) {       //if previous alignment stage complete use counter to wait
            printf("Waiting\n");
            waitcount++;
            if(waitcount > 10) {
                waiting = false;
                waitcount = 0;
            }
            return state;
        } else {            //if previous alignment stage not complete:
            // CHECK PHASE 1, Z axis
            int result = check_z(navx_result);     //check rotation
            if (result != 0 && state != 1 && state != 3) {          //if rotation not good, stop and redo rotation alignment
                p_d1->stop();
                waiting = true;
                state = 1;
                return state;
            }
            //
            if(state == 1) {            //State 1: rotation alignment
                alignment_z(p_d1, result);        //drive motors based on result of check_z (CW/CCW/STOP)
                if (result == 0) {          //if motor was stopped (i.e. rotational alignment done)
                    printf("COMPLETED ALIGNMENT Z\n");
                    waiting = true;             //start waiting and jump to next state (X alignment)
                    state = 2;
                }
                return state;
            }
            // CHECK PHASE 2, X axis
            else if (destination_x != 0 && state != 2) {
                 p_d1->stop();
                 waiting = true;
                 state = 2;
                 return state;
            }
            // IF STATE = 2, DO STATE 2
            if (state == 2) {    //State 2: X alignment
                result = destination_x;         //check_x alignment
                alignment_x(p_d1, result);        //drive motor based on x alignment reading (L/R/STOP)
                if (result == 0) {          //if motor was stopped (i.e. X alignment done)
                    printf("COMPLETED ALIGNMENT X\n");
                    waiting = true;             //start waiting and jump to next state (Y alignment)
                    state = 3;
                }
                return state;
            }
            // CHECK PHASE 3, Y axis
            else if (destination_y != 0 && state != 3) {
                 p_d1->stop();
                 waiting = true;
                 state = 3;
                 return state;
            }
            // IF STATE = 3, DO STATE 3
            if (state == 3) {    //State 3: Y alignment
                result = destination_y;         //check_y alignment
                alignment_y(p_d1, result);        //drive motor based on alignment reading (F/B/STOP)
                if (result == 0) {          //if motor was stopped (Y-alignment done)
                    printf("COMPLETED ALIGNMENT Y\n");
                    waiting = true;             //start waiting and jump to next state (All aligned)
                    state = 4;
                }
                return state;
            }
            // FINAL STATE = 4
            if(state == 4) {
                printf("ALIGNED!!\n");
                return state;
            }
        }
        return state;
    }


    // check status of past states
    int check_z(int navx_result) {
        printf("CHECK Z\n");
        if (navx_result > 1 && navx_result < 359) {     //if out of rotation alignment
            if (navx_result > 180) {
                return 2;       //rotate clockwise
            } else {
                return 1;       //rotate counterclockwise
            }
        } else {
            return 0;           //no rotation (correctly aligned rotationally)
        }
    }

    //motor command based on state
    void alignment_z(Drive_Module * p_d1, int state) {
        switch(state) {
            case 0:             //no rotation --> no movement
                p_d1->stop();
                printf("BREAK IN Z\n");
                break;
            case 1:             //rotate counterclockwise --> rotate negative direction
                p_d1->drive('Z', ROTATE_SPEED_NEG);
                printf("ROTATING CCW\n");
                break;
            case 2:             //rotate clockwises --> rotate positive direction
                p_d1->drive('Z', ROTATE_SPEED_POS);
                printf("ROTATING CW\n");
                break;
        }
    }

    // should return 0 if done
    void alignment_x(Drive_Module * p_d1, int state) {
        // PHASE 2: rough alignment in x axis use optical encoders to set stopping boundaries
        // dest x : 0 = stop, 1 = go right, 2 = go left
        switch(state) {
            case 0:
                p_d1->stop();
                printf("BREAK IN X\n");
                break;
            case 1:
                p_d1->drive('X', -0.08);
                printf("DRIVE RIGHT\n");
                break;
            case 2:
                p_d1->drive('X', 0.19);
                printf("DRIVE LEFT\n");
                break;
        }
    }

    void alignment_y(Drive_Module * p_d1, int state) {
        // PHASE 2: rough alignment in x axis use optical encoders to set stopping boundaries
        // dest x : 0 = stop, 1 = go right, 2 = go left
        switch(state) {
            case 0:
                p_d1->stop();
                printf("BREAK IN Y\n");
                break;
            case 1:
                p_d1->drive('Y',  0.19);
                printf("DRIVE FORWARDS\n");
                break;
            case 2:
                p_d1->drive('Y', -0.08);
                printf("DRIVE BACKWARDS\n");
                break;
        }
    }
};


#endif
