#pragma once

// Required by Driver
#define COMMAND_PACKET_RECEIVED 0x01
#define COMMAND_STATUS_UPDATE   0x02

// Commands execute a non-periodic subroutine
#define COMMAND_SYSTEM_PAUSE	0x02
#define COMMAND_SYSTEM_CONTINUE	0x03
#define COMMAND_STNDBYMODE		0x04	// Retracted
#define COMMAND_TEST_CON		0x05	// Retracted
#define COMMAND_GPIO_OP			0x06
#define COMMAND_NO_OP			0x07
#define COMMAND_FFLUSH			0x08
#define COMMAND_DEBUG_DATA		0x09
#define COMMAND_SAVE_DATA		0x0A
#define COMMAND_PING			0x0B
#define COMMAND_RECORD_HW		0x0C

#define NO_COMMAND				0xFF

// Pull Saved Data from RAM
#define REQUEST_DATA_X			0xF1
#define REQUEST_DATA_Y			0xF2
#define REQUEST_DATA_Z			0xF3


// Settings define Parameters for the Workflow
#define SETTING_SET				0x40
#define SETTING_GET				0x41
#define SETTING_SMPL_BUF_SIZE	0x50
#define SETTING_USB_BUF_SIZE	0x51
#define SETTING_DEBUG_MODE		0x52
#define SETTING_FLASH_MODE		0x53
#define SETTING_CLK_DIV			0x54
#define SETTING_SPI_FAST_BRR	0x55
#define SETTING_SPI_SLOW_BRR	0x56
#define SETTING_CNV_PERIOD		0x57
#define SETTING_CNV_MULT		0x58
#define SETTING_CNV_NUM			0x59
#define SETTING_XMIT_PERIOD		0x5A
#define SETTING_SYSTEM_STATUS	0x5B

// Convenience Stuff
#define OFF						0x00
#define ON						0x01