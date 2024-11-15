#!/bin/bash

# Configuration
MCU="atmega328p"
CPU_FREQ="16000000UL"
PORT="/dev/ttyACM0"
BAUD="115200"
AVR_INCLUDE="/usr/lib/avr/include" # Add this line

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Check if source file exists
if [ ! -f "$1" ]; then
  echo -e "${RED}Error: Please provide a .c file as argument${NC}"
  echo "Usage: $0 your_file.c"
  exit 1
fi

SOURCE=$1
FILENAME="${SOURCE%.*}"

echo -e "${GREEN}Compiling $SOURCE for Arduino Uno (ATmega328P)...${NC}"

# Modified compile steps with explicit include path
avr-gcc -Os -DF_CPU=$CPU_FREQ -mmcu=$MCU -I$AVR_INCLUDE -c -o "${FILENAME}.o" "$SOURCE" &&
  avr-gcc -mmcu=$MCU "${FILENAME}.o" -o "${FILENAME}.elf" &&
  avr-objcopy -O ihex -R .eeprom "${FILENAME}.elf" "${FILENAME}.hex"

if [ $? -eq 0 ]; then
  echo -e "${GREEN}Compilation successful!${NC}"
  echo -e "${GREEN}Uploading to Arduino...${NC}"

  # Upload to Arduino
  avrdude -F -V -c arduino -p $MCU -P $PORT -b $BAUD -U flash:w:"${FILENAME}.hex":i

  # Clean up
  rm "${FILENAME}.o" "${FILENAME}.elf" "${FILENAME}.hex"
else
  echo -e "${RED}Compilation failed!${NC}"
  exit 1
fi
