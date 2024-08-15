#include "Fred/Mapi/indefinitemapi.h"

class ElectronicStatus: public IndefiniteMapi
{
private:
	void processExecution();

public:
	ElectronicStatus();
	virtual ~ElectronicStatus();
};