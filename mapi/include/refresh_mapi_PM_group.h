#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/mapigroup.h"
#include "Fred/fred.h"
class RefreshMapiPMGroup: public Mapigroup
{
public:
    RefreshMapiPMGroup(Fred* fred);

    string processInputMessage(string input);
    string processOutputMessage(string output);

private:
    Fred* fred;
    float hexValue;
    bool firstTime;
    string sequence;
    std::vector<std::string> services;
    std::vector<long long> oldValues;
};