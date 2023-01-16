// RGB888_Convert_to_BMP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "BGR888.h"

using namespace std;

#define IMG_W 640
#define IMG_H 480

#define BI_BITFIELDS 0x3

typedef char BYTE;
typedef short WORD;
typedef int DWORD;
typedef int LONG;



#pragma pack(1)
typedef struct tagBITMAPFILEHEADER {
	WORD bfType;//位图文件的类型，在Windows中，此字段的值总为‘BM’(1-2字节）
	DWORD bfSize;//位图文件的大小，以字节为单位（3-6字节，低位在前）
	WORD bfReserved1;//位图文件保留字，必须为0(7-8字节）
	WORD bfReserved2;//位图文件保留字，必须为0(9-10字节）
	DWORD bfOffBits;//位图数据的起始位置，以相对于位图（11-14字节，低位在前）
	//文件头的偏移量表示，以字节为单位
}BitMapFileHeader;	//BITMAPFILEHEADER;

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;//本结构所占用字节数（15-18字节）
	LONG biWidth;//位图的宽度，以像素为单位（19-22字节）
	LONG biHeight;//位图的高度，以像素为单位（23-26字节）
	WORD biPlanes;//目标设备的级别，必须为1(27-28字节）
	WORD biBitCount;//每个像素所需的位数，必须是1（双色），（29-30字节）
	//4(16色），8(256色）16(高彩色)或24（真彩色）之一
	DWORD biCompression;//位图压缩类型，必须是0（不压缩），（31-34字节）
	//1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
	DWORD biSizeImage;//位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位（35-38字节）
	LONG biXPelsPerMeter;//位图水平分辨率，像素数（39-42字节）
	LONG biYPelsPerMeter;//位图垂直分辨率，像素数（43-46字节)
	DWORD biClrUsed;//位图实际使用的颜色表中的颜色数（47-50字节）
	DWORD biClrImportant;//位图显示过程中重要的颜色数（51-54字节）
}BitMapInfoHeader;	//BITMAPINFOHEADER;

#pragma pack(1)
typedef struct tagRGBQUAD {
	BYTE rgbBlue;//蓝色的亮度（值范围为0-255)
	BYTE rgbGreen;//绿色的亮度（值范围为0-255)
	BYTE rgbRed;//红色的亮度（值范围为0-255)
	BYTE rgbReserved;//保留，必须为0
}RgbQuad;	//RGBQUAD;


int Rgb888ConvertBmp(uint8_t* buf, int width, int height, const char* filename)
{
	FILE* fp;

	BitMapFileHeader bmfHdr; //定义文件头
	BitMapInfoHeader bmiHdr; //定义信息头
	RgbQuad bmiClr[3]; //定义调色板

	bmiHdr.biSize = sizeof(BitMapInfoHeader);
	bmiHdr.biWidth = width;//指定图像的宽度，单位是像素
	bmiHdr.biHeight = height;//指定图像的高度，单位是像素
	bmiHdr.biPlanes = 1;//目标设备的级别，必须是1
	bmiHdr.biBitCount = 24;//表示用到颜色时用到的位数 16位表示高彩色图
	bmiHdr.biCompression = BI_BITFIELDS;//BI_RGB仅有RGB555格式
	bmiHdr.biSizeImage = (width * height * 3);//指定实际位图所占字节数
	bmiHdr.biXPelsPerMeter = 0;//水平分辨率，单位长度内的像素数
	bmiHdr.biYPelsPerMeter = 0;//垂直分辨率，单位长度内的像素数
	bmiHdr.biClrUsed = 0;//位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）
	bmiHdr.biClrImportant = 0;//说明对图象显示有重要影响的颜色索引的数目，0表示所有颜色都重要

	//RGB888格式掩码  B-G-R
	//0x00FF0000
	bmiClr[0].rgbBlue = 0;
	bmiClr[0].rgbGreen = 0;
	bmiClr[0].rgbRed = 0xff;
	bmiClr[0].rgbReserved = 0;
	//0x0000FF00
	bmiClr[1].rgbBlue = 0;
	bmiClr[1].rgbGreen = 0xff;
	bmiClr[1].rgbRed = 0;
	bmiClr[1].rgbReserved = 0;
	//0x000000FF
	bmiClr[2].rgbBlue = 0xff;
	bmiClr[2].rgbGreen = 0;
	bmiClr[2].rgbRed = 0;
	bmiClr[2].rgbReserved = 0;


	bmfHdr.bfType = (WORD)0x4D42;//文件类型，0x4D42也就是字符'BM'
	bmfHdr.bfSize = (DWORD)(sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + sizeof(RgbQuad) * 3 + bmiHdr.biSizeImage);//文件大小
	bmfHdr.bfReserved1 = 0;//保留，必须为0
	bmfHdr.bfReserved2 = 0;//保留，必须为0
	bmfHdr.bfOffBits = (DWORD)(sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + sizeof(RgbQuad) * 3);//实际图像数据偏移量

	if (!(fp = fopen(filename, "wb"))) {
		return -1;
	}
	else {
		printf("file %s open success\n", filename);
	}

	fwrite(&bmfHdr, 1, sizeof(BitMapFileHeader), fp);
	fwrite(&bmiHdr, 1, sizeof(BitMapInfoHeader), fp);
	fwrite(&bmiClr, 1, 3 * sizeof(RgbQuad), fp);

	//BGR
	fwrite(buf, 1, bmiHdr.biSizeImage, fp);	//mirror


	printf("Image size=%d, file size=%d, width=%d, height=%d\n", bmiHdr.biSizeImage, bmfHdr.bfSize, width, height);
	printf("%s over\n", __FUNCTION__);
	fclose(fp);

	return 0;
}



int main()
{
	Rgb888ConvertBmp(imgBGR888, IMG_W, IMG_H, "BGR888.bmp");
    std::cout << "Hello World!\n";
}

