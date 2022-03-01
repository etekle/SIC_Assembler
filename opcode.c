#include "headers.h"


int opcode(char *Test){

    if(strcmp("ADD",Test) == 0)
      return 0x18;
    else if(strcmp("AND",Test) == 0)
      return 0x40;
    else if(strcmp("COMP",Test) == 0)
      return 0x28;
    else if(strcmp("DIV", Test) == 0)
      return 0x24;
    else if(strcmp("J",Test) == 0)
      return 0x3C; 
    else if(strcmp("JEQ",Test) == 0)
      return 0x30;
    else if(strcmp("JGT", Test) ==0)
      return 0x34;            
    else if(strcmp("JLT", Test) == 0)
      return 0x38;
    else if(strcmp("JSUB", Test) ==0)
      return 0x48;
    else if(strcmp("LDA", Test) == 0)
      return 0x00;
    else if(strcmp("LDCH",Test) ==0)
      return 0x50;
    else if(strcmp("LDL", Test) == 0)
      return 0x08;
    else if(strcmp("LDX",Test) == 0)
      return 0x04;
    else if(strcmp("MUL",Test) ==0)
      return 0x20;
    else if(strcmp("OR",Test) == 0)
      return 0x44;
    else if(strcmp("RD",Test) ==0)
      return 0xd8;
    else if(strcmp("RSUB",Test) ==0)
      return 0x4C;
    else if(strcmp("STA",Test) == 0)
      return 0x0C;
    else if(strcmp("STCH", Test) == 0)
      return 0x54; 
    else if(strcmp("STL",Test) == 0)
      return 0x14;
    else if(strcmp("STSW",Test) == 0)
      return 0xE8;
    else if(strcmp("STX",Test) == 0)
      return 0x10;
    else if(strcmp("SUB",Test) == 0)
      return 0x1C;
    else if(strcmp("TD",Test) ==0)
      return 0xE0;
    else if(strcmp("TIX",Test) ==0)
      return 0x2C;
    else if(strcmp("WD",Test) ==0)
      return 0xDC;
    else
    return 0;

}
