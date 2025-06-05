#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define IN 0
#define OUT 1
#define PRESSED 0
#define NOT_PRESSED 1
#define OFF 0
#define ON 1024

#define RESET_BUTTON_PIN 6
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define RED_LED_PIN 13

#define PWM_DIVIDER 16
#define PWM_PERIOD 4096

#define TURN_OFF 0
#define RED 1
#define GREEN 2
#define BLUE 3

bool resetButtonStatus = NOT_PRESSED;

unsigned short int redLedPwmLevel = OFF, greenLedPwmLevel = OFF, blueLedPwmLevel = OFF;
unsigned int redLedSlice, greenLedSlice, blueLedSlice;

void initializeComponents();
void readButtons();
void setLED(char color);

int main()
{
    initializeComponents();

    while (true)
    {
        readButtons();

        if (resetButtonStatus == PRESSED)
        {
            reset_usb_boot(0, 0);
        }
    }
}

void initializeComponents()
{
    stdio_init_all();

    gpio_init(RESET_BUTTON_PIN);
    gpio_set_dir(RESET_BUTTON_PIN, IN);
    gpio_pull_up(RESET_BUTTON_PIN);

    gpio_set_function(RED_LED_PIN, GPIO_FUNC_PWM);
    redLedSlice = pwm_gpio_to_slice_num(RED_LED_PIN);
    pwm_set_clkdiv(redLedSlice, PWM_DIVIDER);
    pwm_set_wrap(redLedSlice, PWM_PERIOD);
    pwm_set_gpio_level(RED_LED_PIN, redLedPwmLevel);
    pwm_set_enabled(redLedSlice, true);
    gpio_set_function(GREEN_LED_PIN, GPIO_FUNC_PWM);
    greenLedSlice = pwm_gpio_to_slice_num(GREEN_LED_PIN);
    pwm_set_clkdiv(greenLedSlice, PWM_DIVIDER);
    pwm_set_wrap(greenLedSlice, PWM_PERIOD);
    pwm_set_gpio_level(GREEN_LED_PIN, greenLedPwmLevel);
    pwm_set_enabled(greenLedSlice, true);
    gpio_set_function(BLUE_LED_PIN, GPIO_FUNC_PWM);
    blueLedSlice = pwm_gpio_to_slice_num(BLUE_LED_PIN);
    pwm_set_clkdiv(blueLedSlice, PWM_DIVIDER);
    pwm_set_wrap(blueLedSlice, PWM_PERIOD);
    pwm_set_gpio_level(BLUE_LED_PIN, blueLedPwmLevel);
    pwm_set_enabled(blueLedSlice, true);
    setLED(TURN_OFF);
}

void readButtons()
{
    resetButtonStatus = gpio_get(RESET_BUTTON_PIN);
}

void setLED(char color)
{
    switch (color)
    {
    case TURN_OFF:
        pwm_set_gpio_level(RED_LED_PIN, OFF);
        pwm_set_gpio_level(GREEN_LED_PIN, OFF);
        pwm_set_gpio_level(BLUE_LED_PIN, OFF);
        break;

    case RED:
        pwm_set_gpio_level(RED_LED_PIN, ON);
        pwm_set_gpio_level(GREEN_LED_PIN, OFF);
        pwm_set_gpio_level(BLUE_LED_PIN, OFF);
        break;

    case GREEN:
        pwm_set_gpio_level(RED_LED_PIN, OFF);
        pwm_set_gpio_level(GREEN_LED_PIN, ON);
        pwm_set_gpio_level(BLUE_LED_PIN, OFF);
        break;

    case BLUE:
        pwm_set_gpio_level(RED_LED_PIN, OFF);
        pwm_set_gpio_level(GREEN_LED_PIN, OFF);
        pwm_set_gpio_level(BLUE_LED_PIN, ON);
        break;

    default:
        pwm_set_gpio_level(RED_LED_PIN, OFF);
        pwm_set_gpio_level(GREEN_LED_PIN, OFF);
        pwm_set_gpio_level(BLUE_LED_PIN, OFF);
        break;
    }
}