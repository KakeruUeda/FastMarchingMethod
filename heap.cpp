#include "lib.h"

int getLeft(int parent) {
    return parent * 2 + 1;
}

int getRight(int parent) {
    return parent * 2 + 2;
}

int getParent(int child) {
    return (child - 1) / 2;
}
/*
void FMM::AddHeap(vector<int>& H, int size) {

    int add; 
    int parent;

    add = size;
    if (add == 0) {
        return;
    }

    while (1) {
        parent = getParent(add);
        
        if (H.at(parent) > H.at(add)) {
            swap(H.at(parent), H.at(add));
            add = parent;
            if (add == 0) {
                break;
            }
        } else {
            break;
        }
    }
}*/
void FMM::DeleteHeap(vector<vector<int>>& H, int size) {
    int left; 
    int right;
    int smaller; 
    int parent; 
   
    swap(H.at(0), H.at(size-1));
    
    size--;
    parent = 0;
    
    while (1) {
        left = getLeft(parent);
        right = getRight(parent);

        /* 子ノードの大きい値を持つ方の位置を取得 */
        if (left < size && right < size) {
            /* 左右両方の子ノードが存在する場合は比較して確認 */
            if (H.at(left).at(0) > H.at(right).at(0)) {
                smaller = right;
            } else {
                smaller = left;
            }
        } else if (left < size) {
            smaller = left;
        } else {
            break;
        }

        if (H.at(smaller).at(0) >= H.at(parent).at(0)) {
            break;
        }
        swap(H.at(smaller), H.at(parent));

        parent = smaller;
    }
}
/*
void makeHeap(int a[], int num) {

    int size;
    size = 0;

    while (size < num) {

        addHeap(a, size);
        size++;
    }
}*/

void FMM::UpHeap(vector<vector<int>> &H, int _i, int _j){
    int parent;
    int target;

    auto it = find_if(
        begin(H), end(H),
        [&_i, &_j](const auto& row) {
            return row.at(1) == _i && row.at(2) == _j;
        }
    );
    if(it == end(H)) {
        cout << "not found";
    }
    else {
        cout << distance(begin(H), it);
    }

    while (1) {    
    
        parent = getParent(target);
        if (H.at(parent).at(0) > H.at(target).at(0)) {

            swap(H.at(parent), H.at(target));
            target = parent; 

            if (target == 0) {
                break;
            }
        } else {
            break;
        }
    }
    
}