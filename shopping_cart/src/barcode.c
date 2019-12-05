/*
 * @file barcode.c
 * @brief This file consists of functions related to Barcode Sensor.
 *
 * @author: Siddhant Jajoo.
 * @date 11/16/2019
 * @copyright Copyright (c) 2019
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "inc/barcode.h"
#include "inc/leuart.h"



static int barcode_payload_size_fetch(struct barcode_packet* barcode_packet)
{
	int payload_size = 0;

	barcode_packet->payload_size[0] = leuart_buffer_pop();
	barcode_packet->payload_size[1] = leuart_buffer_pop();
	barcode_packet->payload_size[2] = leuart_buffer_pop();

	payload_size += (barcode_packet->payload_size[0] - ASCII_DIGIT_START) * 100;
	payload_size += (barcode_packet->payload_size[1] - ASCII_DIGIT_START) * 10;
	payload_size += (barcode_packet->payload_size[2] - ASCII_DIGIT_START) * 1;

	return payload_size;

}

static int barcode_cost_fetch(struct barcode_packet* barcode_packet)
{
	int cost = 0;

	barcode_packet->cost[0] = leuart_buffer_pop();
	barcode_packet->cost[1] = leuart_buffer_pop();
	barcode_packet->cost[2] = leuart_buffer_pop();

	cost += (barcode_packet->cost[0] - ASCII_DIGIT_START) * 100;
	cost += (barcode_packet->cost[1] - ASCII_DIGIT_START) * 10;
	cost += (barcode_packet->cost[2] - ASCII_DIGIT_START) * 1;

	return cost;
}


//Payload_size might need change depending on what needs to be sent to the android application
void barcode_packet_create(struct barcode_packet* barcode_packet, int *payload_size)
{

	if( barcode_packet->payload == NULL && leuart_circbuff.read_index < LEUART_BUFFER_MAXSIZE)
	{
		int local_payload_size, cost;

		barcode_packet->preamble = leuart_buffer_pop();

		//Fetching and converting the character data of Payload size and Cost size here into interger digits.
		local_payload_size = barcode_payload_size_fetch(barcode_packet);
		printf("Payload_size: %d\n", local_payload_size);

		cost = barcode_cost_fetch(barcode_packet);
		printf("Cost: %d\n", cost);

		barcode_packet->payload = malloc(sizeof(char) * local_payload_size);
		if(barcode_packet->payload == NULL)
		{
			printf("ERROR: Cannot Malloc Payload data in barcode_packet_create() function./n");
		}

		*payload_size = local_payload_size;

	}
}

//Output should be 2,0,0,2,39,1,SS,SS where SS is checksum value and varies as per the data packet.
void barcode_test_blocking(void)
{
	const uint8_t cmd[9] = {0x7E, 0x00, 0x07, 0x01, 0x00, 0x2A, 0x02, 0xD8, 0x0F};


	//Disable Interrupts over here in order to support blocking
	if ((LEUART0->IEN & LEUART_IEN_RXDATAV) || (LEUART0->IEN & LEUART_IEN_TXC))
	{
		LEUART_IntDisable(LEUART0, LEUART_IEN_RXDATAV | LEUART_IEN_TXC);
	}


	leuart_send(LEUART0, cmd[0]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[1]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[2]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[3]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[4]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[5]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[6]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[7]);
	printf("Data Sent\n");
	leuart_send(LEUART0, cmd[8]);
	printf("Data Sent\n");


	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));

}


void barcode_test_blocking_scanning(void)
{

	//Disable Interrupts over here in order to support blocking
	if ((LEUART0->IEN & LEUART_IEN_RXDATAV) || (LEUART0->IEN & LEUART_IEN_TXC))
	{
		LEUART_IntDisable(LEUART0, LEUART_IEN_RXDATAV | LEUART_IEN_TXC);
	}


	printf("Testing Barcode UART by scanning the barcode.\n");

	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));
	printf("DATA: %x\n", leuart_rcv(LEUART0));

}

