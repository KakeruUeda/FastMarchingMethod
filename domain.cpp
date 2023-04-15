#include "lib.h"
using namespace std;
void FMM::DefineGrid(){

    nx = 6;
    ny = 6;
    Lx = 1e0;
    Ly = 1e0;
    goal_i = 4;
    goal_j = 4;

    dx=Lx/nx;
    dy=Ly/ny;

    double v = 1e0;

    f = 1e0/v;

    for(int i=0; i<1000; i++){
        H.emplace_back();
    }

    x.resize(ny+1, vector<vector<double>>(nx+1, vector<double>(2, 0)));
    T.resize(ny+1, vector<double>(nx+1, 0));
    lambda.resize(ny+1, vector<int>(nx+1, 0));

    for(int i=0; i<nx+1; i++){
        for(int j=0; j<ny+1; j++){
            x.at(i).at(j).at(0) = j*dx;
            x.at(i).at(j).at(1) = i*dx;
        }
    }
}

void FMM::FastMarchingMethod(){
    InitGrid();
    bool is_empty_H = false;
    while(!is_empty_H){
        int i = H.at(0).at(1);
        int j = H.at(0).at(2);
        DeleteHeap(H, size);
        FixGrid(i, j, lambda, T, H);
        if(size==0) is_empty_H = true;
    }
}

void FMM::UpdateGrid(int _i, int _j){
    T_H = min(T.at(_i-1).at(_j), T.at(_i+1).at(_j));
    T_V = min(T.at(_i).at(_j-1), T.at(_i).at(_j+1));

    if( f > abs(T_H-T_V)){
        T.at(_i).at(_j) = (T_H+T_V+sqrt(2*pow(f, 2)-pow((T_H-T_V), 2)))/2;
    }else{
        T.at(_i).at(_j) = f + min(T_H, T_V);
    }
}

void FMM::FixGrid(int &_i, int &_j, vector<vector<int>>& _lambda, vector<vector<double>>& _T, vector<vector<int>>& _H){
    _lambda.at(_i).at(_j) = fix;

    for(int i=_i-1; i<=_i+1; _i+2){
        for(int j=_j-1; j<=_j+1; _j+2){
            if( i != goal_i && j != goal_j && _lambda.at(i).at(j) != fix){
                UpdateGrid(i,j);
                if(_lambda.at(i).at(j) == far){
                    _lambda.at(i).at(j) == near;
                    H.at(tmp).push_back(tmp);
                    H.at(tmp).push_back(i);
                    H.at(tmp).push_back(j);
                    tmp++;
                    size++;
                }else{
                    UpHeap(H,i,j);
                }
            }
        }
    }
}

void FMM::InitGrid(){

    for(int i=0; i<ny+1; i++){
        for(int j=0; j<nx+1; j++){
            if(i == goal_i && j == goal_j){
                T.at(i).at(j) = 0;
                lambda.at(i).at(j) = fix;
            }else{
                T.at(i).at(j) = 1e10;
                lambda.at(i).at(j) = far;
            }
        }
    }

    for(int i=0; i<ny+1; i++){
        for(int j=0; j<nx+1; j++){
            if(lambda.at(i).at(j) == fix){
                FixGrid(i, j, lambda, T, H);
            }
        }
    }
}

