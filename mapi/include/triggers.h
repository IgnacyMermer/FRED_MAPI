#include "Fred/Mapi/mapi.h"

class Triggers: public Mapi
{
public:
    Triggers();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;

    void sequenceOperation1(long long num, std::string address, std::string& sequence);
    void sequenceOperation2(int num, std::string address, std::string& sequence);
    void sequenceOperation3(int num, std::string address, std::string& sequence);
    void sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence);
};
