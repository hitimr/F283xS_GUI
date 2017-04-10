#pragma once

// USB Commands
#define COMMAND_PACKET_RECEIVED 0x01
#define COMMAND_STATUS_UPDATE   0x02
#define COMMAND_PAUSE			0x02
#define COMMAND_CONTINUE		0x03
#define COMMAND_STNDBYMODE		0x04
#define COMMAND_TEST_CON		0x05
#define COMMAND_GPIO_OP			0x06
#define COMMAND_NO_OP			0x07
#define COMMAND_FFLUSH			0x08
#define COMMAND_DEBUG_DATA		0x09
#define COMMAND_SAVE_DATA		0x0A
#define COMMAND_PING			0x0B

#define REQUEST_ALL_DATA		0xF1
#define REQUEST_TEST_DATA		0xF2
#define REQUEST_TEMP_DATA		0xF3
#define NO_COMMAND				0xFF

#define OFF						0x00
#define ON						0x01