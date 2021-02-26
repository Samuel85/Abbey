#include "configreader.h"

bool ConfigReader::parse()
{
    bool r = false;
    ifstream f(filename);
    string line;
    if (f.is_open())
    {
        r = true;
        while(!f.eof())
        {
            // Get new line and ignore empty lines
            getline(f >> std::ws ,line);
            
            if ((line.at(0) != '#') && (line.at(0) != ' '))
            {
                // Remove spaces
                line.erase(remove(line.begin(), line.end(), ' '), line.end());                    
                
                // find the delimiter
                int delimiterPosition = line.find("=");
                
                // get token and value
                string token = line.substr(0, delimiterPosition);
                string value = line.substr(delimiterPosition+1,line.length());
                
                // store it
                list.insert(Entry(token,value));
                
                // print it
                #ifdef _DEBUGMODE_
                cout << "token:" << token << "," << "value:" << value << endl;
                #endif
            }
        }
    }
    else {
        cout << "Can't open config file" << endl;
    }    
    f.close();
    
    #ifdef _DEBUGMODE_
    list.displayAll();
    #endif
    
    return r;
}

string ConfigReader::getValue(const string &token)
{
    return list.getValue(token);
}

void ConfigReader::setValue(const string &token, const string &value)
{
    return list.setValue(token, value);
}

bool ConfigReader::isEmpty(){
    return list.isEmpty();
}

void ConfigReader::deleteAll(){
    list.deleteAll();
}