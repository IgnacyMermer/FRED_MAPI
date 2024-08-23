#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class BoardStatus: public Iterativemapi
{
public:
    BoardStatus();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;
};

/*#include "Fred/Mapi/indefinitemapi.h"

class ActualValues: public IndefiniteMapi
{
private:
	void processExecution();

public:
	ActualValues();
	virtual ~ActualValues();
};

*/