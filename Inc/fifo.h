#ifndef __FIFO_H__
#define __FIFO_H__
#include <stdint.h>

#define TXFIFOSIZE 		1024
#define RXFIFOSIZE 		1024
#define TXFAIL 			0
#define RXFAIL			0
#define TXSUCCESS		1
#define RXSUCCESS		1
#define FIFOFAIL 		0

typedef char tx_data_Type;
typedef char rx_data_Type;

void tx_fifo_init(void);
uint8_t tx_fifo_put(tx_data_Type data);
uint8_t tx_fifo_get(tx_data_Type *pdata);
uint8_t tx_fifo_size(void);
void rx_fifo_init(void);
uint8_t rx_fifo_put(rx_data_Type data);
uint8_t rx_fifo_get(rx_data_Type *pdata);
uint8_t rx_fifo_size(void);




#endif
