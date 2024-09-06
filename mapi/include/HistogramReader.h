#include "Fred/Mapi/mapi.h"

class HistogramReader : public Mapi
{
public:
    HistogramReader();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    string sequence;
    uint32_t finalValue;
};