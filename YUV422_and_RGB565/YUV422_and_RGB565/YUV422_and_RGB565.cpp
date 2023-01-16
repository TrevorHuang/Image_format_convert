// YUV422_and_RGB565.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#define IMG_H 0
#define IMG_W 0



void YUV422ToRGB565(uint16_t* inbuf, uint8_t* outbuf)
{
	uint8_t Y, U, Y1, V;
	uint8_t R, G, B;

	for (unsigned int i = 0; i < IMG_H * IMG_W / 2; i++)
	{

		Y = *inbuf >> 8 & 0x00ff;
		U = *inbuf & 0x00ff;
		inbuf++;
		Y1 = *inbuf >> 8 & 0x00ff;
		V = *inbuf & 0x00ff;
		inbuf++;

		R = (1.164 * (Y - 16) + 2.018 * (U - 128));
		G = (1.164 * (Y - 16) - 0.392 * (U - 128) - 0.813 * (V - 128));
		B = (1.164 * (Y - 16) + 1.159 * (V - 128));


		*outbuf++ = B;
		*outbuf++ = G;
		*outbuf++ = R;

		R = (1.164 * (Y1 - 16) + 2.018 * (U - 128));
		G = (1.164 * (Y1 - 16) - 0.392 * (U - 128) - 0.813 * (V - 128));
		B = (1.164 * (Y1 - 16) + 1.159 * (V - 128));


		*outbuf++ = B;
		*outbuf++ = G;
		*outbuf++ = R;

	}

}



int main()
{
    std::cout << "Hello World!\n";
}

