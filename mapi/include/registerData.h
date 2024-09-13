#include <cstdint>
#include <iostream>
#include <map>
#include <vector>


struct RegisterData {
    std::map<std::string, std::string> addresses;
    std::map<std::string, std::vector<std::vector<int64_t>>> registerFields;
    std::map<std::string, std::pair<std::string, std::string>> equations;
    struct ActualValues {
        uint32_t SERIAL_NUM      : 8;
        uint32_t externalClock   : 1;
    } act;
    struct TempValues {
        int electronicStatus     :16;
    } temp;
};

extern RegisterData registersData;