//
// Created by jeff2310 on 5/10/18.
//

#include "FactralNoise.h"
#include "PerlinNoise.h"
#include <FreeImage.h>
#include <string>
#include <sstream>

void generateTexture512(){
    Noise noise(512);

    FreeImage_Initialise();

    int oct;
    int octaveLevel = 7;
    int samplePeriod;
    float sampleFreq;
    float fractalNoise[512][512];

    FIBITMAP *bitmap;

    BYTE *bits;

    for (oct = 1; oct <= octaveLevel; oct++) {
        PerlinNoise perlinNoise(512);
        bitmap = FreeImage_AllocateT(FIT_BITMAP, 512, 512);
        int bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
        samplePeriod = 1<<(oct-1);
        sampleFreq = 1/(float)samplePeriod;
        int sampleX, sampleY, sampleX1, sampleY1;
        int height=FreeImage_GetHeight(bitmap), width=FreeImage_GetWidth(bitmap);
        for (int y = 0; y < height; y++) {
            bits = FreeImage_GetScanLine(bitmap, y);
            sampleY = (y/samplePeriod)*samplePeriod;
            sampleY1 = (sampleY + samplePeriod) % height;
            float y_blend = (y-sampleY)*sampleFreq;
            for (int x = 0; x < width; x++) {
                sampleX = (x/samplePeriod)*samplePeriod;
                sampleX1 = (sampleX + samplePeriod) % width;
                float x_blend = (x-sampleX)*sampleFreq;

                float noise00, noise01, noise10, noise11, noise0, noise1;
                float sampleNoise;
                unsigned char color;
                noise00 = perlinNoise.noiseValue((float) sampleX + 0.5f, (float) sampleY + 0.5f) * 0.5f + 0.5f;
                noise10 = perlinNoise.noiseValue((float) sampleX1 + 0.5f, (float) sampleY + 0.5f) * 0.5f + 0.5f;
                noise01 = perlinNoise.noiseValue((float) sampleX + 0.5f, (float) sampleY1 + 0.5f) * 0.5f + 0.5f;
                noise11 = perlinNoise.noiseValue((float) sampleX1 + 0.5f, (float) sampleY1 + 0.5f) * 0.5f + 0.5f;

                noise0 =  (noise00 + (noise10-noise00) * x_blend);
                noise1 = (noise01 + (noise11-noise01) * x_blend);

                sampleNoise = (noise0 + (noise1-noise0) * y_blend);
                color = (unsigned char)floorf(sampleNoise * 255);

                bits[FI_RGBA_RED] = bits[FI_RGBA_BLUE] = bits[FI_RGBA_GREEN] = color;
                bits += bytespp;

                fractalNoise[x][y] += sampleNoise / (1<<(octaveLevel-oct+1));
            }
        }
        std::ostringstream buffer;
        buffer << "octave_" << oct << ".png";
        std::string name_str = buffer.str();
        FreeImage_Save(FIF_PNG, bitmap, name_str.c_str(), 0);
        FreeImage_Unload(bitmap);
    }

    bitmap = FreeImage_AllocateT(FIT_BITMAP, 512, 512);
    int height=FreeImage_GetHeight(bitmap), width=FreeImage_GetWidth(bitmap);
    int bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
    for(int y=0; y<height; y++){
        bits = FreeImage_GetScanLine(bitmap, y);
        for(int x=0; x<width; x++){
            bits[FI_RGBA_RED] = bits[FI_RGBA_BLUE] = bits[FI_RGBA_GREEN] = (unsigned char)floorf(fractalNoise[x][y] * 255);;
            bits+=bytespp;
        }
    }
    FreeImage_Save(FIF_PNG, bitmap, "fractal_noise.png", 0);

    FreeImage_DeInitialise();
}