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
    int destination_x;
    int destination_y;

    // statuses
    bool is_rotating_ccw;
    bool is_rotating_cw;

    // * drive navx, video directions, optical encoder, lidar
    Alignment_Module(mraa_i2c_context ctx) {
        i2c = ctx;
        //needs to create optical encoder module
        //needs to create lidar module
        is_rotating_ccw = false;
        is_rotating_cw = false;
    }

    /*
        @return phase of alignment (1 = rotation, 2 = X, 3 = Y, 4 = X)
     */
    int align(Drive_Module * p_d1, int rotation, int dest_x, int dest_y, bool valid_dest) {
        if (valid_dest) {
            // set destination_x if camera measured correctly
            destination_x = dest_x;
            destination_y = dest_y;
        }
        if (rotation == -1) {
            //error cannot align without navx working
            return -1;
        }
        // PHASE 1: rotate to correct position
        printf("Rotation %d\n", rotation);
        if (rotation > 5 && rotation < 355) {
            if (rotation > 180) {
                // rotate clockwise
                is_rotating_ccw = false;
                p_d1->drive('Z', 0.225);
                if(!is_rotating_cw) {
                    printf("ROTATE CW\n");
                    //stop to clear cached vars in swerve module
                    p_d1->clear_cached();
                    is_rotating_cw = true;
                }
                return 1;
            } else {
                // rotate counterclockwise
                is_rotating_cw = false;
                p_d1->drive('Z', -0.09);
                if (!is_rotating_ccw) {
                    printf("ROTATE CCW\n");
                    //stop to clear cached vars in swerve module
                    p_d1->clear_cached();
                    is_rotating_ccw = true;
                }
                return 1;
            }
        } else {
            printf("CORRECT POSITION\n");
            p_d1->stop();
            sleep(1);
        }
        return 0;
        // PHASE 2: rough alignment in x axis use optical encoders to set stopping boundaries
        // printf("rotation %d, move_x %d, move_y, %d\n", rotation, dest_x, dest_y);
        // dest x : 0 = stop, 1 = go right, 2 = go left
        // dest y : 0 = stop, 1 = forward, 2 = backward

        if (destination_x != 0) {
            // is_rotating_ccw = false;
            // is_rotating_cw = false;

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
