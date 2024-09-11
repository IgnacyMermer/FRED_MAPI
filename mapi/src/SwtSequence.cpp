#include "SwtSequence.h"
#include<cstring>
#include<charconv>

namespace fit_swt
{

SwtSequence& SwtSequence::addOperation(Operation type, uint32_t address, uint32_t* data, bool expectResponse=true)
{
    std::string saddress = wordToHex(address);
    return addOperation(type, saddress.c_str(), data, expectResponse);
}

SwtSequence& SwtSequence::addOperation(Operation type, const char* address, uint32_t* data, bool expectResponse=true)
{
    switch(type)
    {
        case Operation::Read:
        {
            m_buffer.append(_READ_PREFIX_);
            m_buffer.append(address);
            m_buffer.append("0000");
            m_buffer.append(_FRAME_POSTFIX_);

            if(expectResponse)
            {
                m_buffer.append(_READ_WORD_);
            }
        }
        break;

        case Operation::Write:
        {
            m_buffer.append(_WRITE_PREFIX_);
            m_buffer.append(address);
            m_buffer.append(wordToHex(data[0]));
            m_buffer.append(_FRAME_POSTFIX_);

        }
        break;

        case Operation::RMWbits:
        {
            m_buffer.append(_RMW_BITS_AND_PREFIX_);
            m_buffer.append(address);
            m_buffer.append(wordToHex(data[0]));
            m_buffer.append(_FRAME_POSTFIX_);

            if(expectResponse)
            {
                m_buffer.append(_READ_WORD_);
            }

            m_buffer.append(_RMW_BITS_OR_PREFIX_);
            m_buffer.append(address);
            m_buffer.append(wordToHex(data[1]));
            m_buffer.append(_FRAME_POSTFIX_);
        }

        break;

        case Operation::RMWsum:
        {
            m_buffer.append(_RMW_SUM_PREFIX_);
            m_buffer.append(address);
            m_buffer.append(wordToHex(data[0]));
            m_buffer.append(_FRAME_POSTFIX_);

            if(expectResponse)
            {
                m_buffer.append(_READ_WORD_);
            }
        }
        break;
    }

    return *this;
}


std::string SwtSequence::wordToHex(uint32_t word)
{
    return {
        halfByteToHex((word >> 28) & 0x0F),
        halfByteToHex((word >> 24) & 0x0F),


        halfByteToHex((word >> 20 )& 0x0F),
        halfByteToHex((word >> 16) & 0x0F),


        halfByteToHex((word >> 12) & 0x0F),
        halfByteToHex((word >> 8) & 0x0F),

        halfByteToHex((word >> 4) & 0x0F),
        halfByteToHex(word & 0x0F)
    };
}



}// fir_swt

