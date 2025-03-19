#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__
#define BOARD "DE1-SoC"
/* Memory */
#define DDR_BASE 0x40000000
#define DDR_END 0x7FFFFFFF
#define SDRAM_BASE 0x00000000
#define SDRAM_END 0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE 0x08000000
#define FPGA_PIXEL_BUF_END 0x0803FFFF
#define FPGA_CHAR_BASE 0x09000000
#define FPGA_CHAR_END 0x09001FFF
/* Cyclone V FPGA devices */
#define LED_BASE 0xFF200000
#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050
#define JP1_BASE 0xFF200060
#define JP2_BASE 0xFF200070
#define PS2_BASE 0xFF200100
#define PS2_DUAL_BASE 0xFF200108
#define JTAG_UART_BASE 0xFF201000
#define IrDA_BASE 0xFF201020
#define TIMER_BASE 0xFF202000
#define TIMER_2_BASE 0xFF202020
#define AV_CONFIG_BASE 0xFF203000
#define RGB_RESAMPLER_BASE 0xFF203010
#define PIXEL_BUF_CTRL_BASE 0xFF203020
#define CHAR_BUF_CTRL_BASE 0xFF203030
#define AUDIO_BASE 0xFF203040
#define VIDEO_IN_BASE 0xFF203060
#define EDGE_DETECT_CTRL_BASE 0xFF203070
#define ADC_BASE 0xFF204000
/* Cyclone V HPS devices */
#define MTIME_BASE 0xFF202100
#endif
	
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
	
static void handler(void) __attribute__ ((interrupt ("machine")));
void KEYBOARD_ISR();
bool prev_release_code = false;
char correct[] = "APPLE";
char word[6][6] = {0}; //5 letters, 1 null char \0
int letterIdx = 0; //which letter we're on
int wordIdx = 0; //word we're on
	
/* function prototypes */
void HEX_PS2(char, char, char);
/*******************************************************************************
* This program demonstrates use of the PS/2 port by displaying the last three
* bytes of data received from the PS/2 port on the HEX displays.
******************************************************************************/

int main(void) {
	volatile int * PS2_ptr = (int *)PS2_BASE;
	//int PS2_data, RVALID, interrupt;
	//char byte1 = 0, byte2 = 0, byte3 = 0;
	
	//setup interrupts in keyboard -> set RE to 1
	//int PS2_control = *(PS2_ptr+1); //get control register -> do we need to get it then only set that one bit?
	*(PS2_ptr+1) = 0b1;
	//QUESTION: do i have to poll the RI bit myself? or does it do it automatically ugh
	
	//setup interrupts in interrupt regs
	
	// disable interrupts in case on
	int mstatus_value, mtvec_value, mie_value;
	mstatus_value = 0b1000; // interrupt bit mask -> CLEAR mstatus reg mie bit 3 by writing 1 to it
	__asm__ volatile ("csrc mstatus, %0" :: "r"(mstatus_value));
	
	// set trap address (mtvec)
	mtvec_value = (int) &handler; 
	__asm__ volatile ("csrw mtvec, %0" :: "r"(mtvec_value));
	
	// disable all interrupts that are currently enabled
	__asm__ volatile ("csrr %0, mie" : "=r"(mie_value));
	__asm__ volatile ("csrc mie, %0" :: "r"(mie_value));
	mie_value = 0b10000000000000000000000; // keyboard interrupt only (ONE HOT 0-indexed 22)//1010000000010001000
	
	// set interrupt enables
	__asm__ volatile ("csrs mie, %0" :: "r"(mie_value));
	
	// enable Nios V interrupts
	__asm__ volatile ("csrs mstatus, %0" :: "r"(mstatus_value));

	
	//*(PS2_ptr) = 0xFF; // reset -> whats the point of this? for the display?
	
	while (1) {
		//loop does nothing for now
		
	}
}

void handler (void){
	int mcause_value;
	__asm__ volatile ("csrr %0, mcause" : "=r"(mcause_value));

	//ps2 keyboard port irq22 (BINARY 22) 0b10000000000000000000000000010110
	if (mcause_value == 0b10000000000000000000000000010110) 
		KEYBOARD_ISR(); //make keybaord isr!
		
	// else, ignore the trap
}

