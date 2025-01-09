/*
 * bitwise_operation.h
 *
 *  Created on: Mar 13, 2022
 *      Author: khanh
 */

#ifndef UTIL_BITWISE_OPERATION_H_
#define UTIL_BITWISE_OPERATION_H_

// Read Bit at any position
#define READ_BIT(buff, pos)                  ((buff>>pos) & 0x01)

// Write Bit at any position
#define WRITE_BIT(buff, val, pos)            (buff &= ~(1<<pos)); buff |= (val<<pos)

#endif /* UTIL_BITWISE_OPERATION_H_ */
