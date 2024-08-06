#include "Fred/Mapi/indefinitemapi.h"

class PM_status: public IndefiniteMapi
{
private:
	void processExecution();

public:
	PM_status(std::string endpoint);
	virtual ~PM_status();

    std::string pmEndpoint;
};
