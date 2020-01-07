/** @file PWM.c
 *  @brief c-file for the PWM
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "PWM.h"

/** Function for initializing PWM on the ATmega2560.
 */
void PWM_init(void) {
    // Set mode to Fast PWM (mode 14)
    set_bit(TCCR1A, WGM11);

    set_bit(TCCR1B, WGM12);
    set_bit(TCCR1B, WGM13);

    set_bit(TCCR1A, COM1A1);

    // Set clock frequency
    set_bit(TCCR1B, CS11);

    // Top ~ 40 000
    ICR1 = (uint16_t)((F_CLK/(N * F_PWM)) - 1);

    uint16_t BOTTOM = (uint16_t)(1.5/PWM_T * ((F_CLK/(N * F_PWM)) - 1));

    // Used as PWM output
    // Output Compare Register
    OCR1A = BOTTOM;

    // Set output servo pin, PB5 on ATmega2560, pin 11 on Arduino shield
    set_bit(DDRB, PB5);

    printf("PWM initialized. \n\r");
}

/** Function for converting joystick position (-100 to 100) to a pulse width (1.0 - 2.0 ms) and finding the duty cycle (PW/T)
 *  @param message position - CAN message containing the position of the joystick.
 *  @return double PWM_D - duty cycle.
 */
double PWM_joystick_to_duty_cycle(message position){

    int8_t x_position = position.data[0];

    // Joystick position is values between (-100 - 100), negative values gets lost in CAN transmission as an uint8_t is transmitted
    if (position.data[0] > 100) {
        x_position = position.data[0] - 256;
    }

    double PWM_resolution = 1.000; // From 1 ms to 2 ms - duty cycle for the PWM signal.
    double PWM_PW = 0.000;
    double PWM_D = 0.000;

    double ratio = 0.000;

    if (x_position < 0.000) {
        ratio = (x_position + 100.000)/100.000;
        PWM_PW = (PWM_resolution/2.000 * ratio) + 1.000;
    }

    else {
        ratio = x_position/100.000;
        PWM_PW = (PWM_resolution/2.000 * ratio) + 1.500;
    }

    // Calculating duty cycle
    PWM_D = PWM_PW/PWM_T;
    return PWM_D;
}

/** Function for setting output compare register in the ATmega2560 to wanted duty cycle.
 *  @param double duty_cycle - the duty cycle to be set.
 */
void PWM_set_duty_cycle(double duty_cycle) {

    // Checking that signal doesn't exceed limitations of servo
    if ((duty_cycle <= MAX_DEFLECTION_ANGLE_LEFT/PWM_T) || (duty_cycle >= MAX_DEFLECTION_ANGLE_RIGHT/PWM_T)) {
        printf("Invalid duty cycle. Signal exceeds limitations of servo.");
    }

    // Set OCR1A (PWM output) to correspond to new duty cycle
    else {
        OCR1A = (uint16_t)(duty_cycle * ((F_CLK/(N * F_PWM)) - 1));
    }
}

/** Function for testing that the joystick movement actually moves the servo.
 */
void test_joystick_to_servo(void){
    USART_init(9600);
    PWM_init();
    CAN_init();

    while(1){
        message position = CAN_data_receive();
        double duty_cycle = PWM_joystick_to_duty_cycle(position);

        PWM_set_duty_cycle(duty_cycle);
        _delay_ms(100);
    }
}
