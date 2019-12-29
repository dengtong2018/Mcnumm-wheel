#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>
#include <stdio.h>
#include "bleinterface.h"

#define HEADA 0xAA
#define HEADB 0xBB
#define MIN_PACK_SIZE 6
typedef struct{
	uint8_t head_a;
	uint8_t head_b;
	uint8_t msg_id;
	uint8_t msg_size;
	uint8_t buffer[128];
	uint16_t sum;
}Protocol;

typedef enum{
	MSGID_LEFT_JOYSTICK,
    MSGID_RIGHT_JOYSTICK,
    MSGID_MAX
}MsgId; 

typedef struct{
	int8_t move_forward_back;
	int8_t move_left_right;
}LinerSpeedJoystick;

typedef struct{
	int8_t rotate;
	int8_t reserve;
}AngularSpeedJoystick;

class Message
{
public:
    Message();
	
	bool Parse(uint8_t *buffer , uint8_t size);
	void PackAndSend(uint8_t msg_id ,uint8_t *buffer ,uint8_t size);
    void PackAndSend(uint8_t msg_id ,QByteArray buffer);
	void HandleMessage(uint8_t msg_id ,uint8_t *buffer ,uint8_t size);
};

#endif 
