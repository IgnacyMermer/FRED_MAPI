#include "Fred/Mapi/indefinitemapi.h"

class Default: public IndefiniteMapi
{
private:
	void processExecution();

public:
	Default(std::string endpointParam);
	virtual ~Default();

    std::string endpoint;
};
