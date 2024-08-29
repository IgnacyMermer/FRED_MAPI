#include "Fred/Mapi/mapi.h"

class ORGate: public Mapi{
public:
    ORGate(string endpointParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence, endpoint;
};