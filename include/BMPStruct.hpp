#include "main.hpp"

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t fileType;        // File type always 'BM' which is 0x4D42
    uint32_t fileSize;        // Size of the file (in bytes)
    uint16_t reserved1;       // Reserved, always 0
    uint16_t reserved2;       // Reserved, always 0
    uint32_t offsetData;      // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size;            // Size of this header (in bytes)
    int32_t width;            // Width of the bitmap in pixels
    int32_t height;           // Height of the bitmap in pixels
    uint16_t planes;          // Number of color planes, must be 1
    uint16_t bitCount;        // Number of bits per pixel
    uint32_t compression;     // Compression type (0 = uncompressed)
    uint32_t imageSize;       // Size of the image data (can be 0 for uncompressed images)
    int32_t xPixelsPerMeter;  // Horizontal resolution (pixels per meter)
    int32_t yPixelsPerMeter;  // Vertical resolution (pixels per meter)
    uint32_t colorsUsed;      // Number of colors in the color palette
    uint32_t colorsImportant; // Important colors (0 = all colors are important)
};

#pragma pack(pop)