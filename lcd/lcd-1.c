#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <util/twi.h> // i2c/twi library

#define SSD1306_ADDR 0x3C // Default I2C address for SSD1306

// Initialize I2C (TWI)
void i2c_init() {
  // Set SCL frequency to 100kHz with 16MHz clock
  TWBR = 72;
  // Enable TWI
  TWCR = (1 << TWEN);
}

// Send a byte over I2C
void i2c_send_byte(uint8_t data) {
  // Load data into register
  TWDR = data;
  // Clear interrupt flag and start transmission
  TWCR = (1 << TWINT) | (1 << TWEN);
  // Wait for transmission to complete
  while (!(TWCR & (1 << TWINT)))
    ;
}

// Start I2C transmission
void i2c_start() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)))
    ;
}

// Stop I2C transmission
void i2c_stop() { TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); }

// Initialize the OLED display
void ssd1306_init() {
  i2c_init();

  // Start I2C transmission
  i2c_start();
  i2c_send_byte(SSD1306_ADDR << 1); // Send address with write bit

  // Send initialization commands
  i2c_send_byte(0x00); // Command stream
  i2c_send_byte(0xAE); // Display off
  i2c_send_byte(0xD5); // Set display clock
  i2c_send_byte(0x80);
  i2c_send_byte(0xA8); // Set multiplex
  i2c_send_byte(0x3F);
  i2c_send_byte(0xD3); // Set display offset
  i2c_send_byte(0x00);
  i2c_send_byte(0x40); // Set start line
  i2c_send_byte(0x8D); // Charge pump
  i2c_send_byte(0x14);
  i2c_send_byte(0x20); // Memory mode
  i2c_send_byte(0x00);
  i2c_send_byte(0xA1); // Segment remap
  i2c_send_byte(0xC8); // COM direction
  i2c_send_byte(0xDA); // COM pins
  i2c_send_byte(0x12);
  i2c_send_byte(0x81); // Contrast
  i2c_send_byte(0xCF);
  i2c_send_byte(0xD9); // Pre-charge
  i2c_send_byte(0xF1);
  i2c_send_byte(0xDB); // VCOM detect
  i2c_send_byte(0x40);
  i2c_send_byte(0xA4); // Resume display
  i2c_send_byte(0xA6); // Normal display (not inverted)
  i2c_send_byte(0xAF); // Display on

  i2c_stop();
}

void fill_clear_screen() {
  for (uint16_t i = 0; i < 1024; i++) {
    i2c_send_byte(0x00);
  }
}

// Fill entire screen with a pattern
void fill_screen(int offset) {
  i2c_start();
  i2c_send_byte(SSD1306_ADDR << 1);
  i2c_send_byte(0x00); // Command stream
  i2c_send_byte(0x21); // Column address
  i2c_send_byte(0);    // Start at column 0
  i2c_send_byte(127);  // End at column 127
  i2c_send_byte(0x22); // Page address
  i2c_send_byte(0);    // Start at page 0
  i2c_send_byte(7);    // End at page 7
  i2c_stop();

  // Send data
  i2c_start();
  i2c_send_byte(SSD1306_ADDR << 1);
  i2c_send_byte(0x40); // Data stream

  // the goal here is to draw a line down the middle
  for (uint16_t i = 0; i < 1024; i++) {
    if (i % 64 == 0) {
      i2c_send_byte(0xFF); // All pixels on
    } else {
      i2c_send_byte(0x00); // All pixels on
    }
  }

  i2c_stop();
}

int main(void) {
  ssd1306_init();
  int offset = 0;
  while (1) {
    fill_screen(offset);
  }

  return 0;
}
