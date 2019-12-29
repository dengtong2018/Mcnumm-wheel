#include "message.hpp"
#include "stdio.h"
#include <string.h>

uint16_t CheckSum(uint8_t *buffer ,uint8_t size)
{
	if(buffer == NULL){return 0;}

	uint16_t sum = 0;
	for(uint8_t i = 0 ; i < size ; i++)	{
		sum += *(buffer + i);
	}
	
	return sum;
}

Message::Message(){}	;


bool Message::Parse(uint8_t *buffer , uint8_t size)
{
	if(size < MIN_PACK_SIZE)	{
		printf("pack too llittle\n");
		return false;
	}

	if(size < buffer[3]){
		printf("pack error\n");
		return buffer;
	}

	for(uint8_t i = 0 ; i < size ; i++){
		//printf("%d  " , *(buffer + i));
	}
	//printf("\n");
	
	if(HEADA == buffer[0] && HEADB == buffer[1]){
		uint8_t data_size = buffer[3];
		uint16_t check_sum = CheckSum(buffer ,data_size + 4);
		//printf("size : %u\n" , data_size);
		if(check_sum != buffer[4 + data_size]  + (buffer[5 + data_size] << 8)){
			//printf("check fail:%u %u %u %u\n" , check_sum , buffer[4 + data_size]  + (uint16_t)(buffer[5 + data_size]<< 8)  ,buffer[4 + data_size] ,(uint16_t)(buffer[5 + data_size]) << 8);
			return false;
		}

		HandleMessage(buffer[2] , buffer + 4 , data_size);
		return true;
	}
	
	return false;
}


void Message::PackAndSend(uint8_t msg_id ,uint8_t *buffer ,uint8_t size)
{
	Protocol protocol;
	protocol.head_a = HEADA;
	protocol.head_b = HEADB;
	protocol.msg_id = msg_id;
	protocol.msg_size = size;
	memcpy(protocol.buffer ,buffer ,size);
	protocol.sum = CheckSum( (uint8_t *)&(protocol) , size + MIN_PACK_SIZE - 2);
}

