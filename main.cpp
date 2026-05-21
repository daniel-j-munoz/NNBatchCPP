#include "MLP.h"
#include "Read.h"

// MNIST example

int main(){
    float rate = 0.01f;
    int epoch_size = 1;
    int batch_size = 100; 
    
    string train;
    cout << "train?\n";
    cin >> train;
    if(train == "y"){
        vector<Matrix> X = Read::X("data/train/images", batch_size);
        vector<vector<int>> Y = Read::Y("data/train/labels", batch_size);
        MLP mlp({784, 256, 128, 10});
        while(epoch_size--){
            for(int i = 0; i < X.size(); i++){ 
                cout << to_string(epoch_size) << " " << to_string(i) << "\n";
                mlp.train(X[i], Y[i], rate); 
            }
        }
        mlp.save("saved/angle_invariant");
    } else {
        MLP mlp;
        mlp.load("saved/model");

        vector<Matrix> X = Read::X("data/test/images", batch_size);
        vector<vector<int>> Y = Read::Y("data/test/labels", batch_size);

        float correct = 0.0f;
        for(int batch = 0; batch < X.size(); batch++){
            vector<int> P = MLP::Y(mlp.run(X[batch]));
            for(int j = 0; j < batch_size; j++){
                if(Y[batch][j] == P[j]){
                    correct++;
                }
            }
        }

        cout << "score: " << to_string(correct / (X.size() * batch_size) * 100) << "%" << "\n";
    }
    return 0;
}
