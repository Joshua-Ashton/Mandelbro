#include <complex>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	unsigned int Width = 1920;
    unsigned int Height = 1080;
    const char* OutputFilename = "mandelbro.ppm";

    for (int i = 1; i < argc; i++)
    {
        if ( ( strcmp(argv[i], "-h") == 0 && argc <= i + 1 ) || strcmp(argv[i], "--help") == 0)
        {
            puts(
            "Help for Mandelbro:\n"
            "Mandelbro is a simple cross-platform portable application to generate images of the Mandelbrot set at a given resolution.\n"
            "It saves the images in .ppm format for simplicity.\n"
            "COMMANDS:\n"
            "--help (-h) -> Gives you this.\n"
            "--width (-w) -> Sets the width of the generated image. [Default = 1920]\n"
            "--height (-h) -> Sets the height of the generated image. [Default = 1080]\n"
            "--output (-o) -> Sets the output filename (No spaces please!) [Default = mandelbro.ppm]");
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
    }

    constexpr unsigned int RGB = 3;
    const unsigned int Size = Width * Height * RGB;

    unsigned char* bitmap = new unsigned char[Size];
    if (!bitmap)
    {
        puts("Too big!");
        return -1;
    }

    #pragma omp parallel for schedule(dynamic)
    for (unsigned int x = 0; x < Width; x++)
    {
        for (unsigned int y = 0; y < Height; y++)
        {
            const std::complex<double> c(1.0 * (x - (Width - Height) / 2 ) / Height * 4.0 - 2.0, 1.0 * y / Height * 4.0 - 2.0);
            std::complex<double> z(0.0, 0.0);

            unsigned char shade;
            for (shade = 0; shade != 255; shade++)
            {
                z = z * z + c;
                if (abs(z) > 2.0)
                    break;
            }

            #pragma omp simd
            for (unsigned int i = 0; i < RGB; i++)
            {
                bitmap[(x * RGB + i) + (y * Width * RGB)] = shade;
            }
        }
    }

    // Save in ppm image format for simplicity.
    FILE* fp = fopen(OutputFilename, "w+");

    fprintf(fp, "P6\n#Mandelbro\n%d %d\n%d\n", Width, Height, 0xFF);
    fwrite(bitmap, sizeof(char), Size, fp);

    fclose(fp);

    delete[] bitmap;

    return 1;
}