// fuente : https://www.geeksforgeeks.org/dsa/patience-sorting/
#include <iostream>
#include <vector>
#include <algorithm> 
#include <stack>     
#include <iterator>
#include <climits> 

using namespace std;


vector<int> merge_piles(vector<vector<int>>& v) {
    vector<int> ans;

    while (1) {
        int minu = INT_MAX;
        int index = -1;

        for (int i = 0; i < v.size(); i++) {
            if (minu > v[i][v[i].size() - 1]) {
                minu = v[i][v[i].size() - 1];
                index = i;
            }
        }

        ans.push_back(minu);
        v[index].pop_back();

        if (v[index].empty()) {
            v.erase(v.begin() + index);
        }

        if (v.size() == 0)
            break;
    }

    return ans;
}


void patienceSort(vector<int>& arr) {
    if (arr.empty()) return;

    vector<vector<int>> piles;

    for (int i = 0; i < arr.size(); i++) {
        if (piles.empty()) {
            vector<int> temp;
            temp.push_back(arr[i]);
            piles.push_back(temp);
        }
        else {
            int flag = 1;

            for (int j = 0; j < piles.size(); j++) {
                if (arr[i] < piles[j][piles[j].size() - 1]) {
                    piles[j].push_back(arr[i]);
                    flag = 0;
                    break;
                }
            }

            if (flag) {
                vector<int> temp;
                temp.push_back(arr[i]);
                piles.push_back(temp);
            }
        }
    }

    
    arr = merge_piles(piles);
}