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
    long long hexValue;
    bool firstTime;
    string sequence;
    std::vector<std::string> services, servicesRates;
    std::vector<long long> oldValues, oldTimes;
    std::vector<float> oldValuesRates;
};
