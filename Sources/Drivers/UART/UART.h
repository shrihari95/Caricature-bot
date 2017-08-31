/*
* UART.h
* This header contains all Peripheral Driver Declarations.
*
* Author		:	Piyush Sharm
* CO-Aouth		:	____________
* Date Created	:	11/11/2016
*
*/

#ifndef _UART_H
#define _UART_H

#include "../base.h"

// Constants & Definitions
#define XON  0x11
#define XOFF 0x13
#define LF   0x0A
#define CR   0x0D

#define RX_BUF_SIZE 80
#define TX_BUF_SIZE 80

// UART Recieve Buffer
extern char rbuf[RX_BUF_SIZE];
extern int rin;
extern int rout;
extern unsigned char lineReceived;

// Function Definitions
void UART_init();

void UART_outChar(char x);
char UART_inChar(void);
void UART_print(char* str);
void UART_println(char* str);

#endif