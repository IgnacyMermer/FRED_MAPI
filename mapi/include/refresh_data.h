#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class RefreshData: public Iterativemapi
{
public:
    RefreshData();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    bool firstTime;
    string sequence;
};