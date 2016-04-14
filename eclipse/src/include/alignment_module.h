#include <stdlib.h>
#include <utility>     // using pair
#include "mraa.h"
#include "lidar_module.h"
#include "navx_module.h"

#ifndef ALIGNMENT_MODULE_H
#define ALIGNMENT_MODULE_H

class Alignment_Module{
public:

    float last_lidar_reading;
    int last_navx_reading;
    NavX_Module * navx;
    Lidar_Module * lidar;

    Alignment_Module() {
        printf("[ init ] Created Alignment Module\n");
        navx = new NavX_Module();
        lidar = new Lidar_Module(0);
    }

    // function aligns using the lidar data
    void lidar_alignment() {

    }

    // function aligns using image processing data
    void image_alignment(int16_t instructions[]) {

    }

    // function to return navx rotation value
    int get_rotation() {
        return navx.get_yaw();
    }

    // reorients navx
    void set_zero() {
        navx.set_zero();
    }





};


#endif
