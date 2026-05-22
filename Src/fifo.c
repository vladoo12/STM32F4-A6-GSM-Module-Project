#include "fifo.h"

tx_data_Type static TX_FIFO[TXFIFOSIZE];
rx_data_Type static RX_FIFO[RXFIFOSIZE];

volatile uint32_t tx_put_itr;
volatile uint32_t rx_put_itr;
volatile uint32_t tx_get_itr;
volatile uint32_t rx_get_itr;

//Initialize tx fifo

void tx_fifo_init(void)
{
	tx_put_itr = 0;
	tx_get_itr = 0;

}

//Put data into tx FIFO

uint8_t tx_fifo_put(tx_data_Type data)
{
	//Check if fifo si full
	if((tx_put_itr - tx_get_itr) & ~(TXFIFOSIZE - 1))
	{
		//FIFO is full
		return (TXFAIL);
	}

	//Put data into fifo
	TX_FIFO[tx_put_itr & (TXFIFOSIZE - 1)] = data;

	//Increment itr
	tx_put_itr++;

	return(TXSUCCESS);
}

uint8_t tx_fifo_get(tx_data_Type *pData)
{
	//Check if FIFO is empty
	if(tx_put_itr == tx_get_itr)
	{
		//fifo is empty
		return(TXFAIL);
	}
	//Get the data
	*pData = TX_FIFO[tx_get_itr & (TXFIFOSIZE-1)];

	//Increment the itr
	tx_get_itr ++;

	return(TXSUCCESS);
}


uint8_t tx_fifo_size(void)
{

	return(uint32_t)(tx_put_itr - tx_get_itr);

}


//Initialize rx fifo

void rx_fifo_init(void)
{
	rx_put_itr = 0;
	rx_get_itr = 0;

}
//Put data into rx FIFO

uint8_t rx_fifo_put(rx_data_Type data)
{
	//Check if fifo si full
	if((rx_put_itr - rx_get_itr) & ~(RXFIFOSIZE - 1))
	{
		//FIFO is full
		return (RXFAIL);
	}

	//Put data into fifo
	RX_FIFO[rx_put_itr & (RXFIFOSIZE - 1)] = data;

	//Increment itr
	rx_put_itr++;

	return(RXSUCCESS);
}


uint8_t rx_fifo_get(rx_data_Type *pdata)
{
	//Check if FIFO is empty
	if(rx_put_itr == rx_get_itr)
	{
		//fifo is empty
		return(RXFAIL);
	}
	//Get the data
	*pdata = RX_FIFO[rx_get_itr & (RXFIFOSIZE-1)];

	//Increment the itr
	rx_get_itr ++;
	return(RXSUCCESS);

}


uint8_t rx_fifo_size(void)
{

	return(uint32_t)(rx_put_itr - rx_get_itr);

}