void KEYBOARD_ISR(){
	
	//interrupt generated every time something is there (both press + release)
	//what if i only want it to give me an interrupt when released??? -> just handle in here
	
	volatile int * PS2_ptr = (int *)PS2_BASE;
	
	int PS2_data = *(PS2_ptr); 		// read the Data register in the PS/2 port
	int RVALID = PS2_data & 0x8000; 		// extract the RVALID field -> just to be safe
	short int DATA = PS2_data & 0xFF; //extract data field (max 16 bit)
	//char check_break = DATA >> 8; 		//extract 2 most sig bytes of data
	
	//printf("0x%04X\n", DATA);
	//ISSUE: interrupt is sent twice when released -> once for release key, once for actual letter :(
	//key pressed printing along with key released when releasing a key
	//need a way to remember if the previous was a break.
	
	if(RVALID){
		//if most sig 2 bytes are 11110000 -> means break -> released -> do something
		if(DATA == 0b11110000){ //on first byte of releasing
			prev_release_code = true;
			//printf("Key releasing...\n");
			
		} else if (prev_release_code) { //if previous was 11110000, on second byte of releasing, store the released val
			prev_release_code = false;
			//printf("Key released! Storing letter...\n"); 
			
			//DATA is actual letter
			char letter;
			int back_or_enter = -1; //= 0 for back, 1 for enter, -1 for unset
			
			//very long switch
			switch (DATA) {
				case 0x1C: letter = 'A'; break;
				case 0x32: letter = 'B'; break;
				case 0x21: letter = 'C'; break;
				case 0x23: letter = 'D'; break;
				case 0x24: letter = 'E'; break;
				case 0x2B: letter = 'F'; break;
				case 0x34: letter = 'G'; break;
				case 0x33: letter = 'H'; break;
				case 0x43: letter = 'I'; break;
				case 0x3B: letter = 'J'; break;
				case 0x42: letter = 'K'; break;
				case 0x4B: letter = 'L'; break;
				case 0x3A: letter = 'M'; break;
				case 0x31: letter = 'N'; break;
				case 0x44: letter = 'O'; break;
				case 0x4D: letter = 'P'; break;
				case 0x15: letter = 'Q'; break;
				case 0x2D: letter = 'R'; break;
				case 0x1B: letter = 'S'; break;
				case 0x2C: letter = 'T'; break;
				case 0x3C: letter = 'U'; break;
				case 0x2A: letter = 'V'; break;
				case 0x1D: letter = 'W'; break;
				case 0x22: letter = 'X'; break;
				case 0x35: letter = 'Y'; break;
				case 0x1A: letter = 'Z'; break;
				case 0x66: //backspace: delete the letter
					printf("BACKSPACE! Deleting letter...\n");
					back_or_enter = 0;
					break;
				case 0x5A: //enter: lock in the word
					printf("ENTER! Locking in word...\n");
					back_or_enter = 1;
					break;
				default: printf("Unknown key: %02X\n", DATA); break;
			}
				
			
			//if backspace/enter unset -> store a letter
			if(back_or_enter == -1){
				
				if(letterIdx <= 4){ //4 is 0-indexed -> currently lower than at 5th letter or currently entering 5th letter
					word[wordIdx][letterIdx] = letter;
					letterIdx++; //increase for next loop
				} else if(letterIdx > 4) { //5th letter entered in current round/going to enter 6th word
					printf("Too many letters!\n");
					letterIdx = 5; //make letterIdx stay at null char, dont enter new letter
					return;
				}
				
				
			} else if(back_or_enter == 0){ //delete
				letterIdx--; //go back to prev letter to delete it
				word[wordIdx][letterIdx] = '\0';
				
			} else if(back_or_enter == 1){ //lock in word
				
				if(wordIdx <= 5){ //5 is 0-indexed -> currently lower than 6th word or entering 6th word
					printf("Word: %s\n", word[wordIdx]);
					
					//HERE CHECK IF GAME WON/LOST/LETTER STUFF
					
					letterIdx = 0; //go back to beginning
					wordIdx++; //next row (new word)
				} 
				if (wordIdx > 5){ //6th word entered in current round, going to entering 7th word
					printf("Tries are up!\n");
					
					//DISPLAY CORRECT WORD
					
					return;
				}
			}

			
		} else { //if previous was NOT 11110000, on only byte of pressing, do nothing
			prev_release_code = false;
			//printf("Key pressing...\n"); 
		}
	}//else dont do anything
	return;
}
