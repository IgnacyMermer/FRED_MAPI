#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class BoardType: public Iterativemapi
{
public:
    BoardType();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;
};