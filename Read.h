#include "Matrix.h"

class Read {
    public: 
        static vector<Matrix> X(string path, int batch_size);
        static vector<vector<int>> Y(string path, int batch_size);

};