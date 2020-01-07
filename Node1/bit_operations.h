/** @file bitoperations.h
 *  @brief File containing useful bit-operations used throughout the project.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

#include <avr/io.h>

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
#define toggle_bit( reg,bit ) (reg ^= ( 1 << bit))
#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )

#endif
