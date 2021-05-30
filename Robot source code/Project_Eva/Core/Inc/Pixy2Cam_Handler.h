/*
 * Pixy2Cam_Handler.h
 *
 *  Created on: Nov 6, 2020
 *      Author: chris
 */

#ifndef INC_PIXY2CAM_HANDLER_H_
#define INC_PIXY2CAM_HANDLER_H_

/* Signatures definitions*/
#define Sig_A 20
#define Sig_B 40
#define BARCODE 4
#define VECTOR 1
#define INTERSECTION 2

#define PIXY_CHECKSUM_SYNC          		 0xc1af
#define PIXY_NO_CHECKSUM_SYNC       		 0xc1ae
#define PIXY_SEND_HEADER_SIZE				 4

#define PIXY_TYPE_REQUEST_CHANGE_PROG        0x02
#define PIXY_TYPE_REQUEST_RESOLUTION         0x0c
#define PIXY_TYPE_RESPONSE_RESOLUTION        0x0d
#define PIXY_TYPE_REQUEST_VERSION            0x0e
#define PIXY_TYPE_RESPONSE_VERSION           0x0f
#define PIXY_TYPE_RESPONSE_RESULT            0x01
#define PIXY_TYPE_RESPONSE_ERROR             0x03
#define PIXY_TYPE_REQUEST_BRIGHTNESS         0x10
#define PIXY_TYPE_REQUEST_SERVO              0x12
#define PIXY_TYPE_REQUEST_LED                0x14
#define PIXY_TYPE_REQUEST_LAMP               0x16
#define PIXY_TYPE_REQUEST_FPS                0x18

#define PIXY_RESULT_OK                       0
#define PIXY_RESULT_ERROR                    -1
#define PIXY_RESULT_BUSY                     -2
#define PIXY_RESULT_CHECKSUM_ERROR           -3
#define PIXY_RESULT_TIMEOUT                  -4
#define PIXY_RESULT_BUTTON_OVERRIDE          -5
#define PIXY_RESULT_PROG_CHANGING            -6



typedef struct
{
	int16_t Signature;	//0-255
	int16_t X_pos;		//0-315
	int16_t Y_pos;		//0-207
	int16_t Width;		//0-316
	int16_t Hight;		//0-208
	int16_t Angle;		//-180/0
	uint8_t Tracking;	//0-255
	uint8_t Age;		//0-255
}Pixy2_Data_CCC;
// CCC: Color Connected Components

typedef struct
{
	int16_t Feature;	//1-vector/2-intersection/4-barcode
	int16_t X_pos;		//0-315
	int16_t Y_pos;		//0-207
	uint8_t Barcode;    //0-16
}Pixy2_Data_LTC;
// LTC: Line Tracking Component

uint8_t Transsmit_Recieve_SPI(uint8_t *buffer_ptr, uint8_t Rec_Size);
void sync_inbuff();
void recieve_Initializer();
void Recieve_Data(uint8_t data_len);


void Pixy_SPI_TXn_RXm(uint8_t *buffer_ptr, uint8_t Size);
void Pixy_SPI_Blind_Transmit(uint8_t *buffer_ptr, uint8_t Size);
void Set_Lamp(char upper, char lower);
void Set_RGB_LED(uint8_t Red, uint8_t Green, uint8_t Blue);
void Set_Camera_Brightness(uint8_t brightness);
void Reqeust_Line_Tracking_Object_Data(uint8_t Feature, Pixy2_Data_LTC *LTCData);
void Get_Color_Connected_Components(uint8_t sigmap, uint8_t blocks, Pixy2_Data_CCC *Signature);

#endif /* INC_PIXY2CAM_HANDLER_H_ */
