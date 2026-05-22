#include "rtc.h"



#define PWRRST			(0x01 <<28)
#define CR_DBP			(0x01 << 8)
#define CSR_LSION		(0x01 << 0)
#define CSR_LSIRDY		(0x01 << 1)
#define CR_BDRST		(0x01 << 16)
#define CR_RTCEN		(0x01 << 15)
#define CR_FMT			(0x01 << 6)

#define RTC_WRITE_PROTECTION_ENABLE_1 ((uint8_t)0xCAU)
#define RTC_WRITE_PROTECTION_ENABLE_2 ((uint8_t)0x53U)

#define RTC_INIT_MASK	0xFFFFFFFFU

#define RTC_ASYNCH_PREDIV ((uint32_t)0x7F)
#define RTC_SYNCH_PREDIV ((uint32_t)0x00F9)

static uint32_t	rtc_init_seq(void);
static void rtc_set_async_prescaler(uint32_t AsyncPrescaler);
static void rtc_set_sync_prescaler(uint32_t SynchPrescaler);
static uint32_t exit_init_seq(void);




void rtc_init(void)
{
	/////////DOCUMENTATION PART/////////


	//Enable clock access to PWR
	RCC -> APB1ENR |= PWRRST;

	//Enable Backup access to config registers
	PWR->CR |= CR_DBP;

	//Enable low speed internal clock LSI
	RCC->CSR |= CSR_LSION;

	//Wait for LSI to be enabled
	while((RCC->CSR & CSR_LSIRDY) != CSR_LSIRDY){};

	//Force the back up domain to reset
	RCC->BDCR |= CR_BDRST;

	//Relase backup domain reset
	RCC->BDCR &= ~CR_BDRST;

	//Set RTC clock source to LSI

	RCC->BDCR &= ~(0x01 << 8);
	RCC->BDCR |= (0x01 << 9);

	// Enable RTC
	RCC->BDCR |= CR_RTCEN;

	//Disable RTC register write protection
	RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_1;
	RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_2;

	//Enter initilazation mode
	if(rtc_init_seq() != 1)
	{
		//Initilization error
		printf("Initilization error ");
	}

	//Set hour format to AM/PM
	RTC->CR |= CR_FMT;
	//Set Asynch prescaler
	rtc_set_async_prescaler(RTC_ASYNCH_PREDIV);

	//Set Synch presclaer
	rtc_set_sync_prescaler(RTC_SYNCH_PREDIV);


	//Exit initalization mode
	exit_init_seq();
	//Enable RTC register write protection
	RTC->WPR = 0xFF;
}


static void rtc_set_async_prescaler(uint32_t AsyncPrescaler)
{
	MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_A, AsyncPrescaler << RTC_PRER_PREDIV_A_Pos);
}

static void rtc_set_sync_prescaler(uint32_t SynchPrescaler)
{
	MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_S, SynchPrescaler);
}


static void rtc_enable_init_mode()
{
	RTC->ISR = RTC_INIT_MASK; //why did we mask them all, and whz

}

static uint32_t rtc_isActiveflag_init(void)
{
	return (READ_BIT(RTC->ISR,RTC_ISR_INITF) == (RTC_ISR_INITF));

}

static uint32_t	rtc_init_seq(void)
{
	//Set initilazation mode
	rtc_enable_init_mode();

	//Check if initalization mode is set
	while(rtc_isActiveflag_init() != 1){}

	return 1;
}

static void rtc_exit_init_mode(void)
{
	RTC->ISR &= ~(RTC_INIT_MASK);
}

void rtc_clearflag_rs(void)
{
	    WRITE_REG(RTC->ISR, (~((RTC_ISR_RSF | RTC_ISR_INIT) & 0x00000FFFFU) | (RTC->ISR & RTC_ISR_INIT)));
}

static uint32_t rtc_isActiveflag_rs(void)
{
	return (READ_BIT(RTC->ISR, RTC_ISR_RSF) == (RTC_ISR_RSF));
}

static uint32_t wait_for_synchro(void)
{
	//Cleare RSF flag
	rtc_clearflag_rs();
	//Wait for register to synched
	while((rtc_isActiveflag_rs() != 1))
	{

	}
	return 0;
}

static uint32_t exit_init_seq(void)
{
	//Disable RTC init mode
	rtc_exit_init_mode();
	//Wait for register to be synchronized
	return (wait_for_synchro());

}








