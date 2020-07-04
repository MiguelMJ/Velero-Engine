#include "Random.hpp"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char** argv){
    const unsigned N = 10000;
    
    int s;
    cout << "Enter seed: "; cin >> s;
    ge::Random::seed(s);
    
    vector<double> v;
    v.reserve(N);
    
    double max=-1, i_max=-1;
    double min=101, i_min=-1;
    double acc=0;
    
    for(int i=0; i < N; i++){
        double d = ge::Random::rndDouble(0,100);
        acc += d;
        v.push_back(d);
        
        if(d > max){
            i_max = i;
            max = d;
        }
        if(d < min){
            i_min = i;
            min = d;
        }
    }
    double mean = acc/N;
    acc = 0;
    for(double d: v){
        acc += abs(mean-d);
    }
    double var = acc/N;
    
    cout << "max: " << max << " at " << i_max << endl;
    cout << "min: " << min << " at " << i_min << endl;
    cout << "mean: " << mean << endl;
    cout << "var': " << var << endl;
    
    return 0;
}
