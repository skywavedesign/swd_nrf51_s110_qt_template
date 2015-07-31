#ifndef __CUSTOM_BOARD_H__
#define __CUSTOM_BOARD_H__
//------------------------------------------------------------------------------
// This header file describes the board configuration.  We do not use the board
// support package because generally speaking when creating a new project you
// want to create custom code for interacting with your particular board and
// even though this template project uses the PCA100028 board we ultimately
// want to clone this code for a custom board so the project is geared toward
// that goal.
//
//
//------------------------------------------------------------------------------
// LED Pin
#if BOARD_REV == 1
#define LED1_PIN 21
#define LED2_PIN 22
#define LED3_PIN 23
#define LED4_PIN 24

#define BUTTON1_PIN 17
#define BUTTON2_PIN 18
#define BUTTON3_PIN 19
#define BUTTON4_PIN 20

#elif BOARD_REV == 2
#elif BOARD_REV == 3
#endif
#define BM(X) (1L << (X))
#endif // __CUSTOM_BOARD_H__
