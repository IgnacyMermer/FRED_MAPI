#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class RefreshCounters: public Iterativemapi
{
public:
    RefreshCounters();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    bool firstTime;
    string sequence;
};