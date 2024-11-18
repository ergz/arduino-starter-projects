#include <stdbool.h>
#define F_CPU 16000000UL

#include <util/delay.h>

#define DDRB_ADDRESS (*(volatile uint8_t *)0x24)
#define PORTB_ADDRESS (*(volatile uint8_t *)0x25)

int main(void) {

  DDRB_ADDRESS |= (1 << 5);

  while (true) {
    PORTB_ADDRESS |= (1 << 5);
    _delay_ms(500);
    PORTB_ADDRESS &= ~(1 << 5);
    _delay_ms(500);
  }

  return 0;
}
