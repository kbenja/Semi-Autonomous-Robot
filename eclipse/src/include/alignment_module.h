#include <stdlib.h>
#include "mraa.h"
#include "drive_module.h"
#include "navx_module.h"

#ifndef ALIGNMENT_MODULE_H
#define ALIGNMENT_MODULE_H

#define POS_SPEED 0.225
#define NEG_SPEED -0.09

class Alignment_Module {
public:
    mraa_i2c_context i2c;
    int destination_x;
    int destination_y;

    // statuses
    bool is_rotating_ccw;
    bool is_rotating_cw;

    bool aligning_x;
    bool aligning_y;

    // * drive navx, video directions, optical encoder, lidar
    Alignment_Module(mraa_i2c_context ctx) {
        i2c = ctx;
        //needs to create optical encoder module
        //needs to create lidar module
        is_rotating_ccw = false;
        is_rotating_cw = false;
        aligning_x = false;
        aligning_y = false;
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
        if (rotation > 2 && rotation < 358) {
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
        }
        // else {
        //     aligning_x = true;
        // }

        // PHASE 2: rough alignment in x axis use optical encoders to set stopping boundaries
        // dest x : 0 = stop, 1 = go right, 2 = go left

        if(aligning_x) {
            if (destination_x != 0) {
                // is_rotating_ccw = false;
                // is_rotating_cw = false;
                if (destination_x == 1) {
                    // drive to the right
                    printf("MOVING RIGHT\n");
                    p_d1->drive('X', NEG_SPEED);
                    return 2;
                } else {
                    // drive to the left
                    printf("MOVING LEFT\n");
                    p_d1->drive('X', POS_SPEED);
                    return 2;
                }
            } else {
                p_d1->stop();
                aligning_x = false;
                aligning_y = true;
                return 3;
            }
        }

        // // PHASE 3: align in y axis
        // // dest y : 0 = stop, 1 = forward, 2 = backward
        // if(aligning_y) {
        //     if (destination_y != 0) {
        //         // is_rotating_ccw = false;
        //         // is_rotating_cw = false;
        //         if (destination_y == 1) {
        //             // drive to the right
        //             printf("MOVING FORWARD\n");
        //             p_d1->drive('Y', POS_SPEED);
        //             return 2;
        //         } else {
        //             // drive to the left
        //             printf("MOVING BACKWARD\n");
        //             p_d1->drive('Y', NEG_SPEED);
        //             return 2;
        //         }
        //     } else {
        //         // aligned with breaker
        //         printf("STOPPING\n");
        //         p_d1->stop();
        //         return 3;
        //     }
        // }

        // logic messed up
        p_d1->stop();
        printf("ALIGNED!\n");
        return 0;
    }
};


#endif
