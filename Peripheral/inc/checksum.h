/*
 * checksum.h
 *
 *  Created on: Dec 5, 2022
 *      Author: techno-road
 */

#ifndef INC_CHECKSUM_H_
#define INC_CHECKSUM_H_

uint16_t OneComplimentSum(uint32_t sum);
bool CompareChecksum(uint32_t sum);
uint16_t MakeChecksum(uint32_t sum);

#endif /* INC_CHECKSUM_H_ */
