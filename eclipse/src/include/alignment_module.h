#include <stdlib.h>
#include <utility>     // using pair
#include "mraa.h"
#include "lidar_module.h"
#include "navx_module.h"
#include "drive_module.h"

#ifndef ALIGNMENT_MODULE_H
#define ALIGNMENT_MODULE_H

class Alignment_Module{
public:

    float last_lidar_reading;
    int last_navx_reading;
    NavX_Module * navx;
    Lidar_Module * lidar;

    uint16_t current_distance;
    uint16_t desired_distance_cabinet;
    uint16_t desired_distance_edge;
    uint16_t desired_distance_breaker;

    bool lidar_stage1_success;
    bool lidar_stage2_success;
    bool lidar_alignment_complete;

    Alignment_Module() {
        printf("[ init ] Created Alignment Module\n");
        navx = new NavX_Module();
        lidar = new Lidar_Module(3);

    	//INITIALIZATION
    	current_distance = 0;
    	desired_distance_cabinet = 120; //SET THIS ~ 0.10
    	desired_distance_edge = 400; //SET THIS ~ 0.30
    	desired_distance_breaker = 350; //SET THIS ~ 0.15

    	lidar_stage1_success = false;
    	lidar_stage2_success = false;
    	lidar_alignment_complete = false;
    }

    // function aligns using the lidar data
    void lidar_alignment() {

    	current_distance = lidar->get_average_distance_reading_int();

    	//STAGE 1 LIDAR ALIGNMENT - DISTANCE AWAY FROM THE CABINET
    	if (!lidar_stage1_success){

    		//printf("WORKING ON STAGE 1 LIDAR ALIGNMENT\n");

    		if (current_distance > desired_distance_cabinet+25){
    			printf("MOVE FORWARD\n");
    			//MOVE FORWARD Y-DIRECTION
    			//d1.drive('Y', user_input);
    		}
    		else if (current_distance < desired_distance_cabinet-25){
    			printf("MOVE BACKWARD\n");
    			//MOVE BACKWARD Y-DIRECTION
    			//d1.drive('Y', -user_input);
    		}
    		else if ((current_distance <= desired_distance_cabinet+25)&&(current_distance >= desired_distance_cabinet-25)){ //WITH SOME ERROR MARGIN

    			printf("DESIRED DISTANCE!!!\n");
    			// d1.stop(); //CALL DRIVE TO STOP DRIVING
    			//if success, proceed to stage 2
    			printf("STAGE 1 LIDAR ALIGNMENT COMPLETE\n");
    			lidar_stage1_success = true;
    		}
    	}

    	//LIDAR STAGE 2 ALIGNMENT - LEFT TO RIGHT ALIGNMENT X-DIRECTIONS
    	if (lidar_stage1_success){
    		if(current_distance == desired_distance_cabinet){
    			//MOVE RIGHT
    			printf("DRIVING RIGHT\n");
    			//d1.drive('X', user_input);
    		}

    		else if (current_distance == desired_distance_breaker){
    			//MOVE LEFT
    			printf("DRIVING LEFT\n");
    			//d1.drive('X', -user_input);
    		}

    		else if (current_distance == desired_distance_edge){

    			//EDGE DETECTED
    			printf("EDGE DETECTED\n");
    			//STOP
    			//d1.stop();

    			//if success, LIDAR alignment if complete
    			printf("STAGE 2 LIDAR ALIGNMENT COMPLETE\n");
    			lidar_stage2_success = true;
    		}
    	}

    	if (lidar_stage1_success && lidar_stage2_success){
    		printf("ENTIRE LIDAR ALIGNMENT COMPLETE\n");
    		// lidar_alignment_complete = true; //eventually should return TRUE upon exit
    	}

    }

    // function aligns using image processing data
    void camera_alignment(int16_t instructions[]) {

    }

    // function to return navx rotation value
    int get_rotation() {
        return navx->get_yaw();
    }

    // reorients navx
    void set_zero() {
        navx->set_zero();
    }





};


#endif
