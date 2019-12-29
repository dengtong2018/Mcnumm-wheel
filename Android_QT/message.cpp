#include "message.h"
#include "stdio.h"
#include <string.h>
#include <QByteArray>

extern BLEInterface *m_bleInterface;

uint16_t CheckSum(uint8_t *buffer ,uint8_t size)
{
    if(buffer == NULL){return 0;}

	uint16_t sum = 0;
	for(uint8_t i = 0 ; i < size ; i++)	{
		sum += *(buffer + i);
	}
	
	return sum;
}

uint16_t CheckSum(QByteArray buffer)
{
    uint16_t sum = 0;
    for(uint8_t i = 0 ; i < buffer.size() ; i++)	{
        sum += buffer[i];
    }

    return sum;
}

Message::Message()
{

}

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

	if(HEADA == buffer[0] && HEADB == buffer[1]){
		uint16_t check_sum = CheckSum(buffer ,size);
		if(check_sum != buffer[3]){
			printf("check fail\n");
			return false;
		}

		HandleMessage(buffer[2] , buffer + 4 ,size);
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

void Message::PackAndSend(uint8_t msg_id ,QByteArray buffer)
{
    QByteArray pack;
    uint16_t sum = 0;
    pack += HEADA;
    pack += HEADB;
    pack += msg_id;
    pack += (uint8_t)buffer.size();
    pack += buffer;
    sum = CheckSum( pack );
    pack += (uint8_t)sum&0xff;
    pack += (uint8_t)(sum >> 8);
    m_bleInterface->write(pack);
}

void Message::HandleMessage(uint8_t msg_id ,uint8_t *buffer ,uint8_t size)
{
	printf("%s\n" , __FUNCTION__);
}

