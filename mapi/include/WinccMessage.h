#include <vector>
#include <iostream>

class WinccMessage {
private:
    std::vector<std::string> parameters;

public:

    WinccMessage(std::string input);

    bool readMessage() const;
    bool writeMessage() const;
    bool orAndMessage() const;
    bool writeWordMessage() const;
    bool orMessage() const;
    int64_t getValueWriteMessage() const;
    int64_t getIndexAndOrWriteMessage() const;
    int64_t getValueWriteWordMessage() const;
};

