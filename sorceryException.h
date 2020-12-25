#ifndef _SORCERYEXN_H_
#define _SORCERYEXN_H_

#include <string>

// This is our error game msg Struct

struct SorceryException {
    std::string message;
    SorceryException( std::string message );
};

#endif
