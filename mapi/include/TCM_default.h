#include "Fred/Mapi/mapi.h"


class TCM_default: public Mapi
{
public:
    TCM_default(std::string endpointParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    long long finalValue;
    string sequence, endpoint;
    void sequenceOperationWrite(long long num, std::string address, std::string& sequence);
    void sequenceOperationRMWAND(int num, std::string address, std::string& sequence);
    void sequenceOperationRMWOR(int num, std::string address, std::string& sequence);
    void sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence);
    long long parameterValue(std::string strValue);
};