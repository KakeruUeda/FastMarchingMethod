#include <iostream>
#include "lib.h"
using namespace std;

int main(){
    FMM fmm;
    fmm.tmp=0;
    fmm.size=0;
    fmm.DefineGrid();
    fmm.FastMarchingMethod();
}