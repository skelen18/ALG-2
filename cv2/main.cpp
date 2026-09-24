#include <iostream>
#include <vector>

using std::vector;
using ScalarType = double;
using Matrix = vector<vector<ScalarType>>;

vector<ScalarType> solveAxb(Matrix A, vector<ScalarType> b) {
    size_t N = A.size();
    //implementace pivotů row a swappovani, ale nestihl jsem
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            if (A[j][i] != 0) {
                ScalarType factor = A[j][i] / A[i][i];
                for (size_t k = i; k < N; ++k) {
                    A[j][k] -= factor * A[i][k];
                }
                b[j] -= factor * b[i];
            }
        }
    }
    for (int i = N - 1; i >= 0; --i) {
        ScalarType sumFromRowsBelow = 0;
        for (size_t j = i + 1; j < N; ++j) {
            sumFromRowsBelow += A[i][j] * b[j];
        }
        b[i] -= sumFromRowsBelow;
        b[i] /= A[i][i];
    }
    return b;
}



void printVec(const vector<ScalarType>& vec) {
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << "\n";
    
}

void Test() {
    Matrix M = {{2,1}, {1,2}};
    vector<ScalarType> rhs = {1, 1};
    auto solution = solveAxb(M, rhs);
    printVec(solution);

}

int main() {

    Test();
    
	return 0;
}
