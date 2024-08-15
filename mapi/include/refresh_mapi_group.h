#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/mapigroup.h"
#include "Fred/fred.h"
class RefreshMapiGroup: public Mapigroup
{
public:
    RefreshMapiGroup(Fred* fred);

    string processInputMessage(string input);
    string processOutputMessage(string output);

private:
    Fred* fred;
    bool firstTime;
};
