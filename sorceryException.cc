#include "sorceryException.h"

SorceryException::SorceryException( std::string msg ) {
    std::string prefix = "[SORCERY EXCEPTION HANDLER]: ";
    message = prefix + msg;
}
