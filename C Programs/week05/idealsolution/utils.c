#include "utils.h"

char flipChar(char c){

if ('a' <= c && c <= 'z') // a valid lowercase
   return 'z'-c + 'a'; // c-'a' is offset
else if ('A' <= c && c <= 'Z') // a valid lowercase
   return 'Z'-c + 'A';
else if ('0' <= c && c <= '9') // a valid lowercase
   return '9'-c + '0';
else return c;
}
