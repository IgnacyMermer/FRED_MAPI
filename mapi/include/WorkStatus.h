#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class WorkStatus: public Iterativemapi
{
public:
    WorkStatus();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int status;
    string sequence;
};