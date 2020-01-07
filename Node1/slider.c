/** @file slider.c
 *  @brief C-file considering the sliders on the USB-multiboards behaviour. Converting the voltage resolution of 0-255 on the sliders to a percent-representation 0-100.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "slider.h"

#define LEFT_SLIDER_CHANNEL 6
#define RIGHT_SLIDER_CHANNEL 7

#define RESOLUTION 255

/** Function retrieving position of right and left sliders as a percent-value 0-100
 *  @return struct Sliders position - the position of the left and right slider.
 */
Sliders slider_position(void){

    Sliders position;

    // Selecting which channel of the ADC to get output from. Hexadecimal representation of channel wanted from ADC.
    int left_slider = selected_channel_output(LEFT_SLIDER_CHANNEL);
    int right_slider = selected_channel_output(RIGHT_SLIDER_CHANNEL);

    // Output with resolution 0-255
    position.Left = left_slider;
    position.Right = right_slider;

    return position;
}

/** Test function for reading slider position.
 */
void test_slider_position(void){
        //Read Slider position:
        Sliders position = slider_position();

        printf("Left-slider: %i\n\r", position.Left);
        printf("Right-slider: %i\n\r", position.Right);
}
