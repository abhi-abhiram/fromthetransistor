#include <stdlib.h>
#include <stdint.h>

const struct Opcode
{
    const char *literal;
    const u_int8_t code;
};

const Opcode ADD = Opcode{"ADD", 0x00};
const Opcode ADDS = Opcode{"ADDS", 0x01};
const Opcode SUB = Opcode{"SUB", 0x02};
const Opcode SUBS = Opcode{"SUBS", 0x03};

const Opcode AND = Opcode{"AND", 0x20};
const Opcode ANDS = Opcode{"ANDS", 0x21};
const Opcode ORR = Opcode{"ORR", 0x22};
const Opcode ORRS = Opcode{"ORRS", 0x23};
const Opcode EOR = Opcode{"EOR", 0x24};
const Opcode EORS = Opcode{"EORS", 0x25};
const Opcode BIC = Opcode{"BIC", 0x26};
const Opcode BICS = Opcode{"BICS", 0x27};

const Opcode LOAD = Opcode{"LOAD", 0x40};
const Opcode STORE = Opcode{"STORE", 0x41};

char *nextToken(char *s, int len, int lastIndex)
{
    uint i = lastIndex;

    // skip  all spaces
    while (s[i] == ' ')
    {
        i++;
    }

    if (s[i] == '\n')
        return nullptr;

    uint8_t tokenLen = 0;
    uint8_t startIndex = i;
    for (i; i < len; i++)
    {
        tokenLen++;
    }

    char *token = (char *)malloc(sizeof(char) * tokenLen);

    token = s + startIndex;

    return token;
}

int main()
{

    char test[] = "ADD R1, R2, R3";

    return 0;
}