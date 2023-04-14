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

void FMM::FixGrid(int & _i, int & _j, vector<vector<int>> _lambda){
    _lambda.at(_i).at(_j) = fix;

    for(int i=_i-1; i<=_i+1; _i+2){
        for(int j=_j-1; j<=_j+1; _j+2){
            if( i != goal_i && j != goal_j && lambda)
        }
    }
}

tuple<double, int, int> FMM::InitGrid(){

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
                FixGrid(i, j, lambda);
            }
        }
    }

    return forward_as_tuple(T, lambda, H);
}