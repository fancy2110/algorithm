#include <iostream>
#include <string>
//#include "tools/AVLTree.h"

int main() {
    std::string s("aba");
    bool P[1000][1000]={false};  
    int start = 0;
    int max = 1;
    int length = s.length();

    for (int i=0; i < length; i++) {
        P[i][i] = true;
        for(int j = 1; j + i < length; j++) {
            if (s[i] != s[i+j]) {
                break;
            } 
            
            P[i][i+j] = true;
            if (j + 1 > max) {
                start = i;
                max = j + 1;
            }
        }
    }
    std::cout << "start:" << start << ", max:" << max << std::endl;
    
    for (int len=3; len <= length; len++) {
        for (int i=0; i <= length - len; i++) {
            int j = i + len - 1;
            
            if (P[i+1][j-1] && s[i] == s[j]) {
                P[i][j] = true;
                
                if (max < len) {
                    start = i;
                    max = len;
                }
            }
        }
    }
    
    std::cout<< s.substr(start, max) << std::endl;
}