#include <iostream>
#include <vector>
#include <string>

using std::vector, std::cout;

class Z2 {
public:
    int value;

    Z2(int v = 0) {
        value = (v % 2 + 2) % 2;

    }

    Z2 operator+(const Z2& other) const { return Z2(value + other.value); }
    Z2 operator-(const Z2& other) const { return Z2(value - other.value); }

    Z2 operator*(const Z2& other) const { return Z2(value * other.value); }

    bool operator==(const Z2& other) const { return value == other.value; }
    bool operator!=(const Z2& other) const { return value != other.value; }
};


using ScalarType = Z2;
using Matrix = vector<vector<ScalarType> >;

Matrix naplnMatici(int n) {

    int celkem = n * n;

    Matrix mat(celkem, vector<ScalarType>(celkem, 0));

    int dx[5] = {0, -1, 1, 0, 0};
    int dy[5] = {0, 0, 0, -1, 1};

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            int btn = x + n * y;

            for (int s = 0; s < 5; s++) {
                int nx = x + dx[s];
                int ny = y + dy[s];

                if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                    int cil = nx + n * ny;
                    mat[cil][btn] = 1;
                }
            }
        }
    }

    return mat;

}

vector<ScalarType> reseniAxb(Matrix A, vector<ScalarType> b) {
    int N = A.size();
    int radek = 0;

    vector<int> pivotRadek(N, -1);

    for (int col = 0; col < N && radek < N; col++) {
        int pivot = -1;
        for (int i = radek; i < N; i++) {
            if (A[i][col].value == 1) {
                pivot = i;
                break;
            }
        }

        if (pivot == -1) {
            continue;
        }

        std::swap(A[radek], A[pivot]);
        std::swap(b[radek], b[pivot]);
        pivotRadek[col] = radek;

        for (int i = 0; i < N; i++) {
            if (i != radek && A[i][col].value == 1) {
                for (int j = col; j < N; j++) {
                    A[i][j] = A[i][j] - A[radek][j];
                }
                b[i] = b[i] - b[radek];
            }
        }
        radek++;
    }

    vector<ScalarType> vysledek(N, 0);
    for (int c = 0; c < N; c++) {
        if (pivotRadek[c] != -1) {
            vysledek[c] = b[pivotRadek[c]];
        }
    }
    return vysledek;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return -1;
    }

    int n = std::stoi(argv[1]);
    int N = n * n;

    if (argc != N + 2) {
        return -1;
    }

    vector<ScalarType> rhs;
    for (int i = 0; i < N; i++) {
        rhs.push_back(ScalarType(std::stoi(argv[i + 2])));
    }

    Matrix mat = naplnMatici(n);
    
    vector<ScalarType> res = reseniAxb(mat, rhs);

    for (int i = 0; i < N; i++) {
        cout << res[i].value << " ";
    }

    cout << "\n";

    return 0;
}