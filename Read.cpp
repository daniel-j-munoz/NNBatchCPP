#include "Read.h"
#include "Write.h"

template<typename T>
T read(ifstream& f) {
    uint8_t thing[sizeof(T)];
    f.read((char*)thing, sizeof(T));
    T value = 0;
    for (int i = 0; i < sizeof(T); i++) {
        value = (value << 8) | thing[i];
    }
    return value;
}

vector<vector<float>> gradients(int m, int n, const vector<float>& list){
    vector<vector<float>> output = {};

    vector<float> sobel_x = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };

    vector<float> sobel_y = {
        1, 2, 1,
        0, 0, 0,
        -1, -2, -1
    };

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            float gx = 0.0f;
            float gy = 0.0f;
            for(int k = -1; k <= 1; k++){
                for(int h = -1; h <= 1; h++){ 
                    if(
                        (i + k) >= 0 && (i + k) < m && 
                        (j + h) >= 0 && (j + h) < n
                    ){
                        gx += sobel_x[3 * (k + 1) + (h + 1)] * list[n * (i + k) + (j + h)];
                        gy += sobel_y[3 * (k + 1) + (h + 1)] * list[n * (i + k) + (j + h)];
                    }
                }
            }
            output.push_back({gx, gy});
        }
    }

    return output;
}


vector<Matrix> Read::X(string path, int batch_size){
    ifstream stream(path, ios::binary);

    int magic = read<int>(stream);
    int I = read<int>(stream);
    int m = read<int>(stream);
    int n = read<int>(stream);

    vector<Matrix> batches = {};
    for(int k = 0; k < I / batch_size; k++){

        vector<float> data(m * n * batch_size);
        for(int j = 0; j < batch_size; j++){
            for(int i = 0; i < m * n; i++){
                unsigned char number = read<unsigned char>(stream); // unsigned?
                data[batch_size * i + j] = (float)(number);
            }
        }


        // subtract mean, divide std
        for(int i = 0; i < m * n; i++){
            float mean = 0.0f;
            for(int j = 0; j < batch_size; j++){
                mean += data[batch_size * i + j];
            }   
            mean /= batch_size;

            float var = 0.0f;
            for(int j = 0; j < batch_size; j++){
                var += pow(data[batch_size * i + j] - mean, 2);
            }
            var /= batch_size;

            // norm
            for(int j = 0; j < batch_size; j++){
                data[batch_size * i + j] -= mean;
                data[batch_size * i + j] /= sqrt(var + 1e-8); 
            } 
        }

        batches.push_back(Matrix(m * n, batch_size, data));
    }

    return batches;
}


vector<vector<int>> Read::Y(string path, int batch_size){
    ifstream stream(path, ios::binary);
    int magic = read<int>(stream);
    int I = read<int>(stream);

    vector<vector<int>> Y = {};
    for(int i = 0; i < I / batch_size; i++){
        vector<int> y = {};
        for(int j = 0; j < batch_size; j++){
            unsigned char number = read<unsigned char>(stream);
            y.push_back((int)number);
        }
        Y.push_back(y);
    }
    return Y;
}
