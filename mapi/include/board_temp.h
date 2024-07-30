#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class BoardTemp: public Iterativemapi
{
public:
    BoardTemp();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    string sequence;
};