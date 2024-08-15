#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class TriggerSign: public Iterativemapi
{
public:
    TriggerSign(std::string endpoint);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence, triggerEndpoint;
};

/*#include "Fred/Mapi/indefinitemapi.h"

class TriggerSign: public IndefiniteMapi
{
private:
	void processExecution();

public:
	TriggerSign(std::string endpoint);
	virtual ~TriggerSign();

    std::string triggerEndpoint;
};*/
