#include <iostream>
#include "MyGA.h"
#include <cmath>
#include <complex>
#include <format>
int main() {
    auto test_func=[](double x){return sin(x);};
    double x_low=0, x_up=3.14;
    MyGA<double> ga{test_func};
    auto [x,y]=ga.solve(x_low,x_up,0.001,400,4000);
    std::cout<<std::format("x={},y={} 2*x={}\n",x,y, 2*x);
    int64_t g;
    std::complex<double> c1{1.2,3.4};
    auto c2=c1*c1;
    std::cout<<c2<<std::endl;
    std::cout<<"Test";

    return 0;
}