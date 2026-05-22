#ifndef __A6_LIB_H__
#define __A6_LIB_H__
#include "a6_driver.h"
#include "fifo.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define		PHONE_NUM_LEN 		13
#define 	RAW_SMS_LEN			200
#define 	SMS_MSG_LEG			70


void a6_module_init(void);
void a6_send_msg(char *msg, char* phone_number);
uint8_t a6_set_sms_format_to_text(void);
void get_raw_sms(void);
uint8_t a6_is_sms(void);
void get_sender_number(void);
void get_raw_sms(void);
void get_sms_msg(void);


#endif


