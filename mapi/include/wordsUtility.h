#include <vector>
#include <cmath>
#include <iostream>
#include <numeric>


class WordsUtility{
public:
    WordsUtility();

    static bool mapiMessage(std::string input){
        return input.length()>5&&input.substr(0,5)=="FRED,";
    }

    static bool checkValueWords(std::string endpoint, int64_t num, bool& isSigned, std::vector<std::vector<int64_t>> words){
        for(std::vector<int64_t> word : words){
            if(word[0]==1){
                return false;
            }
            //if signed there must be only one word in register
            if(word[3]==1){
                if(words.size()>1||num<word[4]||num>word[5]){
                    return false;
                }
                isSigned=true;
            }
            if(word[6]!=1&&(!isSigned)){
                uint32_t maskTemp = ((1<<(word[2]-word[1]+1))-1)<<word[1];
                uint32_t maskNum = (num&maskTemp)>>word[1];
                if(maskNum<word[4]||maskNum>word[5]){
                    return false;
                }
            }
        }
        return true;
    }

    static bool findCheckWord(std::string endpoint, int64_t num, int index, bool& isSigned, int& firstBit, int& lastBit, std::vector<std::vector<int64_t>> words){
        for(std::vector<int64_t> word : words){
            if(word[1]<=index&&word[2]>=index){
                if(word[0]==1){
                    return false;
                }
                if(word[3]==1){
                    isSigned=true;
                }
                if((num<word[4]||num>word[5])){
                    return false;
                }
                firstBit=word[1];
                lastBit=word[2];
                return true;
            }
        }
        return false;
    }

    static bool checkWord(std::string endpoint, int64_t num, std::vector<std::vector<int64_t>> words){
        for(std::vector<int64_t> word : words){
            if(word[1]<=num&&word[2]>=num){
                if(word[0]==1){
                    return false;
                }
                if(word[1]!=word[2]&&word[6]==0){
                    return false;
                }
                return true;
            }
        }
        return false;
    }

};