#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;


int main()
{
    GdiplusStartupInput gdiplusstartupinput;
    ULONG_PTR gdiplustoken;
    GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, nullptr);

    wstring infilename(L"1.jpg");
    string outfilename("rgb888.h");
    //读图片
    Bitmap* bmp = new Bitmap(infilename.c_str());
    UINT height = bmp->GetHeight();
    UINT width = bmp->GetWidth();
    cout << "width " << width << ", height " << height << endl;

    Color color;
    ofstream fout(outfilename.c_str());
    fout << "#ifndef _COLOR_H_" << endl;
    fout << "#define _COLOR_H_" << endl;
    fout << "uint8_t imgRGB888[] = {" << endl;

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            bmp->GetPixel(x, y, &color);
            fout << "0x";
            fout << hex << (int)color.GetBlue() << ",";
            fout << "0x";
            fout << hex << (int)color.GetGreen() << ",";
            fout << "0x";
            fout << hex << (int)color.GetRed() << ",";
        }
    fout << "};" << endl;
    fout << "#endif" << endl;

    fout.close();

    delete bmp;
    GdiplusShutdown(gdiplustoken);
    return 0;
}