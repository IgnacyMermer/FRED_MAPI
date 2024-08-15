#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class PM_status: public Iterativemapi
{
public:
    PM_status(std::string endpointParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    long long finalValue;
    string sequence, pmEndpoint;
};
