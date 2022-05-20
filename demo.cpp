#include "encoder_util.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "com_port.h"
#include "rfc_cpr.hpp"
using namespace std; 


CPR m1_cpr;
u8 recBuffer1[192][192];
u8 recBuffer_uv1[192][192];
u8 rfcBinBufferFra1[4096][4096];
u8 rfcBinBufferFra_uv1[4096][4096];
u16 rfcPrmBufferFra1[1024][128];
u16 rfcPrmBufferFra_uv1[1024][128]; 

int main() {
	printf("begin to compress the first sequence...\n");
	
  /*********************this is the read-and-transform part************************************/
	//read file
	ifstream fs("F:\\AVS3IP_1\\H4\\test_Night_traffic_1920x1080_25fps_8bits_33_rec_frm1.txt", ios::binary);
	stringstream ss;
	ss << fs.rdbuf();
	fs.close();
	string str = ss.str();//read into string

	unsigned char   *y = NULL;
	y= (unsigned char*)malloc(10000000);
	int Np = 0;
	for (int i = 0; i < 4149360; i += 2) {
		char p1 = str[i];
		char p2 = str[i + 1];
		unsigned char Nibble[2];

		Nibble[0] = str[i];
		Nibble[1] = str[i + 1];
		int ret = 0;
		for (int j = 0; j < 2; j++)
		{
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A') 
				Nibble[j] = Nibble[j] - 'A' + 10;
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')
				Nibble[j] = Nibble[j] - 'a' + 10;
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')
				Nibble[j] = Nibble[j] - '0';
			else {
				ret = 1;
			}
		}   // for (int j = ...)

		if (ret)
			continue;
		y[i/2] = Nibble[0] << 4;   // Set the high nibble
		y[i/2] |= Nibble[1];   //Set the low nibble
		Np += 1;
	}
/*********************this is the read-and-transform part************************************/
	
	int total_bits=0;
	//frame-level RFC: for test and debug
	//RFC : compress the luma data
	for (int idxY = 0; idxY < 1920; idxY += 4) {
		for (int idxX = 0; idxX < 1080; idxX += 32) {

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 32; j++) {
					recBuffer1[i][j] = y[(idxY + i) * 1080 + idxX + j];
				}
			}
			int block_bits = 0;
			m1_cpr.proc(recBuffer1, 0, 32, 4, &block_bits);
			m1_cpr.cpyDatBit(rfcBinBufferFra1, idxX, idxY, 32, 4);
			m1_cpr.cpyDatPrm(rfcPrmBufferFra1, idxX, idxY, 32, 4);
			total_bits += block_bits;
		}
	}

	printf("the number of conpressed bits is %d \n", total_bits);
	printf("conpression ratio is %5f \n", (double)total_bits / (1920 * 1080 * 8));




	printf(" \n \nbegin to compress the second sequence...\n");
	ifstream fs1("F:\\AVS3IP_1\\H4\\test_Outdoor_crowd_1920x1080_25fps_8bits_33_rec_frm1.txt", ios::binary);
	stringstream ss1;
	ss1 << fs1.rdbuf();
	fs1.close();
	string str1 = ss1.str();//read into string

	unsigned char             *y1 = NULL;
	y1 = (unsigned char*)malloc(10000000);
	for (int i = 0; i < 4149360; i += 2) {
		unsigned char Nibble[2];

		Nibble[0] = str1[i];
		Nibble[1] = str1[i + 1];
		int ret = 0;
		for (int j = 0; j < 2; j++)
		{
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A')
				Nibble[j] = Nibble[j] - 'A' + 10;
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')
				Nibble[j] = Nibble[j] - 'a' + 10;
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')
				Nibble[j] = Nibble[j] - '0';
			else {
				ret = 1;
			}
		}   // for (int j = ...)

		if (ret)
			continue;
		y1[i / 2] = Nibble[0] << 4;   // Set the high nibble
		y1[i / 2] |= Nibble[1];   //Set the low nibble

	}

	
	int total_bits1 = 0;
	//frame-level RFC: for test and debug
	//RFC : compress the luma data
	for (int idxY = 0; idxY < 1920; idxY += 4) {
		for (int idxX = 0; idxX < 1080; idxX += 32) {

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 32; j++) {
					recBuffer1[i][j] = y1[(idxY + i) * 1080 + idxX + j];
				}
			}
			int block_bits = 0;
			m1_cpr.proc(recBuffer1, 0, 32, 4, &block_bits);
			m1_cpr.cpyDatBit(rfcBinBufferFra1, idxX, idxY, 32, 4);
			m1_cpr.cpyDatPrm(rfcPrmBufferFra1, idxX, idxY, 32, 4);
			total_bits1 += block_bits;
		}
	}

	printf("the number of conpressed bits is %d \n", total_bits1);
	printf("conpression ratio is %5f \n", (double)total_bits1 / (1920 * 1080 * 8));

	return 0;
}
