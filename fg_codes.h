/*******************************************************************
*
*  DESCRIPTION: Defines for error and command codes in the frame grabber
*               composite model.
*
*  AUTHOR: Brian Webb
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 18/10/2007
*
*******************************************************************/

#ifndef __FRAME_GRABBER_COMS_CODES_H
#define __FRAME_GRABBER_COMS_CODES_H

// Commands are always > 0
// Errors are always < 0
// Hence:
#define NONE 0.0

#define EVENT_DATA_SHIFT 4    // all other data in a command is 4 hex digits wide
#define EVENT_DATA_MASK 0xF

#define MSEC 1000

// Broad command types

#define COMMAND_FOR_HSI	0x1
#define COMMAND_FOR_IA	0x2
#define COMMAND_FOR_BUF	0x3
#define COMMAND_FOR_TIM	0x4

// HSI commands

#define HSI_SET_GRAB_TYPE	0x1

// HSI grab types

#define HSI_GRAB_ONBOARD				0x1
#define HSI_GRAB_OFFBOARD_BUFFERED		0x2
#define HSI_GRAB_OFFBOARD_UNBUFFERED	0x3
#define HSI_GRAB_LAST_MODE_NUM          0x3 // always equals the one above it

// Image Acquisition commands

#define IA_SET_SIZE        0x1
#define IA_GRAB_SINGLE     0x2
#define IA_GRAB_CONTINUOUS 0x3
#define IA_GRAB_ABORT      0x4

// IA responses
#define IA_GRAB_ABORTED    0x1

// Buffer commands

#define BUF_CREATE_BUFFER	0x1
#define BUF_SET_INDEX		0x2
#define BUF_STORE_IMAGE		0x3
#define BUF_RETRIEVE_IMAGE	0x4

// Timer Commands
#define TIM_SET_ACTIVE				0x1
#define TIM_SET_DELAY				0x2
#define TIM_SET_INACTIVE			0x3
#define TIM_START_TIMER				0x4
#define TIM_STOP_TIMER				0x5
#define TIM_START_PERIODIC_TIMER	0x6

// Timer modes
#define TIM_MODE_OFF		0x1
#define TIM_MODE_TRIGGERED	0x2
#define TIM_MODE_PERIODIC	0x3

// Error codes
#define ERROR_UNKNOWN_COMMAND				        -1
#define ERROR_NO_GRAB_IN_PROGRESS			        -2
#define ERROR_NOT_ENOUGH_FREE_SPACE		   	        -3
#define ERROR_INDEX_OUT_OF_RANGE			        -4
#define ERROR_IMAGE_BUFFER_SIZE_MISMATCH            -5
#define ERROR_IMAGE_SIZE_DIFFERENT_THAN_EXPECTED    -6
#define ERROR_NO_BUFFERS_DEFINED                    -7
#define ERROR_TIMER_NOT_SET							-8
#define ERROR_TIMER_DELAY_OR_INACTIVE_NOT_SET		-9

#endif // __FRAME_GRABBER_COMS_CODES_H