#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class PM_default: public Mapi{
public:
    PM_default(std::string endpointParam, std::string pmNumberParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    long long finalValue;
    string sequence, endpoint, pmNumber;
};
