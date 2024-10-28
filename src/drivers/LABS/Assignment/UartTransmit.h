#ifndef UARTTRANSMIT_H
#define UARTTRANSMIT_H

void uart_init();  // Declare the uart_init function
void uart_send(const char *message);
char uart_receive();
void on_uart_rx();

#endif
