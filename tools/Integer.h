//
// Created by 樊溪 on 2017/7/4.
//

#ifndef STUDY_INTEGER_H
#define STUDY_INTEGER_H

#include<vector>
#include<iostream>

using  namespace std;

class Integer : protected vector<int>{
    //是否为负数
    bool negative;

public:
    Integer(const char * value);
    Integer(long value = 0);
    Integer(const string & value);

    Integer &operator= (const Integer& value);
    Integer &operator+ (const Integer& value);
    Integer &operator- (const Integer& value);
    Integer &operator* (const Integer& value);
    Integer &operator/ (const Integer& value);

    friend std::ostream & operator<< (std::ostream &cout, const Integer & value);
    friend std::istream & operator>> (std::istream &cin, Integer &value);
};


#endif //STUDY_INTEGER_H
