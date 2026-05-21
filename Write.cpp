#include "Write.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Write::save(int m, int n, vector<float>& data, string path){
    unsigned char* new_data = new unsigned char[m * n * 3];
    for(int i = 0; i < m * n; i++){
        float v = data[i];
        if(v < 0) v = 0;
        if(v > 255) v = 255;

        unsigned char pixel = (unsigned char)(v);
        int idx = i * 3;
        new_data[idx + 0] = pixel; // R
        new_data[idx + 1] = pixel; // G
        new_data[idx + 2] = pixel; // B
    }

    stbi_write_png(path.c_str(), n, m, 3, new_data, n * 3);
    delete[] new_data;
}