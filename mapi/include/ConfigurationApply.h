

#include "Fred/Mapi/indefinitemapi.h"

class ConfigurationApply: public IndefiniteMapi
{
private:
	void processExecution();

public:
	ConfigurationApply();
	virtual ~ConfigurationApply();
};
