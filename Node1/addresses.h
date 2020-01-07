/** @file addresses.h
 *  @brief File containing useful addresses for the ADC, OLED and SRAM.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef ADDRESSES_H
#define ADDRESSES_H

// ADC
#define adc_addr ((volatile char*) 0x1400)


//OLED
#define cmnd_oled_addr ((volatile char*) 0x1000)
#define data_oled_addr ((volatile char*) 0x1200)

// OLED command addresses
#define oled_col_start_addr 0x00
#define oled_col_end_addr 0x7f

#define oled_page_start_addr_0 0xb0

#define oled_page_start_addr 0x00
#define oled_page_end_addr 0x07

#define oled_set_col_addr 0x21
#define oled_set_page_addr 0x22



// SRAM
#define sram_addr ((volatile char*) 0x1800)

#endif
