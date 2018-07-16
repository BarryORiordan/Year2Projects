#include "utils.h"

char flipChar(char c)
{
    if('a' <= c && c <= 'z')        //A valid Lowercase
        return 'z'- c + 'a';

    else if('A' <= c && c <= 'Z')   //A valid Uppercase
        return 'Z'- c + 'A';

    else if('0' <= c && c <= '9')   //A valid Digit
        return '9' - c + '0';

    return c;                       //Wasn't LC/UC/Digit so leave alone
}
