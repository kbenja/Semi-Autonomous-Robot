#include "mraa.h"

#include <stdio.h>
#include <unistd.h>

int main()
{
	mraa_aio_context adc = mraa_aio_init(0);   //pin A0 for analog in
	uint16_t adc_read;      //uint read value
	float adc_read_f;       //float read value

	//set precision to 12 bits (default is 10)
	mraa_result_t adc_acc = mraa_aio_set_bit(adc, 12);
	if (adc_acc != MRAA_SUCCESS) {
	    printf("Error!: %d\n", adc_acc);
	    return 1;
	}
	int i;
	for (i = 0; i < 60; i++) {
	    adc_read = mraa_aio_read(adc);      //read from A0
	    adc_read_f = mraa_aio_read_float(adc);
	    printf("%d,%d,%5f\n",i,adc_read,adc_read_f);
	    usleep(500000);     //sample every 0.5s
	}

	return MRAA_SUCCESS;
}
