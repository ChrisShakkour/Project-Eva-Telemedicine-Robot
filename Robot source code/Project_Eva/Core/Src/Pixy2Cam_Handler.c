/*
 * Pixy2Cam_Handler.c
 *
 *  Created on: Nov 6, 2020
 *      Author: chris
 */

#include "main.h"

SPI_HandleTypeDef hspi1;
unsigned char inbuffer[30] = {0};
uint8_t sync_byte;


void sync_inbuff(){
//
	for(int i=0; i<5; i++){
		HAL_SPI_Receive(&hspi1, (uint8_t*)&inbuffer[0], 1, 10);
		HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
		//if(inbuffer[0])
	}
	//return 0;
}

void recieve_Initializer(){

}

void Recieve_Data(uint8_t data_len){


}
/*
void Pixy_SPI_TXn_RXm(uint8_t *buffer_ptr, uint8_t Size){
//
	memset(&inbuffer[0], 0, 12);
	HAL_GPIO_WritePin(GPIOB, SPI1_SS_Pin, 0);
	//HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)buffer_ptr, (uint8_t*)&inbuffer[0], 2, 10);
	//HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer_ptr, 2, 10);
	//for(int i=0; i<Size; i++){
		//HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)buffer_ptr, (uint8_t*)&inbuffer[0], 6);

	//}
	HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer_ptr, 6, 10);
	HAL_SPI_Receive(&hspi1, (uint8_t*)&inbuffer[0], 7, 10);
	HAL_GPIO_WritePin(GPIOB, SPI1_SS_Pin, 1);
}
*/
uint8_t Transsmit_Recieve_SPI(uint8_t *buffer_ptr, uint8_t Rec_Size){
//
	uint8_t cnt=0;
	uint8_t temp[40] = {0};
	uint8_t payload_size =0;
	memcpy(&temp[0], (uint8_t *)buffer_ptr, Rec_Size);
	memset(&inbuffer[0], 0, sizeof(inbuffer));
	HAL_GPIO_WritePin(GPIOB, SPI1_SS_Pin, 0);
	// sync on 0xc1ae little Endainess byte
	do {
		inbuffer[0] = inbuffer[1];
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&temp+cnt, (uint8_t*)&inbuffer[1], 1, 1);
		if(cnt++ >20) return 0;
	} while((inbuffer[0]!=0xaf) || (inbuffer[1]!=0xc1));
	//get packet type and payload size
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&temp+cnt+1, (uint8_t*)&inbuffer[2], 2, 5);
	payload_size = inbuffer[3];
	// get all relevant data:
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&temp+cnt+3, (uint8_t*)&inbuffer[4], payload_size+2, 50);
	HAL_GPIO_WritePin(GPIOB, SPI1_SS_Pin, 1);
	return 0;
}

void Pixy_SPI_Blind_Transmit(uint8_t *buffer_ptr, uint8_t Size){
//
	memset(&inbuffer[0], 0, sizeof(inbuffer));
	HAL_GPIO_WritePin(GPIOB, SPI1_SS_Pin, 0);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)buffer_ptr, (uint8_t*)&inbuffer[0], Size, 50);
	HAL_GPIO_WritePin(GPIOB, SPI1_SS_Pin, 1);
}


/*  SetLamp(upper, lower):
 *	Byte	Description			Value(s)
 *	0 - 1	16-bit sync			174, 193 (0xc1ae) Little Indians
 *	2		Type of packet		22
 *	3		Length of payload	2
 *	4		Upper - turn on the two white LEDs along Pixy2 top edge	0 (off) or 1 (on)
 *	5		Lower - turn on all channels of lower RGB LED	0 (off) or 1 (on)
 */
void Set_Lamp(char upper, char lower){
//
	uint8_t temp_Buff[14] = {0xae, 0xc1,
							 0x16, 0x02,
							 upper, lower,
							 0, 0, 0, 0,
							 0, 0, 0, 0
							};
	Pixy_SPI_Blind_Transmit(&temp_Buff[0], sizeof(temp_Buff));
}

/*	SetRGB(r, g, b):
 *	Byte	Description			Value(s)
 * 	0 - 1	16-bit sync			174, 193 (0xc1ae)
 *	2		Type of packet		20
 *	3		Length of payload	3
 *	4		r - Red value		0 - 255
 *	5		g - Green value		0 - 255
 *	6		b - Blue value		0 - 255
 */
void Set_RGB_LED(uint8_t Red, uint8_t Green, uint8_t Blue){
//
	uint8_t temp_Buff[15] = {0xae, 0xc1,
						   	 0x14, 0x03,
							 Red, Green, Blue,
							 0, 0, 0, 0,
							 0, 0, 0, 0
							 };
	Pixy_SPI_Blind_Transmit(&temp_Buff[0], sizeof(temp_Buff));
}


/* setCameraBrightness(brightness)
 * 	Byte	Description			Value(s)
 *	0 - 1	Sync				174, 193 (0xc1ae)
 *	2		Type of packet		16
 *	3		Length of payload	1
 *	4		Brightness			0 - 255
 */
void Set_Camera_Brightness(uint8_t brightness){
//
	uint8_t temp_Buff[13] = {0xae, 0xc1,
							 0x10, 0x01,
							 brightness,
							 0, 0, 0, 0,
							 0, 0, 0, 0
							};
	Pixy_SPI_Blind_Transmit(&temp_Buff[0], sizeof(temp_Buff));
}

/*
 *
 *
 */
void Reqeust_Line_Tracking_Object_Data(uint8_t Feature, Pixy2_Data_LTC *LTCData){
//
	uint8_t temp_Buff[6] = {0xae, 0xc1,
							0x30, 0x02,
							0x01, Feature
							 };
	if(Transsmit_Recieve_SPI(&temp_Buff[0], sizeof(temp_Buff)) == 0){
	//
		LTCData->Barcode = inbuffer[11];
		LTCData->X_pos = inbuffer[8];
		LTCData->Y_pos = inbuffer[9];
	}
}


void Get_Color_Connected_Components(uint8_t sigmap, uint8_t blocks, Pixy2_Data_CCC *Signature){
//
	uint8_t temp_Buff[6] = {0xae, 0xc1,
							0x20, 0x02,
							sigmap, blocks
							};
	if(Transsmit_Recieve_SPI(&temp_Buff[0], sizeof(temp_Buff)) == 0){
	//
		Signature->Signature = inbuffer[6] +(inbuffer[7]<<8);  //little Endianess
		Signature->X_pos 	 = inbuffer[8] +(inbuffer[9]<<8);  //little Endianess
		Signature->Y_pos 	 = inbuffer[10]+(inbuffer[11]<<8); //little Endianess
		Signature->Width 	 = inbuffer[12]+(inbuffer[13]<<8); //little Endianess
		Signature->Hight 	 = inbuffer[14]+(inbuffer[15]<<8); //little Endianess
		Signature->Angle 	 = inbuffer[16]+(inbuffer[17]<<8); //little Endianess
		Signature->Tracking  = inbuffer[18];
		Signature->Age 		 = inbuffer[19];
	}
}

