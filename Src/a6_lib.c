#include "a6_lib.h"


void USART1_IRQHandler(void);
static void a6_send_cmd(const char *cmd);
static void a6_process_data(void);
static void search_check(char letter);
static void wait_reasponse(char *pt);
static char to_lower(char letter);
static void uart_output_char(char data);
static void cpy_to_uart(void);
static uint8_t a6_startup_test(void);
static uint8_t a6_signal_quality(void);
static uint8_t a6_network_reg(void);
static uint8_t a6_module_name(void);
static void flush_buffer(void);



char sender_number[PHONE_NUM_LEN] = {'\0'};
char raw_msg[RAW_SMS_LEN]		  = {'\0'};
char sms_msg[SMS_MSG_LEG]	 	  = {'\0'};

#define SR_TXE		(0x1 << 7)
#define SR_RXNE		(0x1 << 5)

#define MAX_NUM_OF_TRY 		10
#define TEMP_BUFF_SIZE		300
int itr = 0;

char temp_buffer[TEMP_BUFF_SIZE];



char sub_str[32];
volatile bool searching 	= false;
volatile bool is_response 	= false;
volatile int search_idx 	=  0;

void a6_module_init(void)
{
	//Enable fifios
	tx_fifo_init();
	rx_fifo_init();

	//Enable a6 UART
	a6uart_init();
	//Enable debug uart
	debug_uart_init();
	//Initialize the flags
	searching = false;
	is_response = false;

	printf("A6 Initialization...\n\r");

	//Enable interrupt
	NVIC_EnableIRQ(USART1_IRQn);

	//Run test command
	a6_startup_test();

	//Check signal quality
	a6_signal_quality();

	//Check network registration status
	a6_network_reg();

	//Get module name
	a6_module_name();


}


uint8_t a6_set_sms_format_to_text(void)
{
    uint8_t num_of_try = MAX_NUM_OF_TRY;
    wait_reasponse("ok\r\n");

    while(num_of_try)
    {
        a6_send_cmd("AT+CMGF=1\r\n");
        systick_delay_ms(3000);

        if(is_response)
        {
            /* Success */
            printf("Format set successful......\n\r");
            flush_buffer();

            return 1;
        }
        num_of_try--;
    }

    printf("Format set: Test failure......\n\r");
    return 0;
}

//Send command to a6
void a6_send_msg(char *msg, char* phone_number)
{
	uint8_t num_of_try = MAX_NUM_OF_TRY;
	wait_reasponse("ok\r\n");

	while(num_of_try)
	{
		char SMS_AT_CMD1[] = "AT+CMGF=1\r\n"; //select to text mode
		char SMS_AT_CMD2[21] = "AT+CMGS=+381";
		char CR[] = "\r\n";

		uint8_t MSG_END[] = {26}; //why 26?

		strcat(SMS_AT_CMD2,phone_number);
		strcat(SMS_AT_CMD2,CR);

		//Set message format to text
		a6_send_cmd(SMS_AT_CMD1);
		systick_delay_ms(750);

		//Send phone number
		a6_send_cmd(SMS_AT_CMD2);
		systick_delay_ms(750);

		//Send the sms message
		a6_send_cmd(msg);
		systick_delay_ms(750);

		//Send message end
		a6_send_cmd((char*)MSG_END); // why did we type cast it to char*

		if(is_response)
		{
			printf("SMS sent...\n\r");
            flush_buffer();
			return 1;
		}

		num_of_try--;

	}

	printf("Unable to send SMS...\n\r");
	return 0;


}




static uint8_t a6_startup_test(void)
{
    uint8_t num_of_try = MAX_NUM_OF_TRY;
    wait_reasponse("ok\r\n");

    while(num_of_try)
    {
        a6_send_cmd("AT+COPN=?\r\n");
        systick_delay_ms(3000);

        if(is_response)
        {
            /* Success */
            printf("Test was successful......\n\r");
            flush_buffer();
            return 1;
        }
        num_of_try--;
    }

    printf("ERR : Test failure......\n\r");
    return 0;
}

static uint8_t a6_signal_quality(void)
{
    uint8_t num_of_try = MAX_NUM_OF_TRY;
    wait_reasponse("ok\r\n");

    while(num_of_try)
    {
        a6_send_cmd("AT+CSQ=?\r\n");
        systick_delay_ms(3000);

        if(is_response)
        {
            /* Success */
            printf("Signal check successful......\n\r");
            flush_buffer();
            return 1;
        }
        num_of_try--;
    }

    printf("Signal check: Test failure......\n\r");
    return 0;
}

//function that tell us if there is a new message

