#include <avr/io.h>
#include <util/delay.h>

int main(void) {

  // set the LED pin as output
  DDRB |= (1 << DDB5);

  // Set up button pin (PORTB4/Pin 12) as input with pull-up resistor

  // right hand side will expand to ~(1 << 4) --> ~(00010000) --> (11101111)
  // and how we apply the &= DDRB = DDRB & (11101111)
  // which basically just turns off bit 5 in this register and making in an
  // input
  DDRB &= ~(1 << DDB4);   // Set as input
  PORTB |= (1 << PORTB4); // Enable pull-up resistor

  while (1) {
    if (!(PINB & (1 << PINB4))) {
      PORTB |= (1 << PORTB5);
    } else {
      PORTB &= ~(1 << PORTB5);
    }
  }

  return 0;
}
