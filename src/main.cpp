#include "../include/main.hpp"

int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    string av = argv[1];
    Object* obj = new Object(av);
    obj->renderer();
    return 0;
}

bool loadBMP(const std::string& filename, int& width, int& height, std::vector<unsigned char>& data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open BMP file " << filename << std::endl;
        return false;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // Read the file header
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (fileHeader.fileType != 0x4D42) { // Check that this is a BMP file by file signature 'BM'
        std::cerr << "Not a valid BMP file" << std::endl;
        return false;
    }

    // Read the DIB header
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    cout << infoHeader.bitCount << " " << infoHeader.compression << endl;
    // Check that it's a 24-bit BMP (no compression)
    if (infoHeader.bitCount != 24 || infoHeader.compression != 0) {
        std::cerr << "Only uncompressed 24-bit BMP files are supported" << std::endl;
        return false;
    }

    width = infoHeader.width;
    height = infoHeader.height;
    int rowStride = (width * 3 + 3) & ~3; // BMP rows are padded to multiples of 4 bytes

    data.resize(rowStride * height);
    file.seekg(fileHeader.offsetData, std::ios::beg); // Move to the start of the pixel data
    file.read(reinterpret_cast<char*>(data.data()), data.size());

    // Convert BGR to RGB format and remove padding
    std::vector<unsigned char> rgbData;
    rgbData.reserve(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * rowStride + x * 3;
            rgbData.push_back(data[index + 2]); // R
            rgbData.push_back(data[index + 1]); // G
            rgbData.push_back(data[index]);     // B
        }
    }

    data = std::move(rgbData);
    file.close();
    return true;
}


void calculateUVsForTriangles(std::vector<float>& vertices) {
    size_t vertexCount = vertices.size() / 8; // Assuming 8 components (x, y, z, r, g, b, u, v)

    for (size_t i = 0; i < vertexCount; i += 3) {
        // Calculate UVs based on the vertex positions in the XY plane
        float u1 = (vertices[i * 8 + 0] + 1.0f) * 0.5f;
        float v1 = (vertices[i * 8 + 1] + 1.0f) * 0.5f;

        float u2 = (vertices[(i + 1) * 8 + 0] + 1.0f) * 0.5f;
        float v2 = (vertices[(i + 1) * 8 + 1] + 1.0f) * 0.5f;

        float u3 = (vertices[(i + 2) * 8 + 0] + 1.0f) * 0.5f;
        float v3 = (vertices[(i + 2) * 8 + 1] + 1.0f) * 0.5f;

        // Store the UVs back into the vertex buffer
        vertices[i * 8 + 6] = u1;
        vertices[i * 8 + 7] = v1;
        vertices[(i + 1) * 8 + 6] = u2;
        vertices[(i + 1) * 8 + 7] = v2;
        vertices[(i + 2) * 8 + 6] = u3;
        vertices[(i + 2) * 8 + 7] = v3;
    }
}