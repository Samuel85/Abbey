# ConfigReader

This is a simple config reader created for my "Abbey" repository.

All tokens and values in the file "config.txt", are parsed and stored in memory using a single linked list for easy access.

# Example
```
// Uncomment this flag to get more info
//#define _DEBUGMODE_

#include "configreader.h"

int main()
{
    ConfigReader *configreader = new ConfigReader("config.txt");
    configreader->parse();
    
    int language = stoi(configreader->getValue("LANGUAGE"));    
    string save0 = configreader->getValue("SAVE0");
    
    
    std::cout << "language=" << language << endl;
    std::cout << "SAVE0=" << save0 << endl;
    std::cout << "SAVE1=" << configreader->getValue("SAVE1") << endl;

    configreader->setValue("SAVE1","newVal");
    std::cout << "SAVE1=" << configreader->getValue("SAVE1") << endl;
    
    return 0;
}

```
