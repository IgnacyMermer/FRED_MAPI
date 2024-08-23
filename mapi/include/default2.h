#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class Default2: public Iterativemapi
{
public:
    Default2(std::string endpointParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    long long finalValue;
    string sequence, endpoint;
    void sequenceOperation1(long long num, std::string address, std::string& sequence);
    void sequenceOperation2(int num, std::string address, std::string& sequence);
    void sequenceOperation3(int num, std::string address, std::string& sequence);
    void sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence);
    long long parameterValue(std::string strValue);
};