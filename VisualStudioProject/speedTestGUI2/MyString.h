#ifndef MYSTRING_H
#define MYSTRING_H

#include <string>

namespace std {

     #if defined _UNICODE || defined UNICODE

     typedef wstring tstring;

     #else

     typedef string tstring;

    #endif

}

#endif MYSTRING_H