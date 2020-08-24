#include "utility.hpp"

using namespace std ;

namespace Utility
{
    const vector<string> split(string str, const string deliminator) noexcept
    {
        if(str.length() < deliminator.length()) {
            return vector<string>{str} ;
        }

        vector<string> vec ;
        while(true) {
            auto pos = str.find(deliminator) ;
            if(pos == string::npos) {
                vec.push_back(str) ;
                return vec ;
            }
            const auto head = str.substr(0, pos) ;
            if(!head.empty()) {
                vec.push_back(head) ;
            }
            str = str.substr(pos + deliminator.size()) ;
        }
        return vec ;
    }

    const string remove_str(string str, const unsigned char target) noexcept {
        //target char is collected at end of sequence by remove.
        //Thus, erase form return iterator of remove to end of it.
        str.erase(remove(str.begin(), str.end(), target), str.end()) ;
        return str ;
    }
}