#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class ActualValues: public Iterativemapi
{
public:
    ActualValues();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;
};