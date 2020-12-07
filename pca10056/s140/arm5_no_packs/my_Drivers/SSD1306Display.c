
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

void OledSendCommand(ret_code_t(*twitx)(nrf_drv_twi_t,uint8_t,uint8_t[],uint8_t,bool),nrf_drv_twi_t tt,uint8_t _Command)
{
	uint8_t command[2] = {0x00,_Command};
	ret_code_t err_code = twitx(tt, 0x3c, command, sizeof(command),true);
	nrf_delay_ms(10);
}