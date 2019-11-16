/*
 * @file leuart.c
 * @brief This file consists of functions related to LEUART peripheral.
 *
 * @author: Siddhant Jajoo.
 * @date 11/05/2019
 * @copyright Copyright (c) 2019
 *
 */

#include <stdio.h>
#include "em_core.h"
#include "em_cmu.h"
#include "inc/leuart.h"
#include "inc/connection_param.h"
#include "inc/external_events.h"


/**
 * @brief Interrupt handler for LEUART
 * @param void
 * @return void
 */
void LEUART0_IRQHandler(void)
{
	//Disable All Interrupts
	CORE_AtomicDisableIrq();

	//Acknowledge and Clear the Interrupt
	uint32_t flags = LEUART_IntGet(LEUART0);
	LEUART_IntClear(LEUART0, LEUART_IF_TXC);


	//Update the External Event
	external_event |= LEUART_EVENT;
	gecko_external_signal(external_event);

	// RX portion of the interrupt handler
//	if (flags & LEUART_IF_RXDATAV)
//	{
//		//	  while (LEUART0->STATUS & LEUART_STATUS_RXDATAV)
//		//    { // While there is still incoming data
//		//      //char data = LEUART_Rx(LEUART0);
//		//    }
//		printf("Received Data");
//		//  }
//	}
	// TX portion of the interrupt handler
	if (flags & LEUART_IF_TXC)
	{
		//printf("Data Transmitted\n");
	}

	//Enable All Interrupts
	CORE_AtomicEnableIrq();
}



static void leuart_gpio_init(void)
{
	// GPIO clock
	CMU_ClockEnable(cmuClock_GPIO, true);

	// Initialize LEUART0 TX and RX pins
	GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1); 	// TX (Pin Number 7)
	GPIO_PinModeSet(gpioPortD, 11, gpioModeInputPull, 1);   // RX (Pin Number 9)
}


/**
 * @brief Initialization function for LEUART. This function enables the required clock and GPIO peripherals.
 * @param void
 * @return void
 */
void leuart_init(void)
{
	//Enabling GPIO required for LEUART.
	leuart_gpio_init();

	// Enable clocks for LEUART0
	CMU_ClockEnable(cmuClock_LEUART0, true);
	CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1); // Don't prescale LEUART clock

	// Initialize the LEUART0 module
	LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;
	LEUART_Init(LEUART0, &init);

	// Enable LEUART0 RX/TX pins
	LEUART0->ROUTEPEN |= LEUART_ROUTEPEN_RXPEN | LEUART_ROUTEPEN_TXPEN;

	LEUART0->ROUTELOC0 |= (LEUART0->ROUTELOC0 & (~_LEUART_ROUTELOC0_TXLOC_MASK)) | LEUART_ROUTELOC0_TXLOC_LOC18;
	LEUART0->ROUTELOC0 |= (LEUART0->ROUTELOC0 & (~_LEUART_ROUTELOC0_RXLOC_MASK)) | LEUART_ROUTELOC0_RXLOC_LOC18;

	// Enable LEUART0 RX/TX interrupts
//	LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV | LEUART_IEN_TXC);
	//LEUART_IntEnable(LEUART0, LEUART_IEN_TXC);
	//NVIC_EnableIRQ(LEUART0_IRQn);

	//for (int i = 0; i < 100000; i++);
}


void leuart_send(LEUART_TypeDef *leuart, uint8_t data)
{
	uint8_t tx_data = data;
	LEUART_Tx(leuart, tx_data);
}

char leuart_rcv(LEUART_TypeDef *leuart)
{
	return LEUART_Rx(leuart);
}


void leuart_loopback_test_blocking(void)
{
	const uint8_t cmd[9] = {0x7E, 0x00, 0x07, 0x01, 0x00, 0x2A, 0x02, 0xD8, 0x0F};

	//TODO: Disable Interrupts over here

	leuart_send(LEUART0, cmd[0]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[1]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[2]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[3]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[4]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[5]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[6]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[7]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

	leuart_send(LEUART0, cmd[8]);
	printf("Data Sent\n");
	printf("DATA: %x\n", leuart_rcv(LEUART0));

}

