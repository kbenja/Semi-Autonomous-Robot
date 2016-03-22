#include "mraa/aio.h"
#include "i2c_library.h"

#ifndef ENCODER_MODULE_H
#define ENCODER_MODULE_H

// SCL = Yello
// SDA = White

class Encoder_Module {
int id;
public:
    Encoder_Module() {
        id = 1;
        printf("[ init ] Motor module initialized with id %d\n", id);
    }
    Encoder_Module(int set_id) {
        id = set_id;
        printf("[ init ] Motor module initialized with id %d\n", id);
    }
};


#endif
