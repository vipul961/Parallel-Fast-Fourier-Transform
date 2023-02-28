#include <iostream>
#include <complex>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;
typedef complex<double> cx;
const double PI = 3.1415926536;
cx J{ 0,1 }, WW;

void fft(vector<cx>& a, vector<cx>& b,int i, int n, int n1, int n2);
unsigned int bitReverse(unsigned int x, int size);
void fft_recursive(vector<cx>& a);

int main() {

    
    ifstream In("input1024.txt");//Use the right file name described in HW2 
    if (!In) {
        cout << "Fail to open file" << endl;
        return 0;
    }

    vector<cx> a,a1;
    //vector<cx> b(a.size());
    
    double d1;
   
    while (In >> d1) {
        a.emplace_back(cx(d1));
        a1.emplace_back(cx(d1));
    }
    In.close();
    fft_recursive(a1);
    vector<cx> b(a.size());
    

    //Block for iterative FFT using 4 threads
    {
        int count = 0;
        int n = log2(int(a.size()));
        for (int i = 2; i <= a.size(); i *= 2) {
            fft(a, b, i, n,0,a.size()-1);
            cout << ++count << endl;
        }
    }

    ofstream Out("output_data_1024_recu.txt");//Use the right file name described in HW2
    ofstream Out1("output_data_1024.txt");//Use the right file name described in HW2
    if (!Out) {
        cout << "Fail to access output file." << endl;
        return 0;
    }

    for (int i = 0; i < a.size(); ++i) {
        Out << a[i] << "\n";
        Out1 << b[i] << "\n";
    }

    Out.close();
    Out1.close();
    return 0;
    
}
void fft(vector<cx>& a, vector<cx>& b, int i,int n,int n1,int n2){
    if (i == a.size() || i == a.size() / 2) {
        int k = 0;
        for (int l = n1; l < n2; l++) {
            WW = exp(cx(-2) * cx(PI) * J * cx(k++) / cx(i));
            //cout << "value of W is : " << real(WW) << endl;
            b[l] = a[bitReverse(l , n)] + a[bitReverse(l + i / 2, n)] * WW;
            b[l + i / 2] = a[bitReverse(l , n)] - (a[bitReverse(l + i / 2, n)] * WW);
        }
    }
    else {
        for (int l = n1; l < n2; l += i) {
            for (int k = 0; k < i / 2; k++) {
                if (i == 2) {
                    //cout << bitReverse(l + k, n) << "  " << bitReverse(l + k + i / 2, n);
                    WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                    //cout << "value of W is : " << real(WW) << endl;
                    b[l + k] = a[bitReverse(l + k, n)] + a[bitReverse(l + k + i / 2, n)] * WW;
                    b[l + k + i / 2] = a[bitReverse(l + k, n)] - (a[bitReverse(l + k + i / 2, n)] * WW);
                }
                else {
                    //cout << bitReverse(l + k, n) << "  " << bitReverse(l + k + i / 2, n);
                    WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                    //cout << "value of W is : " << real(WW) << endl;
                    b[l + k] = a[l + k] + a[l + k + i / 2] * WW;
                    b[l + k + i / 2] = a[l + k] - (a[l + k + i / 2] * WW);
                }
            }
            //cout << endl;
        }
    }
    a = b;
    //cout << endl;
}

unsigned int bitReverse(unsigned int x, int size) {
    unsigned int y = 0;
    for (int i = 0; i < size; i++) {
        unsigned int a = (x & 1);
        //cout<<"Value of x is : "<<x<<endl;
        y += a;
        //cout<<"Value of y is : "<<y<<endl;
        x = x >> 1;
        //cout<<"Value of a is : "<<a<<endl;
        y = y << 1;
        //cout<<"Value of y is : "<<y<<endl;
    }
    return y >>= 1;
}

void fft_recursive(vector<cx>& a) {
    if (a.size() <= 1) {
        return;
    }
    vector<cx>odd_num,even_num;
    for (int i = 0; i < a.size(); i++) {
        if ((i & 1) == 0) {
            even_num.push_back(a[i]);
        }
        else {
            odd_num.push_back(a[i]);
        }
    }
    fft_recursive(even_num);
    fft_recursive(odd_num);
    if (a.size() >= 2) {
        for (int k = 0; k < a.size()/2; k++) {
            //cout << "for loop a size: " << a.size()<<endl;
            cx WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(a.size()));
            a[k] = even_num[k] + odd_num[k] * WW;
            a[k +a.size()/2] = even_num[k] - odd_num[k] * WW;
            //cout << a[k] << "  "<<a[k +a.size()/2];
        }
        //cout << endl;
    }
}



