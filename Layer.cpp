#include "Layer.h"

void normal(Matrix& A, float mean, float std){
    static random_device rd;
    static mt19937 gen(rd());
    normal_distribution<float> dis(mean, std);
    for(int i = 0; i < A.data.size(); i++){
        A.data[i] = dis(gen); 
    }
}

Layer::Layer(int n, int m) : W(n, m), b(n, 1, 0.0f){
    normal(W, 0, sqrt(2.0f / (m + n))); // this is for tanh?... we need it for relu?...
}

Matrix RELU(Matrix matrix){
    for(float& number : matrix.data){
        number = max(number, 0.0f);
    }
    return matrix;
}

Matrix SOFT_MAX(Matrix matrix){
    for(int j = 0; j < matrix.N; j++){
        float max = std::numeric_limits<float>::lowest();
        for(int i = 0; i < matrix.M; i++){
            if(matrix.get(i, j) > max){
                max = matrix.get(i, j);
            }
        }

        float sum = 0.0f;
        for(int i = 0; i < matrix.M; i++){
            matrix.get(i, j) = exp(matrix.get(i, j) - max);
            sum += matrix.get(i, j);
        }

        for(int i = 0; i < matrix.M; i++){
            matrix.get(i, j) /= sum;
        }
    }

    return matrix;
}

Matrix Layer::pass(Matrix X, string function){
    Matrix Z = W * X;
    // Matrix Z = W * X + b;

    // brodcast bias.      broadcast method?...
    for(int j = 0; j < Z.N; j++){
        for(int i = 0; i < Z.M; i++){
            Z.get(i, j) += b.get(i, 0);
        }
    }

    

    if(function == "RELU"){
        for(int i = 0; i < Z.M; i++){
            float mean = 0.0f;
            for(int j = 0; j < Z.N; j++){
                mean += Z.get(i, j);
            }   
            mean /= Z.N;

            float var = 0.0f;
            for(int j = 0; j < Z.N; j++){
                var += pow(Z.get(i, j) - mean, 2);
            }
            var /= Z.N;

            // norm
            for(int j = 0; j < Z.N; j++){
                Z.get(i, j) -= mean;
                Z.get(i, j) /= sqrt(var + 1e-8); 
            } 
        }

        return RELU(Z);
    } else if(function == "SOFT_MAX"){
        return SOFT_MAX(Z);
    } else { 
        throw std::invalid_argument("function \"" + function + "\" not found");
    }   
}



void Layer::encode(ofstream& stream){
    W.encode(stream);
    b.encode(stream); 
}

void Layer::decode(ifstream& stream){
    W.decode(stream);
    b.decode(stream); 
}