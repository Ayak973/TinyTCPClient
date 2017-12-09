/* 
 * File:   stringhelper.h
 * Author: ayak973
 *
 * Created on 7 décembre 2017, 21:05
 */

#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <string>
#include <sstream>

namespace str {
    
    template<typename Arg, typename... Args>
    std::string createMultiStr(Arg&& arg, Args&&... args) {
        std::stringstream strstrout;        
        //c++14: if the template doesnt have a default arg (only template<typename... Args>), 
        //we have to use (void)expander { 0 , (void(strstrout << std::forward<Args>(args)), 0)... }, because a 0 sized array is invalid
        using expander = int[];
        (void)expander {(void(strstrout << std::forward<Arg>(arg)), 0), (void(strstrout << std::forward<Args>(args)), 0)... };
        //The two lines upward can be replaced in c++17 with: (strstrout << ... << args);

        return strstrout.str();
    }
    
}

#endif //STRINGHELPER_H