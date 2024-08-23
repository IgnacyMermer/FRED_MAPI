#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class PM_default: public Mapi{
public:
    PM_default(std::string endpointParam, std::string pmNumberParam);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    long long finalValue;
    string sequence, endpoint, pmNumber;
    void sequenceOperation1(long long num, std::string address, std::string& sequence);
    void sequenceOperation2(int num, std::string address, std::string& sequence);
    void sequenceOperation3(int num, std::string address, std::string& sequence);
    void sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence);
    long long parameterValue(std::string strValue);
    std::string numberLetter(int number);
};