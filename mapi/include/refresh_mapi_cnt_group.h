#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/mapigroup.h"
#include "Fred/fred.h"
class RefreshMapiCNTGroup: public Mapigroup
{
public:
    RefreshMapiCNTGroup(Fred* fred);

    string processInputMessage(string input);
    string processOutputMessage(string output);

private:
    Fred* fred;
    uint32_t hexValue;
    bool firstTime;
    string sequence;
    std::vector<std::string> services, servicesRates;
    std::vector<uint32_t> oldValues, oldTimes;
    std::vector<float> oldValuesRates;
};
