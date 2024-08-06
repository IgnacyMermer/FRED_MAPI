#include "Fred/Mapi/indefinitemapi.h"

class TriggerRand: public IndefiniteMapi
{
private:
	void processExecution();

public:
	TriggerRand(std::string endpoint);
	virtual ~TriggerRand();

    std::string triggerEndpoint;
};
