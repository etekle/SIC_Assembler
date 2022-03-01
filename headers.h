#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//#define DEBUG 1
//#define DEBUG2 1
//#define PRINT 1
#define PRINT2 1

struct symbols {
	int	DefinedOnSourceLine;
	int	Address; 
	char	Name[7];
 };

typedef struct symbols SYMBOL;

struct statement{
  char Name[7];
  char Mnemonic[8];
  char Operand[100];
  int Srcln;
};
typedef struct statement SOURCE;

/*struct opcodes {
	char	OpCode;	
	char	Name[8];
};

typedef struct opcodes OPCODES;

OPCODES OpcodeTable[ 32 ];
*/

int IsAValidSymbol( char *TestSymbol );
int IsAnInstruction(char *Test );
int IsADirective( char *Test );
int opcode( char *Test);
int symbolExists(struct symbols *tab[], char* sname);
void addSymbol(struct symbols* tab[], int addr,  int srcln, char* sName);
void addStatement(struct statement* stat[],char* name, char* mnemonic, char* operand, int srcln);
int findSymbol(struct symbols* tab[], char* symbol);

