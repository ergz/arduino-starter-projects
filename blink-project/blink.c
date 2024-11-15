#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

int main(void) {
  // Set PORTB5 (Pin 13) as output
  DDRB |= (1 << DDB5);

  while (1) {
    // Toggle LED
    PORTB |= (1 << PORTB5); // LED on
    _delay_ms(500);
    PORTB &= ~(1 << PORTB5); // LED off
    _delay_ms(500);
  }

  return 0;
}
