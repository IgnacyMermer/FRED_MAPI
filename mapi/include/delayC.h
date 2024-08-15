#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class DelayC: public Iterativemapi
{
public:
    DelayC();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    string sequence;
};

/*#include "Fred/Mapi/indefinitemapi.h"

class DelayC: public IndefiniteMapi
{
private:
	void processExecution();

public:
	DelayC();
	virtual ~DelayC();
};
*/
