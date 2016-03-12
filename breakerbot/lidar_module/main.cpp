// from https://raw.githubusercontent.com/intel-iot-devkit/mraa/master/examples/analogin_a0.c

#include <unistd.h>
#include <iostream>
//! [Interesting]
#include "mraa/aio.h"

int main()
{
    int count = 0;
    mraa_aio_context adc_a0;
    uint16_t adc_value = 0;
    float adc_value_float = 0.0;

    adc_a0 = mraa_aio_init(0);
    if (adc_a0 == NULL) {
        return 1;
    }

    for (;;) {
        count++;
        adc_value = mraa_aio_read(adc_a0);
        adc_value_float = mraa_aio_read_float(adc_a0);
        // fprintf(stdout, "ADC A0 read %X - %d\n", adc_value, adc_value);
        // fprintf(stdout, "At %d, %.5f\n", count, adc_value_float);
        std::cout << count << " " << adc_value_float*(55.0) << std::endl;
    }

    mraa_aio_close(adc_a0);

    return MRAA_SUCCESS;
}
//! [Interesting]
