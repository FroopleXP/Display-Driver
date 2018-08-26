/*
 * Blinky.c
 *
 * Created: 06/08/2018 21:04:47
 * Author : Connor D. Edwards
 */ 

/*
	
	DIN: ARPIN 11	- PB3 00001000 = 0x08
	DCK: ARPIN 10	- PB2 00000100 = 0x04
	CLR: ARPIN 9	- PB1 00000010 = 0x02
					- TOT 00001110 = 0x0E
	
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define CLOCK (1 << PORTB2)
#define DATA (1 << PORTB3)
#define REFRESH (1 << PORTB1)

uint8_t chars[] = {
	0b11101110, // A
	0b00111110, // b
	0b10011100, // C
	0b01111010, // d
	0b10011110, // E
	0b10001110, // F
	0b11110110, // g
	0b00101110, // h
	0b01100000, // i
	0b01110000, // J
	0b01101110, // K
	0b00011100, // L
	0b00000010, // m
	0b00101010, // n
	0b00111010, // o
	0b11001110, // P
	0b11100110, // q
	0b00001010, // r
	0b10110100, // S
	0b00011110, // t
	0b01111100, // u
	0b00111000, // v
	0b00000010, // w
	0b00000010, // x
	0b01100110, // Y
	0b00000010  // z
};

void writeWordOut(uint16_t byteToWrite) {
	
	// Setting the DATA and CLOCK lines low
	PORTB &= !(DATA | CLOCK);
	
	for (uint8_t n = 0; n < 16; n++) {
		
		// Setting the data pin high or low depending on the bit state
		(byteToWrite & (1 << n)) ? (PORTB |= DATA) : (PORTB &= !DATA);
		
		// Clock the data in
		PORTB |= CLOCK;
		_delay_us(10);
		PORTB &= !CLOCK;
		
	}
	
	// Refresh the output
	PORTB |= REFRESH;
	_delay_us(1);
	PORTB &= !REFRESH;
	
}

int main(void) {	
	
	DDRB = CLOCK | DATA | REFRESH; // Set direction of port B
	
	// Holds displays
	uint16_t bytesToWrite[] = {
		0b1110111010000000,
		0b0010101001000000,
		0b0111000000100000,
		0b1110111000010000
	};
	
	char message[] = {
		'R', 'O', 'R', 'Y'
	};
	
	// Main loop
    while (1) {		
		
		for (uint8_t i = 0; i < sizeof(message); i++) {
			uint16_t letter = chars[(int)message[i] - 65] << 8;
			uint16_t final = letter | (0b10000000 >> i);
			writeWordOut(final);
			_delay_us(1);
		}
		
    }
}

