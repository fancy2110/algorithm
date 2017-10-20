//
// Created by 樊溪 on 2017/7/4.
//

#include "Integer.h"

Integer &Integer::operator=(const Integer &value) {
    if (this == &value) {
        return *this;
    }

    this->negative = value.negative; 
    this->clear();

    for(int i = 0; i < value.size(); i++) 
    {
        this->push_back(value[i]);
    }

    return *this;
}

Integer &Integer::operator+(const Integer &value) {
    Integer *new_value = new Integer(*this);
    int number = 0;

    int min_size = min(new_value->size(), value.size());
    int max_size = max(new_value->size(), value.size());


    for(int begin = new_value->size(); begin <= max_size; begin++) 
    {
        //补齐至最大位数，多补一位以防进位
        new_value->push_back(0);     
    }


    for(int i = 0; i <= max_size; i++) {
        int int_a = (new_value->size() > i ? new_value->at(i) : 0) * (new_value->negative ? -1 : 1); 
        int int_b = (value.size() > i ? value[i] : 0) * (value.negative ? -1 : 1);

        int int_c = (int_a + int_b + number)  % 10;
        number = (int_a + int_b + number) / 10;
        (*new_value)[i]  = int_c;
    }


    for (int i = max_size ; i >= 0; i--) 
    {
        if (new_value->at(i) != 0) { //非空第一个数字为决定了相加结果为正为负
            new_value->negative = new_value->at(i) < 0;
            break;
        }
    }

    for (int i = 0 ; i < max_size; i++) 
    {
        if (new_value->at(i) < 0)
        {
            if (new_value->negative) {
                (*new_value)[i] = -new_value->at(i);
            } else {
                (*new_value)[i+1] --;
                (*new_value)[i] = 10 + new_value->at(i); 
            }
        } else if (new_value->at(i) > 0) {
            if (new_value->negative) {
                (*new_value)[i] = 10 - new_value->at(i);
                (*new_value)[i+1] += 1;
            }
        }
    }

    //高位清零, 遇到第一个不为零的位时退出
    for (int i = max_size ; i > 0; i--) 
    {
        if (new_value->at(i) == 0) 
        {
            new_value->pop_back(); //清空高位零数
            continue;
        }
        break;
    }

    return (*new_value);
}

Integer &Integer::operator*(const Integer &value) {



    return *this;
}

Integer &Integer::operator-(const Integer &value) {
    Integer new_value(value);
    new_value.negative = !value.negative;
    return this->operator+(new_value);
}

Integer &Integer::operator/(const Integer &value) {
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Integer &value) {
    if (value.negative) {
        os << "-";
    }

    bool flag = false;
    for (auto iter = value.rbegin() ; iter != value.rend(); ++iter) {
        if ((*iter) == 0 && !flag) {
            flag = false;
            continue;
        }

        flag = true;
        os << (*iter);
    }


    if (value.size() == 0) {
        os << 0;
    }

    return os;
}

std::istream &operator>>(std::istream &in, Integer &value) {
    string data;
    in >> data;

    for (int i = 0; i < data.length(); i++) {
        int int_value = data[i] - '0';
        if (int_value < 0 || int_value > 9) {
            std::cout << "error format Integer" << std::endl;
            throw std::exception();
        } else {
            value.push_back(int_value);
        }
    }

    return in;
}

Integer::Integer(const char *value) {
    size_t size = strlen(value);

    if (value == NULL) {
        push_back(0);
        return;
    }

    if (value[0] == '-') {
        negative = true;
        value++;
        size--;
    } else {
        negative = false;
    }

    for (int i = 0; i < size; i++) {
        int int_value = value[i] - '0';
        if (int_value < 0 || int_value > 9) {
            throw std::exception();
        }

        this->push_back(int_value);
    }

    std::reverse(begin(), end());
}

Integer::Integer(long value) {
    negative = value < 0;

    if (negative) {
        value = -value;
    }

    clear();

    if (value == 0) {
        push_back(0);
        return;
    }

    int i = 0;
    while (value > 0) {
        push_back((int) (value % 10));
        value /= 10;
        i++;
    }
}

Integer::Integer(const string &value) {
    const char *data = value.c_str();
    size_t size = strlen(data);

    if (data[0] == '-') {
        negative = true;
        data++;
        size--;
    } else {
        negative = false;
    }

    for (int i = 0; i < size; i++) {
        int int_value = data[i] - '0';
        if (int_value < 0 || int_value > 9) {
            throw std::exception();
        }

        this->push_back(int_value);
    }

    std::reverse(begin(), end());
}


