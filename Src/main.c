#include <stdint.h>
#include "hardware_modules.h"

#include "a6_lib.h"

bool btn_state;

uint32_t sensor_value;
int main(void)
{

	light_init();
	button_init();
	start_conversion();
	while(1)
	{
		sensor_value = adc_read();

	}

}
