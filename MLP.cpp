#include "MLP.h"

Matrix average(Matrix matrix){
    Matrix avg(matrix.M, 1);
    for(int i = 0; i < matrix.M; i++){
        float sum = 0.0f;
        for(int j = 0; j < matrix.N; j++){
            sum += matrix.get(i, j);
        }   
        avg.get(i, 0) = sum / matrix.N;
    }
    return avg;
}

MLP::MLP(){
    L = {};
}

MLP::MLP(vector<int> list){
    for(int i = 0; i < list.size() - 1; i++){
        L.push_back(Layer(list[i + 1], list[i]));
    }
}

Matrix MLP::run(Matrix X){
    for(int i = 0; i < L.size() - 1; i++){
        X = L[i].pass(X, "RELU");
        L[i + 1].X = X;
    }

    return L[L.size() - 1].pass(X, "SOFT_MAX");
}

void MLP::train(Matrix& X, const vector<int>& Y, float alpha){
    // y' - y
    Matrix spine = run(X);      
    for(int j = 0; j < Y.size(); j++){
        spine.get(Y[j], j) -= 1;
    }

    Layer& l = L[L.size() - 1];
    l.W -= spine * l.X.T() * (alpha / X.N); 
    l.b -= average(spine * alpha);  

    // Backpropagate
    for(int k = L.size() - 1; k > 0; k--){
        spine = L[k].W.T() * spine;
        for(int j = 0; j < X.N; j++){
            for(int i = 0; i < L[k].X.M; i++){
                if(L[k].X.get(i, j) <= 0.0f){
                    spine.get(i, j) = 0.0f;
                }
            }
        }

        Layer& l = L[k - 1];
        Matrix input = (k > 1) ? l.X : X;
        l.W -= spine * input.T() * (alpha / X.N);
        l.b -= average(spine * alpha);  
    }
}

vector<int> MLP::Y(const Matrix& A){
    vector<int> list = {};
    for(int j = 0; j < A.N; j++){
        float max = std::numeric_limits<float>::lowest();
        int index;
        for(int i = 0; i < A.M; i++){
            float number = A.data[A.N * i + j];
            if(number > max){
                max = number;
                index = i;
            }
        }

        list.push_back(index);
    }
    return list;
}

void MLP::save(string path){
    int size = L.size();
    ofstream stream(path, ios::binary);
    stream.write((char*)&size, sizeof(size));

    for(Layer& l : L){
        l.encode(stream);
    }
}

void MLP::load(string path){
    int size = 0;
    ifstream stream(path, ios::binary);
    stream.read((char*)&size, sizeof(size));

    L = {};
    for(int i = 0; i < size; i++){
        Layer layer(1, 1);
        layer.decode(stream);
        L.push_back(layer);
    }
}