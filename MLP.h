#include "Layer.h"

class MLP {
    public: 
        vector<Layer> L; 
        MLP();
        MLP(vector<int> list);
        Matrix run(Matrix X);
        void train(Matrix& X, const vector<int>& Y, float alpha);
        static vector<int> Y(const Matrix& A);
        void save(string path);
        void load(string path);
};

// batch_norm