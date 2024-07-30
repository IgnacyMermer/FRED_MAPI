#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class DelayA: public Iterativemapi
{
public:
    DelayA();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    string sequence;
};