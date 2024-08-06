/*#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class Triggers: public Iterativemapi
{
public:
    Triggers();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;
};*/


#include "Fred/Mapi/indefinitemapi.h"

class Triggers: public IndefiniteMapi
{
private:
	void processExecution();

public:
	Triggers();
	virtual ~Triggers();
};

