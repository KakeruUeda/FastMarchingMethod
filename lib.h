#include <bits/stdc++.h>
#include <vector>
using namespace std;

enum lambda_type{
    fix = 1,
    near = 2,
    far = 3, 
};

class FMM{
    public:
    int nx, ny, goal_i, goal_j;
    double Lx, Ly;
    double dx, dy;

    vector<vector<vector<double>>> x;
    vector<vector<double>> T;
    vector<vector<int>> lambda;

    void DefineGrid();
    tuple<double, int, int> InitGrid();
    void FixGrid(int & _i, int & _j, vector<vector<int>> _lambda);
};