#include "Matrix.h"

Matrix::Matrix(){

}

/**
 *  @param values row major order 
 * */
Matrix::Matrix(int M, int N, const vector<float>& values){
    this->data = {};
    this->M = M; 
    this->N = N;
    for(int ith = 0; ith < M * N; ith++){
        data.push_back(values[ith]);
    }
}

/** @param value init w/ value */
Matrix::Matrix(int M, int N, float value){
    this->data = {};
    this->M = M; 
    this->N = N;
    for(int i = 0; i < M * N; i++){
        data.push_back(value);
    }
}

/**
 * Identity Matrix
 */
Matrix::Matrix(int M, int N){
    this->data = {};
    this->M = M; 
    this->N = N;
    data = vector<float>(M * N, 0.0f);
    for(int m = 0; m < M; m++){
        for(int n = 0; n < N; n++){
            int ith = N * m + n;
            if(m == n){
                data[ith] = 1.0f;
            } else {
                data[ith] = 0.0f; 
            }
        }
    }
}

// ............. 2D ..................................... 

// void or copy?
Matrix Matrix::operator*(float scale){
    Matrix output = *this;
    output.scale(scale);
    return output;
}

Matrix Matrix::operator*=(float scale){
    *this = *this * scale;
    return *this;
}

// replace scale method w/ operator instead?
void Matrix::scale(float scalar){
    for(int ith = 0; ith < data.size(); ith++){
        data[ith] *= scalar;
    }
}

Matrix Matrix::operator*(const Matrix& other) {
    if (N != other.M) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    Matrix result(M, other.N, 0);
    
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < other.N; n++) {
            float dot_product = 0.0f;
            
            for (int i = 0; i < N; i++) {
                dot_product += get(m, i) * other.data[i * other.N + n];
            }
            


            result.get(m, n) = dot_product;
        }
    }
    
    return result;
}

Matrix Matrix::operator*=(const Matrix& other){
    *this = *this * other;
    return *this;
}

Matrix Matrix::T() {
    vector<float> temp(M * N, 0);
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            temp[M * n + m] = get(m, n);
        }
    }

    return Matrix(N, M, temp);
}


Matrix Matrix::element_wise(const Matrix& other, char operation){

    // Exception
    if(!(M == other.M && N == other.N)){
        cout << "Size Mismatch "; 
        cout << M << ", " << N << " & " << other.M << ", " << other.N << "\n";
        throw invalid_argument("");
    }

    // Element Wise
    vector<float> new_data = {};
    for(int ith = 0; ith < other.data.size(); ith++){
        if(operation == '*'){
            new_data.push_back(this->data[ith] * other.data[ith]);
        } else if(operation == '+'){
            new_data.push_back(this->data[ith] + other.data[ith]);
        } else if(operation == '-'){
            new_data.push_back(this->data[ith] - other.data[ith]);
        } else if(operation == '/'){
            new_data.push_back(this->data[ith] / other.data[ith]);
        }
        // exponent? 
    }

    return Matrix(M, N, new_data);
}

Matrix Matrix::operator+(const Matrix& other){
    return element_wise(other, '+');
}

Matrix Matrix::operator+=(const Matrix& other){
    *this = *this + other;
    return *this;
}

Matrix Matrix::operator-(const Matrix& other){
    return element_wise(other, '-');
}

Matrix Matrix::operator-=(const Matrix& other){
    *this = *this - other;
    return *this;
}

// any size. just need same # of elements and stuff...
float Matrix::dot(Matrix& other){
    float sum = 0.0f;
    for(int i = 0; i < data.size(); i++){
        sum += other.data[i] * data[i];
    }
    return sum;
}


// .............................. get methods? ............................................................................................
float& Matrix::get(int m, int n) {
    return data[N * m + n];
}

float& Matrix::get(int i){
    return data[i];
}



void Matrix::print(int x) const {
    cout << fixed << setprecision(x);

    for(int m = 0; m < M; m++){
        for(int n = 0; n < N; n++){
            cout << data[N * m + n] << " ";
        }
        cout << "\n";
    }
    cout << defaultfloat; 
    cout << "\n";
}

void Matrix::encode(ofstream& ofs){
    ofs.write((char*)&M, sizeof(M));
    ofs.write((char*)&N, sizeof(N));
    ofs.write((char*)data.data(), data.size() * sizeof(float));
}


void Matrix::decode(ifstream& ifs){
    ifs.read((char*)&M, sizeof(M));
    ifs.read((char*)&N, sizeof(N));
    data.resize(M * N);
    ifs.read((char*)data.data(), sizeof(float) * data.size());
}

    // .data() pointer to start of array?... hm...







void Matrix::set(int t, int l, int b, int r, const Matrix& that){
    for(int i = t; i <= b; i++){
        for(int j = l; j <= r; j++){
            get(i, j) = that.data[(i - t) * that.N + (j - l)];
        }
    }
}

void Matrix::set_columns(int l, int r, const Matrix& that){
    set(0, l, that.M, r, that);
}




// generalizes add col and add row?....

// stacks vertically by default?
void Matrix::stack_down(const Matrix& that){
    for(float number : that.data){
        data.push_back(number);
    }

    M += that.M;
}


void Matrix::stack_right(const Matrix& that){
    vector<float> data;
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            data.push_back(get(i, j));
        }
        
        for(int J = 0; J < that.N; J++){
            data.push_back(that.data[i * that.N + J]);
        }
    }

    N += that.N;
    this->data = data;
}






Matrix Matrix::column(int n){
    vector<float> list = {};
    for(int i = 0; i < M; i++){
        list.push_back(get(i, n));
    }
    return Matrix(M, 1, list);
}

void Matrix::column_set(const Matrix& other, int j){
    for(int i = 0; i < M; i++){
        get(i, j) = other.data[i];
    }
}




// vector<Matrix> Matrix::list_columns(int a, int b){
//     // vector<Matrix> list = {};
//     // for(int i = a; i <= b; i++){
//     //     list.push_back(get(0, i, M, i));
//     // }
// }