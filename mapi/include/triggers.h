#include "Fred/Mapi/mapi.h"

class Triggers: public Mapi
{
public:
    Triggers();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;

    void sequenceOperationWrite(long long num, std::string address, std::string& sequence);
    void sequenceOperationRMWAND(int num, std::string address, std::string& sequence);
    void sequenceOperationRMWOR(int num, std::string address, std::string& sequence);
    void sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence);
};
