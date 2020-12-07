#include "blinker.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

void blinkone()
{
	nrf_gpio_pin_set(13);
	nrf_delay_ms(200);
	nrf_gpio_pin_clear(13);
	nrf_delay_ms(200);
}
