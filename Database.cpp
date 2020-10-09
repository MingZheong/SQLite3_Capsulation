#include "Database.h"

namespace Database{

    list<string> split(string str, string sep)
    {
        list<string> strList;
        uint startPos = 0;
        while (1)
        {
            if(startPos == str.size()+sep.size()) break;
            uint pos = str.find(sep,startPos);
            //cout << pos << endl;
            if(pos == string::npos || pos >= str.size())
                pos = str.size();
            string tmp = str.substr(startPos, pos-startPos);
            strList.push_back(tmp);
            startPos = pos+sep.size();
        }
        return strList;
    }

    

    
}

