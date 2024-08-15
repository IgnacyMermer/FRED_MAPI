#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class BoardType: public Iterativemapi
{
public:
    BoardType();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int finalValue;
    string sequence;
};

/*#include "Fred/Mapi/indefinitemapi.h"

class BoardType: public IndefiniteMapi
{
private:
	void processExecution();

public:
	BoardType();
	virtual ~BoardType();
};
*/