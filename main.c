
#include "headers.h"

int main( int argc, char* argv[]){
  FILE *fp = NULL;
	char line[1024];
	char* newsym;
  char* newsym_m;
	char* nextoken;
  char* nextoken_m;
  char* nexinpt;
  char* nexinpt_m;
  int adrss = 0;
  int srcln = 0;
  char fullline[1024];

  struct symbols* symtab[1024];
  memset(symtab, '\0', 1024 * sizeof(struct symbols*));	

  struct statement* statab[1024];
  memset(statab, '\0', 1024 * sizeof(struct statement*));	

	if ( argc != 2 ) {
	printf("ERROR: Usage: %s filename\n", argv[0]);
	return 0;
	}

#ifdef Debug
  printf("DEBUG: Passed the command-line arguments tests\n");
#endif

	fp = fopen( argv[1], "r");

	if ( fp == NULL ) {
	printf("ERROR: %s could not be opened for reading,\n", argv[1] );
	return 0;
	}

	newsym = malloc(  1024 * sizeof(char) );	
	memset( newsym, '\0', 1024 * sizeof(char) );
        newsym_m = newsym;
	nextoken = malloc(  1024 * sizeof(char)           );	
	memset( nextoken, '\0', 1024 * sizeof(char) );
        nextoken_m = nextoken;
        nexinpt = malloc(  1024 * sizeof(char)           );	
	memset( nexinpt, '\0', 1024 * sizeof(char) );
        nexinpt_m = nexinpt;
  
	while(  fgets( line , 1024 , fp ) != NULL && adrss < 0x7FFF) {

    srcln++;

		strcpy( fullline, line );
		if ( line[0] == 35) {  

#ifdef DEBUG 
  		printf("COMMENT:%s", line );
#endif  
			continue;
		}   

		if (  (line[0] >= 65 ) && ( line[0] <= 90 )   )  {
			newsym = strtok( line, " \t\n");

#ifdef DEBUG
			printf("FULL LINE:%s", fullline );
#endif
			nextoken = strtok( NULL, " \t\n"  );
      nexinpt = strtok(NULL, "\t\n"  );

#ifdef DEBUG        
    printf("NEW SYMBOL : %s\n",newsym);
#endif    

      if ( IsAValidSymbol(newsym) == 0 ) {

        printf("%sERROR: LINE %d, INVALID SYMBOL. \r\n",fullline, srcln);
        if(IsADirective(newsym))
          printf("\'%s\' IS A DIRECTIVE.\r\n", newsym);
        else if(IsAnInstruction(newsym))
          printf("\'%s\' IS AN INSTRUCTION.\r\n", newsym);
        
        return 0;
      }      
      if(symbolExists(symtab, newsym) == 0) {
#ifdef DEBUG
        printf("Is A Valid Symbol Is %d\n", IsAValidSymbol( newsym ) );
#endif
        addSymbol(symtab, adrss = (strcmp(nextoken, "START") == 0) ? strtol(nexinpt, NULL, 16) : adrss , srcln, newsym);

      }
      else if  (symbolExists(symtab, newsym) == -1){
       printf("%sERROR: LINE %d, SYMBOL NAME ALREADY USED. \r\n",fullline, srcln);
        return 0;
      }    

  
#ifdef DEBUG		  
			printf("NEXT TOKEN ON LINE IS %s\n", nextoken );
#endif

      if (IsADirective( nextoken ) == 1 ) {
#ifdef DEBUG
        printf("'%s' IS A DIRECTIVE\n", nextoken );
#endif

        if (strcmp(nextoken, "START") == 0){

          if(strtol(nexinpt, NULL, 16) >= 0x7FFC){
            printf("%sERROR: LINE %d, NO ROOM LEFT IN MEMORY. \r\n",fullline, srcln); 
            return 0;
           }
          
          adrss = strtol(nexinpt, NULL, 16);
        }
        else if(strcmp(nextoken, "WORD") == 0){
          if(strtol(nexinpt,NULL, 0) > pow(2,23))
          {
            printf("%sERROR: LINE %d, WORD CONSTANT EXCEEDS BIT LIMITATION. \r\n",fullline, srcln);
            return 0;
          }
          adrss += 3;
        }
        else if(strcmp(nextoken, "BYTE") == 0) {          
          int i = 2;

          if(nexinpt[1] == '\'')   {
            
            if(nexinpt[0] == 'C') {
              while(nexinpt[i] != '\'' && nexinpt[i] != '\0')
                i++;            
              if(nexinpt[i] == '\'')
                adrss += i - 2;
              else{
                printf("%sERROR: LINE %d, CHARACTER CONSTANT INCOMPLETE. \r\n",fullline, srcln);
                return 0;
              }
            }
            else if(nexinpt[0] == 'X') {
          
              int count = 0;
              
              while(nexinpt[i] != '\'' && nexinpt[i] != '\0'){
                count++;
                i++;
                
                if(toupper((unsigned char)nexinpt[i-1]) > 'F' || nexinpt[i-1] < '0'){
                   printf("%sERROR: LINE %d, INVALID HEX CONSTANT. \r\n",fullline, srcln);
                   return 0;
                }
              }
              if(count % 2 == 1){
                printf("%sERROR: LINE %d, INVALID HEX CONSTANT. \r\n",fullline, srcln);
                return 0;
              }
              else if(nexinpt[i] == '\'' && (i-2) % 2 == 0)
                adrss += (i-2)/2;
              else if(nexinpt[i] == '\0'){
                printf("%sERROR: LINE %d, HEX CONSTANT INCOMPLETE. \r\n",fullline, srcln);
                return 0;
              }              
              
            }
          }
          else{
            printf("%sERROR: LINE %d, INVALID CONSTANT PARAMETERS. \r\n",fullline, srcln);   
            return 0;          
          }

        } 
        else if (strcmp(nextoken, "RESB") == 0)
          adrss += strtol(nexinpt, NULL, 0);
        else if (strcmp(nextoken, "RESW") == 0)
          adrss += (strtol(nexinpt, NULL, 0) * 3);
#ifdef DEBUG
        printf("The Location Counter is set to %X\n\n", adrss);
#endif
      }
      else if(IsAnInstruction( nextoken ) == 1 ) {
#ifdef DEBUG
        printf("'%s' IS AN INSTRUCTION\n", nextoken );
#endif
        adrss += 3;
#ifdef DEBUG        
        printf("The Location Counter is set to %X\n\n", adrss);
#endif
      }
      else{
        printf("%sERROR: LINE %d, INVALID TOKEN. \r\n",fullline, srcln);
        return 0;
      }

      addStatement(statab,newsym,nextoken,nexinpt == NULL ? "0":nexinpt,srcln);

			continue;
    }
    else if((line[0] < 65  ||  line[0] > 90) && line[0] > 32)  {
	    printf("%s ERROR: LINE %d, INVALID SYMBOL. \r\n",fullline, srcln);
      return 0;
    }
    else if(strtok(line, " \n\t\r") == NULL)
    {
      printf("%sERROR: LINE %d, BLANK LINES NOT ALLOWED. \r\n",fullline, srcln);
      return 0;
    }    
    
    nextoken = strcmp(strtok(line, " \t\n"  ),"RSUB") == 0?strtok(line, " \t\n"  ):strtok(fullline, " \t\n"  );
    nexinpt = strtok(NULL, " \t\n"  );
    printf("%st%st%st\n",newsym, nextoken,nexinpt);
    addStatement(statab,"0",nextoken,(nexinpt == NULL) ?"0":nexinpt,srcln);
    if(IsAnInstruction(nextoken) == 1){
      adrss += 3;
 
#ifdef DEBUG
		printf("%s, %x\n", line, adrss );
#endif
    }

	}

  free(newsym_m);
  free(nexinpt_m);
  free(nextoken_m);

  if(adrss >= 0x7FFF){
    printf("%sERROR: LINE %d, NO ROOM LEFT IN MEMORY. \r\n",fullline, srcln); 
    return 0;
  }

#ifdef DEBUG2
  
   for(int i =0 ;statab[i] != NULL;i++)
    printf("%s %s %s\n",statab[i]->Name,statab[i]->Mnemonic,statab[i]->Operand);
#endif



  int nline[1024];
  memset(nline, '\0', 1024 * sizeof(int));

  for (int i = 0; statab[i] != NULL; i++){
    if(strcmp(statab[0] -> Mnemonic,"START") != 0)
    {
       printf("Missing START DIRECTIVE");
       return 0;
    }
    
    if(IsAnInstruction(statab[i] -> Mnemonic) == 1 && symbolExists(symtab,strtok(statab[i] -> Operand, ",\t\n")) == 0 && strcmp(statab[i] -> Mnemonic,"RSUB") != 0){
      printf("%s  %s %s",strcmp(statab[i] -> Name,"0")==0?" ":statab[i]->Name, statab[i] -> Mnemonic, statab[i] -> Operand);
      printf("\nError: Symbol '%s' Line %d Not Found In Symbol Table\r\n", strtok(statab[i]-> Operand,","), statab[i] -> Srcln);
      return 0;
    }
        
  }

#ifdef PRINT 
  for (int i = 0; symtab[i] != NULL; i++){
    printf("%s \t\t %X\r\n",symtab[i] -> Name, symtab[i] -> Address);
  }        
#endif

#ifdef PRINT2

  FILE *fpw = NULL;
  char *file =  strncat(argv[1], ".obj", 5);
  int k = 1;
  int p = 0;

  for(int i = 1; statab[i] != NULL; i++){
    if(IsADirective(statab[i]->Mnemonic) == 0){
      if(strcmp(statab[i] -> Mnemonic,"RSUB")==0)
        nline[p] = 0;
      else
        nline[p] = findSymbol(symtab, strtok(statab[i]-> Operand,","));
      p++;
    }
  }

  fpw = fopen(file, "w");

  printf("H%s%8.6X%.6X\n",(symtab[0] -> Name),symtab[0]-> Address, adrss - symtab[0]-> Address);


  for (int i = 1; statab[i+1] != NULL; i++){    
      int m=2;      

      if(strcmp(statab[i] -> Name, "0") != 0){
        
        if(IsADirective(statab[i] -> Mnemonic) == 1 && strcmp(statab[i] -> Mnemonic,"WORD") != 0 && strcmp(statab[i] -> Mnemonic,"BYTE") != 0){
          k++;
          continue;
        }

        printf("T");

        printf("%.6X",symtab[k] -> Address);
        if(strcmp(statab[i] -> Mnemonic, "WORD" ) == 0){
          printf("03%.6X\n", (int)strtol(statab[i] -> Operand,NULL, 10));      
        }
        else if(strcmp(statab[i] -> Mnemonic, "BYTE" ) == 0){
          int n = 2;
          
           
          if(statab[i] -> Operand[0] == 'C') {
            while(statab[i] -> Operand[m] != '\'' && statab[i] -> Operand[m] != '\0')
              m++;

            m -=2 ;

            adrss = symtab[k] -> Address;

            if(m <= 29){
              printf("%.2X", m);
              while(statab[i] -> Operand[n] != '\'' && statab[i] -> Operand[n] != '\0'){
                printf("%.2X",statab[i] -> Operand[n]);
                n++;            
              }

            }
            else{
              printf("%.2X", 30);

              while(m > 30){                

                for(int k = 0; statab[i] -> Operand[n] != '\'' && k <= 29 && statab[i] -> Operand[n] != '\0';k++){
                  printf("%.2X",statab[i] -> Operand[n]);
                  n++;            
                }
                  
                m -= 30;                 
  
                if(m > 30){
                  adrss += 30;
                  printf("\nT%.6X%.2X",adrss, 30);
                }
                
              }

              if(m != 0){

                printf("\nT%.6X%.2X",adrss + m, m);

                while(statab[i] -> Operand[n] != '\'' && statab[i] -> Operand[n] != '\0'){
                  printf("%.2X",statab[i] -> Operand[n]);
                  n++;            
                }
              }
            }

            printf("\n");
          }
          else if(statab[i] -> Operand[0] == 'X') {
            while(statab[i] -> Operand[m] != '\'' && statab[i] -> Operand[m] != '\0'){
              m++;
              }
            printf("%.2X%.2X\n",(m-2)/2,(int)strtol( strtok(&strtok(statab[i] -> Operand, "\'")[2], "'"),NULL, 16));
          }
          
        }
        else{
        printf("03%.2X",opcode(statab[i] -> Mnemonic));
          int a = 1;
          while(symtab[a+1] != NULL && (strcmp(statab[i] -> Operand, symtab[a] -> Name) != 0))
            a++;

          printf("%.4X\n",symtab[a]-> Address);
        } 
        
        k++;        
      }
      else if(strcmp(statab[i] -> Name, "0") == 0){
       int m = i;
        printf("T");
        printf("%.6X", symtab[k-1] -> Address + 0x03);
        while(strcmp(statab[m] -> Name,"0") == 0)
          m++;
        printf("%.2X",(m-i) * 0x03);
    
        m = i;
        while(strcmp(statab[m] -> Name,"0") == 0){
          printf("%.2X",opcode(statab[m] -> Mnemonic));
          int a = 1;      
          
          if(strcmp(statab[m] -> Mnemonic, "STCH") == 0 || strcmp(statab[m] -> Mnemonic, "LDCH") == 0){
            while(strcmp(strtok(statab[m] -> Operand, " ,"), symtab[a] -> Name) != 0)
              a++;  
            printf("%.4X",(symtab[a] -> Address) + 0x8000);         
          }
          else{ 

            printf("%.4X", strcmp(statab[m] -> Operand, "0") == 0 ? 0x00 : findSymbol(symtab,strtok(statab[m] -> Operand,",")));
           
          }
           m++;
        } 
        printf("\n");
        i = m-1;
        m = 2;
        
      }
           
    }

 for(int i = 0; line[i] != '\0';i++ ){
    printf("M%.6X04+%s\n",nline[i],symtab[0]->Name);
  }

  for (int i = 0; statab[i] != NULL; i++){    
    if(IsAnInstruction(statab[i] -> Mnemonic) == 1){
      for(int j = 0; strcmp(statab[i] ->Name, symtab[j] -> Name) != 0; j++){
        printf("E%.6X\n",symtab[j]-> Address);
      }
        break;
    }
  }

  printf("File '%s' Has Been Created\n",file);

#endif
	fclose( fp );

}
int symbolExists(struct symbols* tab[], char* sname){
  
  int index = 0;
    
  while(tab[index] != NULL)
  {
     
    if(strcmp(sname, tab[index] -> Name) == 0){
      return -1;
      break;
    }
    index++;  
  }
  return 0;

}

void addSymbol(struct symbols* tab[], int addr,  int srcln, char* sName){
  int index = 0;
  struct symbols* newsym;
  newsym = malloc(1024 * sizeof(struct symbol*));
  newsym -> Address = addr;
  newsym -> DefinedOnSourceLine = srcln;
  strcpy(newsym -> Name, sName);
  
  while(tab[index] != NULL)
    index++;

  tab[index] = newsym;
}

void addStatement(struct statement* stat[], char* name, char* mnemonic, char* operand, int srcln){
  int index = 0;  
  struct statement* newstat;
  newstat = malloc(1024 * sizeof(struct statement*));
  strcpy(newstat -> Name, strtok(name, " \t\n"));
  strcpy(newstat -> Mnemonic, strtok(mnemonic," \t\n"));
  strcpy(newstat -> Operand, strtok(operand, " \t\n"));
  newstat -> Srcln = srcln;
  
  

  while(stat[index] != NULL){
    index++;
  }

  stat[index] = newstat;
}

int findSymbol(struct symbols* tab[], char* symbol){
  int index;
  for(index = 0;tab[index] != NULL && strcmp(tab[index] -> Name, symbol) != 0 ;index++){}

  return tab[index] -> Address;
}