uint8_t a6_is_sms(void)
{
	char *ptr = NULL;

	ptr = strstr(temp_buffer,"MESSAGE");
	if(ptr == NULL)
	{
		//No message
		return 0;
	}
	else
	{
		return 1;
	}

}

/*
 * C:MEV> "MESSAGE".1

CMT: "+381611455918".."2026/2/15,10:10:31+00"
Hello there
 */


void get_raw_sms(void)
{
	char *ptr = NULL;

	ptr = strstr(temp_buffer,"+CMT");
	//we get to
	if(NULL != ptr)
	{
		for(int j = 0; j < RAW_SMS_LEN ; j++)
		{
			raw_msg[j] = ptr[j];
		}
	}
}


void get_sms_msg(void)
{
	char *ptr = NULL;
	ptr = strstr(raw_msg,"\"20");

	if(ptr != NULL)
	{

		for(int j = 0; j < SMS_MSG_LEG ; j++)
		{
			sms_msg[j] = ptr[j];
		}
	}

}

void get_sender_number(void)
{
	char *ptr = NULL;
	ptr = strstr(raw_msg,"+381");

	if(ptr != NULL)
	{

		for(int j = 0; j < PHONE_NUM_LEN ; j++)
		{
			sender_number[j] = ptr[j];
		}
	}

}


static uint8_t a6_network_reg(void)
{
    uint8_t num_of_try = MAX_NUM_OF_TRY;
    wait_reasponse("ok\r\n");

    while(num_of_try)
    {
        a6_send_cmd("AT+CSQ=?\r\n");
        systick_delay_ms(3000);

        if(is_response)
        {
            /* Success */
            printf("Network registration status retrieved......\n\r");
            flush_buffer();
            return 1;
        }
        num_of_try--;
    }

    printf("ERR: Network registration status retrieved......\n\r");
    return 0;
}

static uint8_t a6_module_name(void)
{
    uint8_t num_of_try = MAX_NUM_OF_TRY;
    wait_reasponse("ok\r\n");

    while(num_of_try)
    {
        a6_send_cmd("ATI\r\n");
        systick_delay_ms(3000);

        if(is_response)
        {
            /* Success */
            printf("Module name retrieved successfully......\n\r");
            flush_buffer();
            return 1;
        }
        num_of_try--;
    }

    printf("ERR: Module name retrieved error......\n\r");
    return 0;
}


//how to transfer data between fifo and uart

//Copy content of tx_fifo into debug uart
static void cpy_to_uart(void)
{
	char letter;

	//Transmit data register is empty and
	//fifo has content

	while((USART2 -> SR & SR_TXE) && tx_fifo_size() > 0)
	{
		//Get char from fifo
		tx_fifo_get(&letter);

		//Put into uart data register
		USART2->DR = letter;
	}
}

//Output uart character
static void uart_output_char(char data)
{
	if(tx_fifo_put(data) == FIFOFAIL)
	{
		return;

	}

	cpy_to_uart();
}

static char to_lower(char letter)
{
	if((letter >= 'A') &&(letter <= 'Z'))
	{
		letter |= 0x20;
	}
	return letter;

}

//Initialize the string search in the RX data stream

static void wait_reasponse(char *pt)
{
	strcpy(sub_str,pt);
	search_idx   = 0;
	is_response  = false;
	searching	 = true;
}

//Search for string in rx data stream

static void search_check(char letter)
{
	if(searching) //this means that wait_reasponse
	{
		if(sub_str[search_idx] == to_lower(letter))
		{
			search_idx++;
			//Chech if end of string
			if(sub_str[search_idx] == 0)
			{
				is_response = true;
				searching = false;
			}
		}
		else
		{
			//Start over
			search_idx = 0;
		}
	}
}

static void flush_buffer(void)
{
	itr = 0;

	for(int i=0; i<TEMP_BUFF_SIZE; i++)
	{
		temp_buffer[i] = '\0';
	}
}


static void a6_process_data(void)
{
	char letter;
	//Check if there is new data in data register

	if(USART1->SR & SR_RXNE)
	{

		//Store data from a6 uart data regoster to local variable
		letter = USART1->DR;
		temp_buffer[itr++] = letter;

		//Print data from a6 uart data register to debug uart1
		uart_output_char(letter);

		//Check for response
		search_check(letter);
	}
}

static void a6_send_cmd(const char * cmd)
{
	int index = 0;

	while(cmd[index] != 0)
	{
		a6_uart_write_char(cmd[index++]);
	}
}




//"listening" for incoming data
void USART1_IRQHandler(void)
{
	a6_process_data();

}
