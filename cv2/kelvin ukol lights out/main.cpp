#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

using std::vector;

class Z2 {
public:
    int value;

    Z2(int v = 0) : value((v % 2 + 2) % 2) {}

    Z2 operator+(const Z2& other) const { return Z2(value + other.value); }
    Z2 operator-(const Z2& other) const { return Z2(value - other.value); }
    Z2 operator*(const Z2& other) const { return Z2(value * other.value); }
    Z2 operator/(const Z2& other) const {
        if (other.value == 0) {
            throw std::runtime_error("Division by zero in Z2.");
        }
        return Z2(value);
    }

    bool operator==(const Z2& other) const { return value == other.value; }
    bool operator!=(const Z2& other) const { return value != other.value; }
};

using Matrix = vector<vector<Z2> >;

Matrix sestavMatici(int n) {
    int celkem = n * n;
    Matrix A(celkem, vector<Z2>(celkem, 0));

    int dx[5] = {0, -1, 1, 0, 0};
    int dy[5] = {0, 0, 0, -1, 1};

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            int button_index = x + n * y;

            for (int d = 0; d < 5; ++d) {
                int nx = x + dx[d];
                int ny = y + dy[d];

                if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                    int affected_index = nx + n * ny;
                    A[affected_index][button_index] = 1;
                }
            }
        }
    }
    return A;
}
vector<Z2> solveAxb(Matrix A, vector<Z2> b) {
    int N = A.size();
    int aktRadek = 0;
    vector<int> pivotProSloupec(N, -1);

    for (int col = 0; col < N && aktRadek < N; ++col) {
        int vybranyRadek = -1;
        for (int r = aktRadek; r < N; ++r) {
            if (A[r][col] != 0) {
                vybranyRadek = r;
                break;
            }
        }
        if (vybranyRadek == -1) {
            continue;
        }

        std::swap(A[aktRadek], A[vybranyRadek]);
        std::swap(b[aktRadek], b[vybranyRadek]);
        pivotProSloupec[col] = aktRadek;

        for (int r = 0; r < N; ++r) {
            if (r != aktRadek && A[r][col] != 0) {
                for (int c = col; c < N; ++c) {
                    A[r][c] = A[r][c] - A[aktRadek][c];
                }
                b[r] = b[r] - b[aktRadek];
            }
        }
        aktRadek++;
    }

    vector<Z2> x(N, 0);
    for (int col = 0; col < N; ++col) {
        if (pivotProSloupec[col] != -1) {
            x[col] = b[pivotProSloupec[col]];
        }
    }
    return x;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Invalid number of arguments\n";
        return -1;
    }

    const int n = std::stoi(argv[1]);
    const int N = n * n;

    if (argc != N + 2) {
        std::cerr << "Invalid number of arguments\n";
        return -1;
    }

    vector<Z2> b;
    b.reserve(N);
    for (int i = 0; i < N; ++i) {
        b.push_back(Z2(std::stoi(argv[i + 2])));
    }
    Matrix A = sestavMatici(n);

    vector<Z2> x = solveAxb(A, b);
    for (int i = 0; i < N; ++i) {
        std::cout << x[i].value;
        if (i < N - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";

    return 0;
}