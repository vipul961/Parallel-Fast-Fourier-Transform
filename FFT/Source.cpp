#include <iostream>
#include <complex>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;
typedef complex<double> cx;
const double PI = 3.1415926536;

void fft(vector<cx>& a, vector<cx>& b);
unsigned int bitReverse(unsigned int x, int size);


int main() {

    typedef complex<double> cx;
    //cx J{ 0,1 };
    //cx WW = exp(cx(-2) * cx(PI) * J * cx(0) / cx(16));
    //cout << real(WW);
    
    ifstream In("input1024.txt");//Use the right file name described in HW2 
    if (!In) {
        cout << "Fail to open file" << endl;
        return 0;
    }

    vector<cx> a;
    double d1;
   
    while (In >> d1) {
        a.emplace_back(cx(d1));
    }
    In.close();
    for (auto i : a) {
        cout << real(i) << endl;
    }

    vector<cx> b(a.size());
    int n = log2(int(a.size()));
    cx J{ 0,1 }, WW;

    for (int i = 2; i <= a.size(); i *= 2) {
            for (int l = 0; l < a.size(); l += i) {
                for (int k = 0; k < i/2; k++) {
                    if (i == 2) {
                        cout << bitReverse(l + k, n) << "  " << bitReverse(l + k + i / 2, n);
                        WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                        b[l + k] = a[bitReverse(l + k, n)] + a[bitReverse(l + k + i / 2, n)] * WW;
                        b[l + k + i / 2] = a[bitReverse(l + k, n)] - (a[bitReverse(l + k + i / 2, n)] * WW);
                    }
                    else {
                        cout << bitReverse(l + k, n) << "  " << bitReverse(l + k + i / 2, n);
                        WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                        b[l + k] = a[l + k] + a[l + k + i / 2] * WW;
                        b[l + k + i / 2] = a[l + k] - (a[l + k + i / 2] * WW);
                    }
                }
                cout << endl;
            }
            a = b;
        cout << endl;
    }
    //fft(a, b, n);

    ofstream Out("output_data_1024.txt");//Use the right file name described in HW2
    if (!Out) {
        cout << "Fail to access output file." << endl;
        return 0;
    }

    for (int i = 0; i < a.size(); ++i)
        Out << b[i] << "\n";

    Out.close();
    return 0;
    /*    
    
    */
}
void fft(vector<cx>& a, vector<cx>& b){

}

unsigned int bitReverse(unsigned int x, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result <<= 1;
        result |= (x & 1);
        x >>= 1;
    }
    return result;
    
}

void fft_recursive(vector<cx>& a, vector<cx>& b) {

}



