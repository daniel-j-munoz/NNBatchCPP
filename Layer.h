#include "Matrix.h"

class Layer {
    public: 
        Matrix W;
        Matrix b;
        Matrix X;  

        Layer(int N, int M);
        Matrix pass(Matrix x, string function);

        void encode(ofstream& stream);
        void decode(ifstream& stream);
};

