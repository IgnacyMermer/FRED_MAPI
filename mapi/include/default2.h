#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class Default2: public Iterativemapi
{
public:
    Default2(std::string endpointParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    long long finalValue;
    string sequence, endpoint;
};