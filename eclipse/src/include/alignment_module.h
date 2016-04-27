#include <stdlib.h>
#include "mraa.h"
#include "drive_module.h"
#include "navx_module.h"

#ifndef ALIGNMENT_MODULE_H
#define ALIGNMENT_MODULE_H

#define ROT_SPEED 0.25
#define DRIVE_SPEED 0.1

class Alignment_Module {
public:
    mraa_i2c_context i2c;
    int destination_x; // location of where target is
    int destination_y;

    // * drive navx, video directions, optical encoder, lidar
    Alignment_Module(mraa_i2c_context ctx) {
        i2c = ctx;
        //needs to create optical encoder module
        //needs to create lidar module

    }

    /*
        @return phase of alignment (1 = rotation, 2 = X, 3 = Y, 4 = X)
     */
    int align(Drive_Module * p_d1, int rotation, int dest_x, int dest_y, bool valid_dest) {
        printf("rotation %d, move_x %d, move_y, %d\n", rotation, dest_x, dest_y);
        return 0;
        // dest x : 0 = stop, 1 = go right, 2 = go left
        // dest y : 0 = stop, 1 = forward, 2 = backward
        if (valid_dest) {
            // set destination_x if camera measured correctly
            destination_x = dest_x;
        }
        if (rotation != -1) {
            //error cannot align without navx working
            return -1;
        }
        // PHASE 1: rotate to correct position
        if (!(rotation > 359 && rotation < 1)) {
            printf("PHASE 1 ROTATION\n");
            if (rotation > 180) {
                // rotate clockwise
                p_d1->drive('Z', ROT_SPEED);
                return 1;
            } else {
                // rotate counterclockwise
                p_d1->drive('Z', -ROT_SPEED);
                return 1;
            }
        }

        // PHASE 2: rough alignment in x axis use optical encoders to set stopping boundaries
        if (destination_x != 0) {
            printf("PHASE 2 ROTATION\n");
            if (destination_x > 0) {
                // drive to the right
                p_d1->drive('X', -DRIVE_SPEED);
                return 1;
            } else {
                // drive to the left
                p_d1->drive('X', DRIVE_SPEED);
                return 1;
            }
        } else {
            // aligned with breaker
            p_d1->stop();
            return 2;
        }

        // PHASE 3: align in y axis

        // logic messed up
        p_d1->stop();
        return 0;
    }
};


#endif
