#include "Fred/Mapi/mapi.h"

class ResetErrors: public Mapi{
public:
    ResetErrors();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    string sequence;
};

