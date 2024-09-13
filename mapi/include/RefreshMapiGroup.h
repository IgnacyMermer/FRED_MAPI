#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/mapigroup.h"
#include "Fred/fred.h"
#include <utility>


class RefreshMapiGroup: public Mapigroup
{
public:
    RefreshMapiGroup(Fred* fred, std::vector<std::pair<std::string, std::string>> refreshServices);

    string processInputMessage(string input);
    string processOutputMessage(string output);

private:
    Fred* fred;
    int64_t hexValue;
    bool firstTime;
    string sequence;
    std::vector<int64_t> oldValues;
    std::vector<std::pair<std::string, std::string>> refreshServices;
};
