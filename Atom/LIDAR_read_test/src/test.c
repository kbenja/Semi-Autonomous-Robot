/*
 * lidarLite_PWM_read.c will read the PWM output of PulsedLight's LidarLite.
 *
 * Copyright (C) 2015 Martin Jaime
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See <http://www.gnu.org/licenses/> for a copy of the GNU General Public
 * License.
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <err.h>
#include <stdbool.h>

#include "mraa.h"

#define GPIO14 14


#define HIGH 1
#define LOW  0

/*
 * Set offset to improve accuracy.
 * This value will be subtracted from the calculated length.
 * During testing, 0.16 was the value required.
 */
#define OFFSET 0

bool keepRunning = true;

double pulseIn(mraa_gpio_context, int, double);
void intHandler(int);
double duration(struct timespec, struct timespec);

int main()
{

    double pulsewidth;	//pulse width
    float length;		//measured width
    mraa_gpio_context lidar_pin;
    mraa_gpio_context power_en_pin;

    mraa_result_print(mraa_init());

    /* Initiate lidar_pin GPIO pin */
    lidar_pin = mraa_gpio_init(GPIO14);
    if (lidar_pin == NULL)
        err(1, "mraa_gpio_init returned NULL for GPIO14");
    /* Set lidar_pin GPIO direction */
    mraa_gpio_dir(lidar_pin, MRAA_GPIO_IN);

    /* Listen for Ctrl-C signal */
    if (SIG_ERR == signal(SIGINT, intHandler))
        err(1, "signal returned an error");

    /* Main loop */
    while(keepRunning)
    {
        sleep(1);

        pulsewidth = pulseIn(lidar_pin, HIGH, 1.0);		//determine duty cycle
        if (pulsewidth > 0)
        {
            length = (pulsewidth * 1E6) / 1000.0;		//convert to meters
            length -= OFFSET;							//subtract offset
            printf(">>> Length: %f <<<\n", length);		//print measured length in meters
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
