#include <bits/stdc++.h>
using namespace std;
#define X 3 
#define Y 4 
#define Z 5
#define ARR(i,j,k) (b[Y*Z*i + Z*j + k])
int main() {
    int *b = (int *)calloc(X*Y*Z, sizeof(int));
    int count = 0;
    for(int i = 0; i < X; i++) {
        for(int j = 0; j < Y; j++) {
            for(int k = 0; k < Z; k++) {
                ARR(i, j, k) = count++;
            }
        }
    }
    for(int i = 0; i < X*Y*Z; i++) {
        std::cout << b[i] << " ";
    }
}
