#include <bits/stdc++.h>
using namespace std;

int main() {
    int **a = (int **)calloc(2, sizeof(int *));
    for(int i = 0; i < 2; i++) {
        a[i] = (int *)calloc(5, sizeof(int));
        for(int j = 0; j < 5; j++) {
            a[i][j] = (i+1)*5 + j+1;
            cout << a[i][j] << endl;
        }
    }
    cout << a[1][4];
}
