#include "lib.h"
using namespace std;
void FMM::DefineGrid(){

    nx = 30;
    ny = 30;
    Lx = 1e0;
    Ly = 1e0;
    goal_i = 10;
    goal_j = 5;

    dx=Lx/nx;
    dy=Ly/ny;

    numOfNode=(nx+1)*(ny+1);
    numOfElm=nx*ny;

    double v = 1e0;

    f = 1e0/v;

    for(int i=0; i<10000; i++){
        H.emplace_back();
        H_tmp.emplace_back();
    }


    x.resize(numOfNode, vector<double>(2, 0));
    T.resize(ny+1, vector<double>(nx+1, 0));
    element.resize(numOfElm);
    T_1D.resize(numOfNode);
    lambda.resize(ny+1, vector<int>(nx+1, 0));

    for(int ic=0;ic<numOfElm;ic++){
        element[ic].node.resize(4);
    }
    
    int tmp3 = 0;
    for(int i=0; i<ny+1; i++){
        for(int j=0; j<nx+1; j++){
            x.at(tmp3).at(0)=j*dx;
            x.at(tmp3).at(1)=i*dy;
            tmp3++;
        }
    }

    tmp3=0;
    for(int i=0;i<ny;i++){
        for(int j=0;j<nx;j++){
        element[tmp3].node[0]=j   +i*(nx+1);
        element[tmp3].node[1]=j+1 +i*(nx+1);
        element[tmp3].node[2]=j+1 +(i+1)*(nx+1);
        element[tmp3].node[3]=j   +(i+1)*(nx+1);
        tmp3++;
        }
    }
    
}

void FMM::FastMarchingMethod(){
   // cout << "1" << endl;
    int loop = 0;
    InitGrid();
    bool is_empty_H = false;
    while(!is_empty_H){
        cout << "loop = " << loop << endl;
        DeleteHeap(H, size);
        int i = H.at(size).at(1);
        int j = H.at(size).at(2);
        H.erase(H.begin()+size);
        //cout << "i = " << i << " j = " << j << endl;
        FixGrid(i, j, lambda, T, H);
        cout << "size = " << size << endl;
        if(size==0){
            ofstream output_T("T.dat");
            for(int i=0; i<ny+1; i++){
                for(int j=0; j<nx+1; j++){
                    output_T << i << " " << j << " " << T.at(i).at(j) << endl;
                }
            }
            output_T.close();
            is_empty_H = true;
        }
        loop++;
        //if(loop == 3) exit(1);
    }
    //cout << "3" << endl;
}

void FMM::UpdateGrid(int _i, int _j){
    //cout << "update _i = " << _i << endl;
    //cout << "update _j = " << _j  << endl;
    //cout << "2_1_1_1" << endl;
    if(_i == nx && _j != ny && _j != 0){
        //cout << "in1" << endl;
        T_H = T.at(_i-1).at(_j);
        T_V = min(T.at(_i).at(_j-1), T.at(_i).at(_j+1));
    }else if(_j == ny && _i != nx && _i != 0){
        //cout << "in2" << endl;
        T_H = min(T.at(_i-1).at(_j), T.at(_i+1).at(_j));
        T_V = T.at(_i).at(_j-1);
    }else if(_i == 0 && _j != 0 && _j != nx){
        //cout << "in3" << endl;
        T_H = T.at(_i+1).at(_j);
        T_V = min(T.at(_i).at(_j-1), T.at(_i).at(_j+1));   
    }else if(_j == 0 && _i != 0 && _i != nx){
        //cout << "in4" << endl;
        T_H = min(T.at(_i-1).at(_j), T.at(_i+1).at(_j));
        T_V = T.at(_i).at(_j+1);
    }else if(_i == nx && _j == ny){  
       //cout << "in5" << endl;
        T_H = T.at(_i-1).at(_j);
        T_V = T.at(_i).at(_j-1);
    }else if(_i == 0 && _j == 0){
        //cout << "in6" << endl;
        T_H = T.at(_i+1).at(_j);
        T_V = T.at(_i).at(_j+1);
    }else if(_i == nx && _j == 0){
        //cout << "in7" << endl;
        T_H = T.at(_i-1).at(_j);
        T_V = T.at(_i).at(_j+1);
    }else if(_i == 0 && _j == ny){
        T_H = T.at(_i+1).at(_j);
        T_V = T.at(_i).at(_j-1);
    }else{
        T_H = min(T.at(_i-1).at(_j), T.at(_i+1).at(_j));
        T_V = min(T.at(_i).at(_j-1), T.at(_i).at(_j+1));   
    }


    //cout << "2_1_1_2" << endl;
    if( f > abs(T_H-T_V)){
        T.at(_i).at(_j) = (T_H+T_V+sqrt(2*pow(f, 2)-pow((T_H-T_V), 2)))/2;
    }else{
        T.at(_i).at(_j) = f + min(T_H, T_V);
    }
    //cout << "2_1_1_3" << endl;
}

void FMM::FixGrid(int _i, int _j, vector<vector<int>>& _lambda, vector<vector<double>>& _T, vector<vector<int>>& _H){
    _lambda.at(_i).at(_j) = fix;

    //cout << "2_1_00" << endl;
    //cout << "_i = " << _i << "_j = " << _j << endl;
    int step = 0;

    for(int i=_i-1; i<=_i+1; i++){
        for(int j=_j-1; j<=_j+1; j++){
              //cout << "i = " << i << " j = " << j << " step = " << step << endl;
            if(i == nx+1 || j == ny+1 || step == 0 || step == 2 || step == 4 || step == 6 || step == 8 || i < 0 || j < 0){
                //cout << "skip" << endl;
                step++;
                continue;
            }
            //cout << "i = " << i << " j = " << j << " step = " << step << endl;

            if( (i != goal_i || j != goal_j) && _lambda.at(i).at(j) != fix){
                //cout << size << endl;
                //cout << "i = " << i << " j = " << j << " lambda = " << _lambda.at(i).at(j) << endl;
                //cout << "2_1_0" << endl;
                UpdateGrid(i,j);
                //cout << "2_1_1" << endl;
                if(_lambda.at(i).at(j) == far){
                    _lambda.at(i).at(j) = near;
                   // cout << "i = " << i << " j = " << j << " lambda = " << _lambda.at(i).at(j) << endl;
                    //cout << "2_1_2" << endl;
                    //cout << "tmp = " << tmp << "  i = " << i << "  j = " << j <<  endl;
                    H.at(size).push_back(tmp);
                    //cout << "2_1_3" << endl;
                    H.at(size).push_back(i);
                    //cout << "2_1_4" << endl;
                    H.at(size).push_back(j);
                    //cout << "2_1_5" << endl;
                    //cout << "size = " << size << "  tmp = " << tmp <<  "  H.at(size).at(0) = " << H.at(size).at(0) << "  H.at(size).at(1) = " <<  H.at(size).at(1)  << "  H.at(size).at(2) = " <<  H.at(size).at(2)  <<  endl;
                    tmp++;
                    size++;
                }else{
                    //cout << "2_1_6" << endl;
                    UpHeap(H,i,j);
                    //cout << "2_1_7" << endl;
                }
            }
            
            step++;
        }
    }
    //cout << "1_1_5" << endl;
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
    //cout << "1_1" << endl;

    for(int i=0; i<ny+1; i++){
        for(int j=0; j<nx+1; j++){
            if(lambda.at(i).at(j) == fix){
                FixGrid(i, j, lambda, T, H);
            }
        }
    }
    //cout << "1_2" << endl;
}

