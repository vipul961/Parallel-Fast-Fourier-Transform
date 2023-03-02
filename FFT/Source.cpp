//Assignment 2 FFT implementation using 4 threads
//Sawant, Vipul
//SU ID: 347120278
#include <iostream>
#include <complex>
#include <math.h>
#include <vector>
#include <fstream>
#include <thread>
//#include <mutex>


using namespace std;
using namespace chrono;
using namespace literals::chrono_literals;


typedef complex<double> cx;
const double PI = 3.1415926536;
cx J{ 0,1 };//, WW;

void fft_iterative(vector<cx>& a, vector<cx>& b);
void fft(vector<cx>& a, vector<cx>& b,int i, int n, int n1, int n2);
unsigned int bitReverse(unsigned int x, int size);
void fft_recursive(vector<cx>& a);


int main() {
    //place input file and change input size in below variable to read data from file.
    int val = 1024;   //change input value here to read from file name: ex val = 64 represents filename:input64.txt

    ifstream In("input" + to_string(val) + ".txt");//Use the right file name described in HW2 
    
    if (!In) {
        cout << "Fail to open file" << endl;
        return 0;
    }

    vector<cx> a,a1;
    vector<cx> b(a.size());
    

    double d1;
   
    while (In >> d1) {
       a.emplace_back(cx(d1));
       a1.emplace_back(cx(d1));
    }
    In.close();

    //Recursive FFT implementation
    //Block for recursive FFT using 4 threads
    {
        //fft_recursive(a1);
    }
    
    //Iterative FFT implementation
    //Block for iterative FFT using 4 threads
    {
        fft_iterative(a, b);
    }

    
    ofstream Out("output_data_" + to_string(val) + "_recursive.txt");//Use the right file name described in HW2
    ofstream Out1("output_data_" + to_string(val) + ".txt");//Use the right file name described in HW2
    if (!Out) {
        cout << "Fail to access output file." << endl;
        return 0;
    }

    for (int i = 0; i < a.size(); ++i) {
        //Out << a1[i] << "\n";
        Out1 << b[i] << "\n";
    }

    Out.close();
    Out1.close();
    return 0;
    
}

void fft_iterative(vector<cx>& a, vector<cx>& b) {
    int count = 0;
    int n = log2(int(a.size()));
    for (int i = 2; i <= a.size(); i *= 2) {
        if (a.size() <= 8) {
            fft(a, b, i, n, 0, a.size() - 1);
        }
        else {
            if (i == a.size()) {
                thread T0(fft, ref(a), ref(b), i, n, 0, int(a.size()) / 8 - 1);
                thread T1(fft, ref(a), ref(b), i, n, int(a.size()) / 8, int(a.size()) / 4 - 1);
                thread T2(fft, ref(a), ref(b), i, n, int(a.size()) / 4, int(a.size() / 8) * 3 - 1);
                fft(a, b, i, n, int(a.size() / 8) * 3, int(a.size() / 2) - 1);
                if (T0.joinable())T0.join();
                if (T1.joinable())T1.join();
                if (T2.joinable())T2.join();
            }
            else if (i == a.size() / 2) {
                thread T0(fft, ref(a), ref(b), i, n, 0, int(a.size()) / 8 - 1);
                thread T1(fft, ref(a), ref(b), i, n, int(a.size()) / 8, int(a.size()) / 4 - 1);
                thread T2(fft, ref(a), ref(b), i, n, int(a.size()) / 2, int(a.size()) / 2 + int(a.size()) / 8 - 1);
                fft(a, b, i, n, int(a.size()) / 2 + int(a.size()) / 8, int(a.size() / 4) * 3 - 1);
                if (T0.joinable())T0.join();
                if (T1.joinable())T1.join();
                if (T2.joinable())T2.join();
            }
            else {
                thread T0(fft, ref(a), ref(b), i, n, 0, int(a.size()) / 4 - 1);
                thread T1(fft, ref(a), ref(b), i, n, int(a.size()) / 4, int(a.size()) / 2 - 1);
                thread T2(fft, ref(a), ref(b), i, n, int(a.size()) / 2, int(a.size() / 4) * 3 - 1);
                fft(a, b, i, n, int(a.size() / 4) * 3, int(a.size() - 1));
                if (T0.joinable())T0.join();
                if (T1.joinable())T1.join();
                if (T2.joinable())T2.join();
            }
        }
        a = b;
    }
}
void fft(vector<cx>& a, vector<cx>& b, int i,int n,int n1,int n2){
    
    if (a.size() <= 8) {
        for (int l = n1; l < n2; l += i) {
            for (int k = 0; k < i / 2; k++) {
                if (i == 2) {
                    //cout << l + k << "  " << l + k + i / 2<<"## ";
                    cx WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                    //cout << "value of W is : " << real(WW) << endl;
                    
                    b[l + k] = a[bitReverse(l + k, n)] + a[bitReverse(l + k + i / 2, n)] * WW;
                    b[l + k + i / 2] = a[bitReverse(l + k, n)] - (a[bitReverse(l + k + i / 2, n)] * WW);
                }
                else {
                    //cout << l + k<< "  " << l + k + i / 2<<"**  ";
                    cx WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                    //cout << "value of W is : " << real(WW) << endl;
                    b[l + k] = a[l + k] + a[l + k + i / 2] * WW;
                    b[l + k + i / 2] = a[l + k] - (a[l + k + i / 2] * WW);
                }
            }
            //cout << endl;
        }
    }
    else {

        if (i == a.size() || i == a.size() / 2) {
            for (int l = n1; l <= n2; l++) {
                cx WW = exp(cx(-2) * cx(PI) * J * cx(l%i) / cx(i));
                //cout << l << " " << l + i / 2 << "  WW: "<<l%i<<" ";
                //cout << "value of W is : " << real(WW) << endl;
                b[l] = a[l] + a[l + i / 2] * WW;
                b[l + i / 2] = a[l] - (a[l + i / 2] * WW);
            }
            //cout << endl;
        }
        else {
            for (int l = n1; l < n2; l += i) {
                for (int k = 0; k < i / 2; k++) {
                    if (i == 2) {
                        cx WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                        //cout << l + k << " " << l + k + i / 2 << "  WW: "<<k<<" ";
                        //cout << "value of W is : " << real(WW) << endl;
                        b[l + k] = a[bitReverse(l + k, n)] + a[bitReverse(l + k + i / 2, n)] * WW;
                        b[l + k + i / 2] = a[bitReverse(l + k, n)] - (a[bitReverse(l + k + i / 2, n)] * WW);
                    }
                    else {
                        cx WW = exp(cx(-2) * cx(PI) * J * cx(k) / cx(i));
                        //cout << l + k << " " << l + k + i / 2 << "   WW: "<<k<<" ";
                        //cout << "value of W is : " << real(WW) << endl;
                        b[l + k] = a[l + k] + a[l + k + i / 2] * WW;
                        b[l + k + i / 2] = a[l + k] - (a[l + k + i / 2] * WW);
                    }
                }
            }
            //cout << endl;
        }
    }
}

unsigned int bitReverse(unsigned int x, int size) {
    unsigned int y = 0;
    for (int i = 0; i < size; i++) {
        unsigned int a = (x & 1);
        //cout<<"Value of x is : "<<x<<endl;
        y += a;
        //cout<<"Value of y is : "<<y<<endl;
        x >>= 1;
        //cout<<"Value of a is : "<<a<<endl;
        y <<= 1;
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

//hello changes



