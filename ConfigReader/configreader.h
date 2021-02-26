#ifndef _CONFIGREADER_H_
#define _CONFIGREADER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "singlelinkedlist.h"

using namespace std;

class ConfigReader
{
private:
    string filename;
    struct Entry
    {
        string token;
        string value;
        
        Entry(string t, string v):token(t),value(v) {}
        Entry(){}        
        void print(){
            cout << "token:" << token << ", value:" << value << endl;
        }
    };
    SingleLinkedList<Entry> list;
public:
    ConfigReader(const string fname) : filename(fname){}
    ~ConfigReader(){}
    bool parse();
    string getValue(const string &token);
    void setValue(const string &token, const string &value);
    bool isEmpty();
    void deleteAll();
};

#endif
