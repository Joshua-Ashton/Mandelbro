#include <complex.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{
	unsigned int Width = 1920;
    unsigned int Height = 1080;
    const char* OutputFilename = "mandelbro.png";

    for (int i = 1; i < argc; i++)
    {
        if ( ( strcmp(argv[i], "-h") == 0 && argc <= i + 1 ) || strcmp(argv[i], "--help") == 0)
        {
            puts(
            "Help for Mandelbro:\n"
            "Mandelbro is a simple cross-platform portable application to generate images of the Mandelbrot set at a given resolution.\n"
            "It saves the images in .png format for simplicity.\n"
            "COMMANDS:\n"
            "--help (-h) -> Gives you this.\n"
            "--width (-w) -> Sets the width of the generated image. [Default = 1920]\n"
            "--height (-h) -> Sets the height of the generated image. [Default = 1080]\n"
			"--size (-s) -> Sets the size of the generated image. Eg. 1920x1080 x S. [No default.]\n"
            "--output (-o) -> Sets the output filename (No spaces please!) [Default = mandelbro.png]");
            return 1;
        }

        if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0)
        {
            if (argc >= i)
                Width = atoi(argv[i + 1]);
            continue;
        }

        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0)
        {
            if (argc >= i)
                Height = atoi(argv[i + 1]);
            continue;
        }

        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (argc >= i)
                OutputFilename = argv[i + 1];
            continue;
        }
		
		if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0)
        {
            if (argc >= i)
            {
				int k = atoi(argv[i + 1]);
				Width = k * 1920;
				Height = k * 1080;
			}
            continue;
        }
    }

    const unsigned int Size = Width * Height;

    unsigned char* bitmap = (unsigned char*)malloc(Size);
    if (!bitmap)
    {
        puts("Too big!");
        return 1;
    }
	
    #pragma omp parallel for schedule(dynamic)
    for (unsigned int y = 0; y < Height; y++)
    {
		unsigned int yTimesWidth = y * Width;
        for (unsigned int x = 0; x < Width; x++)
        {
            double _Complex c = 1.0 * (x - (Width - Height) / 2.0 ) / Height * 4.0 - 2.0 + (1.0 * y / Height * 4.0 - 2.0) * _Complex_I;
            double _Complex z = 0.0;

			unsigned char shade;
            for (shade = 0; shade < 255; shade++)
            {
                z = z * z + c;
                if (cabs(z) > 2.0)
                    break;
            }

            bitmap[x + yTimesWidth] = shade;
        }
    }

	stbi_write_png(OutputFilename, Width, Height, 1, bitmap, 0);

    free(bitmap);

    return 0;
}