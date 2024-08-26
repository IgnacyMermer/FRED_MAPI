#include "Fred/Mapi/indefinitemapi.h"

class InitGBT: public IndefiniteMapi
{
private:
	void processExecution();

public:
	InitGBT();
	virtual ~InitGBT();
};