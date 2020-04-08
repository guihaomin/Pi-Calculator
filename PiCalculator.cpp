// PiCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <thread>
#define thread_count 1
#define divide_precision 100
using namespace std;
string thread_result[thread_count];
/* precondition: a is larger than b */
string strDiv(int a, int b, int precision) {
    int result, remainder;
    result = a / b;
    string ans = "";
    int tmp = result;
    if (result == 0) {
        ans.push_back('0');
    }
    else {
        while (tmp != 0) {
            ans.push_back('0' + (tmp % 10));
            tmp /= 10;
        }
    }
    //ans.push_back(result);
    ans.push_back('.');
    a = a - result * b;
    a *= 10;
    while (precision > 0) {
        result = a / b;
        remainder = a - result * b;
        if (remainder == 0) {
            ans.push_back('0' + result);
            break;
        }
        else {
            ans.push_back('0' + result);
        }
        a = remainder * 10;
        precision--;
    }
    return ans;
}
string strMinus(string a, string b) {
    string::size_type dotLoca = a.find(".", 0);
    string::size_type dotLocb = b.find(".", 0);
    string fraca = a.substr(dotLoca + 1);
    string fracb = b.substr(dotLocb + 1);
    string inta = a.substr(0, dotLoca);
    string intb = b.substr(0, dotLocb);
    while (fraca.size() < fracb.size()) {
        fraca = fraca + "0";
    }
    /*
    if (inta.size() < intb.size()) {
        string tmp = inta;
        inta = intb;
        intb = tmp;
    }*/
    int lena = fraca.size();
    int lenb = fracb.size();
    int len_inta = inta.size();
    int len_intb = intb.size();

    //int ptr = lena - 1;
    int underflow = 0;
    string result = "";
    int addResult = 0;
    int toAdd1;
    int toAdd2;
    string tmp;
    for (int ptr = lena - 1; ptr >= 0; ptr--) {
        toAdd1 = fraca[ptr] - '0';
        if (ptr >= lenb) {
            toAdd2 = 0;
        }
        else {
            toAdd2 = fracb[ptr] - '0';
        }
        addResult = toAdd1 - toAdd2 + underflow;
        if (addResult < 0) {
            underflow = -1;
            addResult = addResult + 10;
        }
        else {
            underflow = 0;
        }
        tmp = "";
        tmp.push_back('0' + addResult);
        result = tmp + result;
    }
    result = "." + result;
    for (int ptr = len_inta - 1; ptr >= 0; ptr--) {
        toAdd1 = inta[ptr] - '0';
        if (ptr - (len_inta - len_intb) < 0) {
            toAdd2 = 0;
        }
        else {
            toAdd2 = intb[ptr - (len_inta - len_intb)] - '0';
        }
        addResult = toAdd1 - toAdd2 + underflow;
        if (addResult < 0) {
            underflow = -1;
            addResult = addResult + 10;
        }
        else {
            underflow = 0;
        }
        
        tmp = "";
        tmp.push_back('0' + addResult);
        result = tmp + result;
    }
    /*if (overflow == 1) {
        result = "1" + result;
    }*/
    return result;
}
string strAdd(string a, string b) {
    string::size_type dotLoca = a.find(".", 0);
    string::size_type dotLocb = b.find(".", 0);
    string fraca = a.substr(dotLoca + 1);
    string fracb = b.substr(dotLocb + 1);
    string inta = a.substr(0, dotLoca);
    string intb = b.substr(0, dotLocb);
    if (fraca.size() < fracb.size()) {
        string tmp = fraca;
        fraca = fracb;
        fracb = tmp;
    }
    if (inta.size() < intb.size()) {
        string tmp = inta;
        inta = intb;
        intb = tmp;
    }
    int lena = fraca.size();
    int lenb = fracb.size();
    int len_inta = inta.size();
    int len_intb = intb.size();

    //int ptr = lena - 1;
    int overflow = 0;
    string result = "";
    int addResult = 0;
    int toAdd1;
    int toAdd2;
    string tmp;
    for (int ptr = lena - 1; ptr >= 0; ptr--) {
        toAdd1 = fraca[ptr] - '0';
        if (ptr >= lenb) {
            toAdd2 = 0;
        }
        else {
            toAdd2 = fracb[ptr] - '0';
        }
        addResult = toAdd1 + toAdd2 + overflow;
        if (addResult >= 10) {
            overflow = 1;
        }
        else {
            overflow = 0;
        }
        addResult = addResult % 10;
        tmp = "";
        tmp.push_back('0' + addResult);
        result = tmp + result;
    }
    result = "." + result;
    for (int ptr = len_inta - 1; ptr >= 0; ptr--) {
        toAdd1 = inta[ptr] - '0';
        if (ptr - (len_inta - len_intb) < 0) {
            toAdd2 = 0;
        } else {
            toAdd2 = intb[ptr - (len_inta-len_intb)] - '0';
        }
        addResult = toAdd1 + toAdd2 + overflow;
        if (addResult >= 10) {
            overflow = 1;
        }
        else {
            overflow = 0;
        }
        addResult = addResult % 10;
        tmp = "";
        tmp.push_back('0' + addResult);
        result = tmp + result;
    }
    if (overflow == 1) {
        result = "1" + result;
    }
    return result;
}
void calculate_pi(int thread_id, int start_offset, int n) {
    thread_result[thread_id] = "1.0";
    for (int i = start_offset; i < n; i++) {
        if (i % 1000 == 0) {
            cout << "%" << (i-start_offset) * 100.0 / (n-start_offset) << "\n";
        }
        if (i % 2 == 1) {
            thread_result[thread_id] = strMinus(thread_result[thread_id], strDiv(1, (2 * i + 1), divide_precision));
        }
        else {
            thread_result[thread_id] = strAdd(thread_result[thread_id], strDiv(1, (2 * i + 1), divide_precision));
        }
    }
}
int main()
{
    int n;
    std::cout << "Please give a loop count: ";
    std::cin >> n;
    string result = "1.0";
    thread worker_threads[thread_count];
    int sign;
    n = (n / thread_count) * thread_count + 1;
    int work_load = (n - 1) / thread_count;
    for (int i = 0; i < thread_count; i++) {
        worker_threads[i] = thread(calculate_pi, i, 1 + i * work_load, 1 + (1+ i) * work_load);
    }
    for (int i = 0; i < thread_count; i++) {
        worker_threads[i].join();
    }
    for (int i = 0; i < thread_count; i++) {
        result = strAdd(result, thread_result[i]);
    }
    float thread_count_float = thread_count;
    result = strMinus(result, to_string(thread_count_float));

    cout << "The value of pi/4 is " << result;
}
