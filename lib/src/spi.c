/*
The MIT License (MIT)

Copyright (c) 2019 Techno Road Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <libraries.h>

extern SPI_HandleTypeDef hspi4;

#define SPI &hspi4
#define TIMEOUT 20

void SPI_enable_nss(){
	HAL_GPIO_WritePin(SPI4_nCS_GPIO_Port,SPI4_nCS_Pin,GPIO_PIN_RESET);
	DEBUG_PIN2_HIGH;
}

void SPI_disable_nss(){
	HAL_GPIO_WritePin(SPI4_nCS_GPIO_Port,SPI4_nCS_Pin,GPIO_PIN_SET);
	DEBUG_PIN2_LOW;
}

void SPI_write_byte(uint8_t pdata){
	if(*SPI.Init.DataSize != SPI_DATASIZE_8BIT){return;}

	HAL_SPI_Transmit(SPI,&pdata,1,TIMEOUT);
}

void SPI_write_word(uint16_t pdata){
	if(*SPI.Init.DataSize != SPI_DATASIZE_16BIT){return;}

	uint8_t wd[2] = {pdata&0xFF,(pdata>>8)&0xFF};
	HAL_SPI_Transmit(SPI,wd,1,TIMEOUT);
}

void SPI_write_bytes(uint8_t * pdata,uint16_t length){
	if(*SPI.Init.DataSize != SPI_DATASIZE_8BIT){return;}

	HAL_SPI_Transmit(SPI,pdata,length,TIMEOUT);
}

void SPI_write_words(uint16_t * pdata,uint16_t length){
	if(*SPI.Init.DataSize != SPI_DATASIZE_16BIT){return;}

	uint8_t wb[128];
	memset(wb,0,sizeof(wb));

	for(int i =0;i<length;i++){
		wb[i*2] = *pdata&0xFF;
		wb[i*2 +1] = (*pdata>>8)&0xFF;
	}

	HAL_SPI_Transmit(SPI,(uint8_t *)wb,length,TIMEOUT);
}

void SPI_read_bytes(uint8_t *pdata, uint16_t length){
	if(*SPI.Init.DataSize != SPI_DATASIZE_8BIT){return;}

	HAL_SPI_Receive(SPI,pdata,length,TIMEOUT);
}

void SPI_read_words(uint16_t *pdata, uint16_t length){
	if(*SPI.Init.DataSize != SPI_DATASIZE_16BIT){return;}

	uint8_t rb[128];
	memset(rb,0,sizeof(rb));
	HAL_SPI_Receive(SPI,rb,length,TIMEOUT);

	for(int i =0;i<length;i++){
		pdata[i] = rb[i*2] | (rb[i*2+1] <<8);
	}
}

void SPI_Bloking_wr_word(uint16_t wb,uint16_t *rb){
	if(*SPI.Init.DataSize != SPI_DATASIZE_16BIT){return;}

	uint16_t rdat[2];
	SPI_write_word(wb);
	SPI_read_words(rdat,1);
	rb[0] = rdat[0];
}

void SPI_NonBloking_wr_word(uint16_t wb,uint16_t *rb){
	if(*SPI.Init.DataSize != SPI_DATASIZE_16BIT){return;}

	uint8_t rdat[2] ={0,0};
	uint8_t wd[2] = {wb&0xFF,(wb>>8)&0xFF};
	HAL_SPI_TransmitReceive(SPI,wd,rdat,1,TIMEOUT);

	rb[0] = rdat[0] | (rdat[1] <<8);
}
