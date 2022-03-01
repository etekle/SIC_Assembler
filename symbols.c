#include "headers.h"

int IsAValidSymbol( char *TestSymbol ){

	int Result = 1;
	int maxlen = 6;
	int index = 0;
	int done = 0;
  
	while( !done ) {

		if (TestSymbol[index] == '\0'){
			break;
		}

		if ( index == maxlen ){
			Result = 0;
			done = 1;
		
		}
		index++;	

	}

  if(IsADirective(TestSymbol) || IsAnInstruction(TestSymbol))
    Result = 0;

	return Result;
}
