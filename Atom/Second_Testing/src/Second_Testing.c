//Second Deliverables Testing Pete&Raj
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <err.h>
#include <stdbool.h>
#include <math.h>
#include "mraa.h"

#define GPIO14 14 //A0
#define GPIO15 15 //A1
#define HIGH 1
#define LOW  0

#define OFFSET 0

bool keepRunning = true;

double pulseIn(mraa_gpio_context, int, double);
void intHandler(int);
double duration(struct timespec, struct timespec);

int main()
{
    double pulsewidth, pulsewidth2;	//pulse width
    float length, length2;		//measured width
    mraa_gpio_context lidar_pin;
    mraa_gpio_context lidar_pin2;
    mraa_gpio_context d_pin;
    mraa_result_print(mraa_init());

    /* Initiate lidar_pin GPIO pin */
    lidar_pin = mraa_gpio_init(GPIO14);
    lidar_pin2 = mraa_gpio_init(GPIO15);
    d_pin = mraa_gpio_init(13);
    if ((lidar_pin == NULL) || (lidar_pin2 == NULL))
        err(1, "mraa_gpio_init returned NULL for GPIO14");
    /* Set lidar_pin GPIO direction */
    mraa_gpio_dir(lidar_pin, MRAA_GPIO_IN);
    mraa_gpio_dir(lidar_pin2, MRAA_GPIO_IN);
    mraa_gpio_dir(d_pin, MRAA_GPIO_OUT);

    /* Listen for Ctrl-C signal */
    if (SIG_ERR == signal(SIGINT, intHandler))
        err(1, "signal returned an error");

    /* Main loop */
    while(keepRunning)
    {

        //count++;
    	sleep(1);
        pulsewidth = pulseIn(lidar_pin, HIGH, 1.0);		//determine duty cycle
        if (pulsewidth > 0)
        {
            length = (pulsewidth * 1E6) / 1000.0;		//convert to meters
            length -= OFFSET;							//subtract offset
            //tempSum = tempSum + length;
            //printf(">>> Length: %f <<<\n", length);		//print measured length in meters
        }

        pulsewidth2 = pulseIn(lidar_pin2, HIGH, 1.0);		//determine duty cycle
		if (pulsewidth2 > 0)
		{
			length2 = (pulsewidth2 * 1E6) / 1000.0;		//convert to meters
			length2 -= OFFSET;							//subtract offset
			//tempSum2 = tempSum2 + length2;
			//printf(">>> Length: %f <<<\n", length);		//print measured length in meters
		}

		float delta = fabs(length-length2);
		//printf(">>>Length 1: %f, Length 2: %f<<<\n",length, length2);
		//printf("%f, %f,%f\n",length, length2,delta);
		//sleep(1);

////OUTPUTTING PWM Outputs to Motors
		mraa_init();
		mraa_pwm_context pwm;
		pwm = mraa_pwm_init(3);
		if (pwm == NULL)
		{
			return 1;
		}
		mraa_pwm_period_ms(pwm, 2.5);
		mraa_pwm_enable(pwm, 1);
		mraa_pwm_write(pwm,0.60f);

////Set Desired Distance to be 0.5m, correct accordingly
		if(length < 0.20 || length2 < 0.20)
		{
			//Reverse
			mraa_pwm_write(pwm,0.4f);
		}

		if(0.20<length<0.65 || 0.20<length2<0.65)
		{
			//stop
			printf("Desired Distance = 0.5m\n");
			mraa_pwm_write(pwm,0.60f);
		}

		if(length > 0.70 || length2 > 0.70)
		{
			//Forward
			mraa_pwm_write(pwm,0.9f);
		}

    }

    /* Clean up */
    mraa_gpio_isr_exit(lidar_pin);
    mraa_gpio_close(lidar_pin);
    mraa_deinit();

    printf("Terminating\n");

    return 0;
}

//returning duration of PWM duty cycle
double pulseIn(mraa_gpio_context pin, int value, double timeout)
{
    struct timespec start, end;		//start and endpoints

    clock_gettime(CLOCK_MONOTONIC, &start);		//start timer
    while(value == LOW ? mraa_gpio_read(pin) == HIGH : mraa_gpio_read(pin) == LOW)	//once pulse goes low or timeout reached
    {
        clock_gettime(CLOCK_MONOTONIC, &end);	//stop timer
        if(duration(start, end) > timeout)		//if duration of pulse > timeout return 0
            return 0.0;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    while (value == mraa_gpio_read(pin));
    clock_gettime(CLOCK_MONOTONIC, &end);

    return duration(start, end);
}

double duration(struct timespec start, struct timespec end)
{
    return (double)((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0);
}

void intHandler(int signalNum)
{
    keepRunning = false;
}
