#include "Fred/Mapi/indefinitemapi.h"

class InitElectronics: public IndefiniteMapi
{
private:
	void processExecution();

public:
	InitElectronics();
	virtual ~InitElectronics();
};