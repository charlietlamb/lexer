/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Charles Lamb
Student ID: 201490756
Email: sc21cl@leeds.ac.uk
Date Work Commenced: Feb 2023
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

#include <stdbool.h>
#include "dirent.h"

//define memory for malloc/realloc
#define MORE_MEMORY 1024

//define total lines as global variable
//reset totalLines each time

Token** masterArray;
int totalFiles=0;
int counter = 0;
int charsCounter;
bool setString = false;
bool setDigit = false;

//used for accessing masterArray
int fileCount =0;
int tokenCount =0;



// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  main();
  return 0;
}


// Get the next token from the source file
Token GetNextToken ()
{
	Token t = masterArray[fileCount][tokenCount];
  tokenCount++;
  if(t.tp==EOFile){
    fileCount++;
  }
  return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t = masterArray[fileCount][tokenCount];
  t.tp = ERR;
  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
	return 0;
}

// do not remove the next line
#ifndef TEST


int main(int argc, char *argv[])
{
  // implement your main function here
  // NOTE: the autograder will not use your main function
    char* path = malloc(sizeof(char*) * MORE_MEMORY);
    strcpy(path,argv[1]);
    

    char** dir = getDir(path);
    char** tokens;
    masterArray = malloc(sizeof(Token*) * totalFiles*1024);
    int masterCount =0;
    tokens = malloc(sizeof(char*) * MORE_MEMORY);
    for(int j=0;j<totalFiles;j++){
        //printf("\nNEW FILE\n\n");
        char** file;
        char name[1024];
        char * folder = argv[1];
        folder[strlen(folder)] = '\0';
        strcpy(name,folder);
        strcat(name,"/");
        char temp[1024];
        strcpy(temp,dir[j]);
        strcat(name,temp);
        file = getStrings(name);
        
        char* tokTemp = malloc(sizeof(getTokens(file,counter,dir[j]))+1);
        
        
        tokTemp = getTokens(file,counter,dir[j]);
        //printf("%s",tokTemp);
        //strcpy(tokens[j],tokTemp);
        int lineCount =0;
        int stringLength = strlen(tokTemp);
        for(int i=0;i<stringLength;i++){
          if(tokTemp[i]=='\n'){
            lineCount++;
          }
        }
        Token *tokenArray = malloc(sizeof(Token) * lineCount);
        tokenArray = make(tokTemp);
        counter = 0;
        charsCounter = 0;
        masterArray[masterCount] = tokenArray;
        masterCount++;
    }


}
// do not remove the next line
#endif

Token *make(char* tokens){
  int lineCount =0;
  int stringLength = strlen(tokens);
  for(int i=0;i<stringLength;i++){
    if(tokens[i]=='\n'){
      lineCount++;
    }
  }
  Token *tokenArray = malloc(sizeof(Token) * lineCount);
  char *saveString, *saveAtt;
  int tokenCount =0;
  char delim[2] = "\n";
  char* string;
  string = strtok_r(tokens,delim,&saveString);
  int len = strlen(string);
  string[len-2] = '\0';
  string +=2;
  while(string != NULL){
    if(string != NULL){
      Token t;
      int len = strlen(string);
      string[len-2] = '\0';
      string+=2;
      char* att; //current attribute
      char delim2[3] = ", ";
      att = strtok_r(string, delim2,&saveAtt);
      if(att != NULL){
        strcpy(tokenArray[tokenCount].fl,att);
        att = strtok_r(NULL, delim2,&saveAtt);
      }
      if(att != NULL){
        tokenArray[tokenCount].ln = atoi(att);
        att = strtok_r(NULL, delim2,&saveAtt);
      }
      if(att != NULL){
        strcpy(tokenArray[tokenCount].lx,att);
        att = strtok_r(NULL, delim2,&saveAtt);
      }
      att = strtok_r(NULL, delim2,&saveAtt);
      if(att != NULL){

        if(strcmp(att,"RESWORD")){
          tokenArray[tokenCount].tp = RESWORD;
        }
        if(strcmp(att,"ID")){
          tokenArray[tokenCount].tp = ID;
        }
        if(strcmp(att,"INT")){
          tokenArray[tokenCount].tp = INT;
        }
        if(strcmp(att,"SYMBOL")){
          tokenArray[tokenCount].tp = SYMBOL;
        }
        if(strcmp(att,"STRING")){
          tokenArray[tokenCount].tp = STRING;
        }
        if(strcmp(att,"EOFile")){
          tokenArray[tokenCount].tp = EOFile;
        }
        if(strcmp(att,"ERR")){
          tokenArray[tokenCount].tp = ERR;
          if(strcmp(tokenArray[tokenCount].lx,"Error: unexpected eof in comment")){
            tokenArray[tokenCount].ec = 1;
          }
          if(strcmp(tokenArray[tokenCount].lx,"Error: unexpected eof in string constant")){
            tokenArray[tokenCount].ec = 2;
          }
          if(strcmp(tokenArray[tokenCount].lx,"Error: unexpected eof in string constant")){
            tokenArray[tokenCount].ec = 3;
          }
          if(strcmp(tokenArray[tokenCount].lx,"Error: illegal symbol in source file")){
            tokenArray[tokenCount].ec = 4;
          }
        }else{
          tokenArray[tokenCount].ec = 0;
        }
      }

    }
    tokenCount++;
    string = strtok_r(NULL,delim,&saveString);
  }
  return tokenArray;
}

char* getTokens(char**file,int limit, char* path){
    
    int lineNum = 0;
    int charNum = 0;
    char* line; //current line
    char current[1024] = "\0"; //current string
    bool start = true; //start of line?
    char type[1024] = "init"; //type of token?
    bool typeSame = true; //are the characters the same type?
    char thisChar = ' ';
    char nextChar = ' ';
    bool firstDigit = false; //true if first char is a digit
    char endChar[1024] = "init"; //if string or comment that need to find end char
    bool stringStart = false; //checks if token is a string
    bool tokenDone = false; //if end of token
    char temp[1024] = ""; //used for adding \n to last line of file
    char* tokens; //stores tokens in file
    tokens = malloc(sizeof(char*) * MORE_MEMORY*8); //allocates memory to token array
    strcpy(tokens,"");
    int tokenIndex = 0; //used for indexing tokens
    bool commentMode = false;
    bool finished; //checks if current token has finished creating
    bool skip = false; //skip as line has ended.
    char nextNextChar = ' '; //used to check for //
    char *tokenString = malloc(sizeof(char)*1024);
    bool longComment = false;
    bool EOFile = false;
    bool error = false;
    int stringCount = 0; //used to see when "" start and close

    for(int i=0;i<limit+1;i++){
        if (i==0){
            line = (*file);
        }else{
            //gets line of file
            line = (*file++);
        }
        //prevents eof error
        
        //checks that line exists
        
        if(line!=NULL){
            if(!strchr(line,'\n')){
                strcat(line,"\n");
            }
            if(!strchr(current,'\0')){
                strcat(current,"\0");
            }
        
            int j = 0;
            thisChar = 'a';
            nextChar = 'a';
            //iterates through characters in line
            skip = false;
            if(line[0] == '"'){
                stringCount=1;
            }
            if(isSymbolRet(line[0])&&(line[0] != '/'&& line[0] != '*')&&(!commentMode)){
                char* add;
                char *tokenString = malloc(sizeof(char)*1024);
                char tempStr[2] = { line[0], '\0'};
                add = createToken(tempStr,path,lineNum,getType(current, nextChar, stringStart, longComment, EOFile),tokenString);
                strcat(tokens,add);
                free(tokenString);
                strcpy(current,"\0");
                tokenIndex++;
            }
            while(thisChar != '\n'&&thisChar != '\0'){ //checks line hasn't finished
                if(i==limit-1){
                    if(!strchr(line,'\n')){
                        line[strlen(line)] = '\0';
                    }else{
                        if(!strchr(line,'\0')){
                            line[strlen(line)] = '\0';
                        }
                    }
                    
                    
                }
                thisChar = line[j];
                if(thisChar!='\n'){
                    nextChar = line[j+1];
                    if(nextChar!='\n'){
                        nextNextChar = line[j+2];
                    }else{
                        nextNextChar = '\0';
                    }
                }else{
                    skip = true;
                }
                if(thisChar == '/'&& nextChar == '/'){
                    skip = true;
                    thisChar = '\n';
                }
                if(thisChar == '/'&& nextChar == '*'){
                    commentMode = true;
                    longComment = true;
                }
                if(commentMode){
                    if(thisChar == '*'&& nextChar == '/'){
                        commentMode = false;
                        longComment = false;
                        skip = false;
                    }else{
                        skip = true;
                    }  
                }
                //checks if the first character is a digit
                
                if(!skip){
                    if(isdigit(current[0])){
                        firstDigit = true;
                    }else{
                        firstDigit = false;
                    }
                    
                    //checks if the token is a string
                    if(thisChar =='"'){
                        stringCount +=1;
                    }
                    finished = isFinished(thisChar, nextChar,firstDigit,stringCount,nextNextChar); //checks if nextChar
                    if(stringCount == 2){
                        stringCount = 0;
                    }
                    //is part of the same token as thisChar
                    
                    if(stringStart){
                        if(nextChar != '"'){
                            if(!strchr(current,'\0')){
                                strcat(current,"\0");
                            }
                            int len = strlen(current);
                            current[len] = thisChar;
                            current[len+1] = nextChar;
                            current[len+2] = '\0';
                        }else{
                            stringStart = false;
                            char* add;
                            char *tokenString = malloc(sizeof(char)*1024);
                            add = createToken(current,path,lineNum,getType(current, nextChar, stringStart, longComment, EOFile),tokenString);
                            strcat(tokens,add);
                            tokens = realloc(tokens, sizeof(tokens)+sizeof(add));
                            free(tokenString);
                            tokenIndex++;
                            strcpy(current,"\0");
                        }
                    }else{
                        
                        if(!finished){ //if they're the same type then add to current token
                            
                            if((!strcmp(current,"//"))||((nextChar == '/')&&(nextNextChar == '/'))){ //checks if the current token is a line comment
                                thisChar = '\n';
                            }
                            if((!strcmp(current,"/*"))){ //checks if the current token is a line comment
                                commentMode = true;
                                longComment = true;
                            }
                            //add char to end of string
                            
                            bool whiteSpace = false;
                            if(!strchr(current,'\0')){
                                strcat(current,"\0");
                            }
                            for(int k;k<strlen(current);k++){
                                if (current[k] ==' '){
                                    whiteSpace = true;
                                }
                            }
                            if(stringCount == 1){
                                int len = strlen(current);
                                current[len] = nextChar;
                                current[len+1] = '\0';
                            }else{

                                if ((nextChar!=' '|| (whiteSpace))){
                                    if(!strchr(current,'\0')){
                                        strcat(current,"\0");
                                    }
                                    if(!strchr(current,'\0')){
                                        strcat(current,"\0");
                                    }
                                    int len = strlen(current);
                                    if(j==0 && nextChar != ' '){
                                        current[len] = thisChar;
                                        current[len+1] = nextChar;
                                        current[len+2] = '\0';
                                    }else{
                                        if(j==0){
                                            if(!strchr(current,'\0')){
                                                strcat(current,"\0");
                                            }
                                            len = strlen(current);
                                            current[len] = thisChar;
                                            current[len+1] = '\0'; 
                                        }else{
                                            if(!strchr(current,'\0')){
                                                strcat(current,"\0");
                                            }
                                            len = strlen(current);
                                            current[len] = nextChar;
                                            current[len+1] = '\0'; 
                                        }
                                    }
                                    if(isSymbolRet(nextNextChar)){
                                        char* add;
                                        char *tokenString = malloc(sizeof(char)*1024);
                                        add = createToken(current,path,lineNum,getType(current, nextChar, stringStart, longComment, EOFile),tokenString);
                                        strcat(tokens,add);
                                        free(tokenString);
                                        tokenIndex++;
                                        strcpy(current,"\0");
                                    }
                                }
                            }
                        
                        }else{
                            
                            //if white space or if the two 
                            //characters are of different types e.g. e and +
                            if(stringStart){
                                stringStart = false;
                            }
                            if(!strchr(current,'\0')){
                                strcat(current,"\0");
                            }
                            char* newString = (char*) malloc(strlen(current));
                            int j = 0;
                            for (int i = 0; i < strlen(current); i++) {
                                if (current[i] != '\n') {
                                    newString[j] = current[i];
                                    j++;
                                }
                            }
                            strcpy(current,newString);
                            
                            if(create(current)){
                                char* add;
                                char *tokenString = malloc(sizeof(char)*1024);
                                add = createToken(current,path,lineNum,getType(current, nextChar, stringStart, longComment, EOFile),tokenString);
                                strcat(tokens,add);
                                free(tokenString);
                                //strcpy(current,"");
                                tokenIndex++;
                                
                                strcpy(current,"\0");
                            }else{ //is symbol
                                if(nextChar != ' '){
                                    if(nextChar == '/' && nextNextChar == '/'){
                                        thisChar = '\n';
                                    }else if (nextChar == '/' && thisChar == '*')
                                    {
                                        //if there is an end of multi line comment
                                        strcpy(current,"\0");
                                    }else{
                                        strcpy(current,"\0");
                                        if(!strchr(current,'\0')){
                                            strcat(current,"\0");
                                        }
                                        int len = strlen(current);
                                        current[len] = nextChar;
                                        current[len+1] = '\0';
                                        char* add;
                                        char *tokenString = malloc(sizeof(char)*1024);
                                        add = createToken(current,path,lineNum,getType(current, nextChar, stringStart, longComment, EOFile),tokenString);
                                        strcat(tokens,add);
                                        free(tokenString);
                                        strcpy(current,"\0");
                                        tokenIndex++;
                                    }
                                }
                            }
                        }
                    }
                }
            j++;
            }
        
        charNum++;
        }
    lineNum++;
    }
    char *tokenstring = "";
    if(stringCount == 1){         
        char* add;
        char *tokenString = malloc(sizeof(char)*1024);
        add = createToken("Error: unexpected eof in string constant",path,lineNum,"ERR",tokenString);
        strcat(tokens,add);
        free(tokenString);
        tokenIndex++;
        strcpy(current,"\0");
        return tokens;
    }
    if(longComment){
        char* add;
        char *tokenString = malloc(sizeof(char)*1024);
        add = createToken("Error: unexpected eof in comment",path,lineNum,"ERR",tokenString);
        strcat(tokens,add);
        free(tokenString);
        tokenIndex++;
        strcpy(current,"\0");
        return tokens;
    }
    char* add;
    char *tokenString2 = malloc(sizeof(char)*1024);
    add = createToken("End of File",path,lineNum,"EOFile",tokenString2);
    strcat(tokens,add);
    free(tokenString);
    strcpy(current,"\0");
    tokenIndex++;
    return tokens;
}

bool create(char* current){
    if(strchr(current, '\n')){
        return false;
    }
    if(!strchr(current,'\0')){
        strcat(current,"\0");
        }
    if(current==NULL){
        strcpy(current,"\0");
    }
    int length = strlen(current);
    bool empty = true;
    for(int i=0; i<length;i++){
        if (isdigit(current[i])||isalpha(current[i])){
            empty = false;
        }   
    }
    if(empty){
        return false;
    }else{
        return true;
    }
    return false;
}

bool isFinished(char thisChar, char nextChar, bool firstDigit, int stringCount, char nextNextChar){ //checks if token is finished
    
    
    if(stringCount==2){
        return true;
    }
    if(stringCount != 1){
        if(nextChar==' '){ //checks for white space
            return true;
        }
        if(firstDigit){
            if(thisChar == '(' || thisChar == ')'){
                return true;
            }
            if(isdigit(thisChar)){
                return false;
            }else{
                return true;
            }
        }

        for(int i=0;i<symbolSize;i++){
            if(symbolsChar[i]==nextChar){
                return true;
            }
        }
    }
    
    return false;
}

bool isSymbolRet(char thisChar){
    for(int i=0;i<symbolSize;i++){
        if(symbolsChar[i]==thisChar){
            return true;
        }
    }
    return false;
}

char* getType(char* current, char nextChar, bool stringStart, bool longComment, bool EOFile){ //gets type of token str
    for(int i=0;i<keywordSize;i++){
        if(!strcmp(current,keywords[i])){
            return "RESWORD";
        }
    }
    for(int i=0;i<symbolSize;i++){
        if(!strcmp(current,symbols[i])){
            return "SYMBOL";
        }
    }
    bool integer = true;
    if(!strchr(current,'\0')){
        strcat(current,"\0");
    }
    if(current==NULL){
        strcpy(current,"\0");
    }
    for(int i=0;i<strlen(current);i++){
        if(!isdigit(current[i])){
            integer = false;
        }
    }
    if(integer){
        return "INT";
    }

    if((stringStart) && (nextChar =='\n'|| EOFile)){
        return "ERR_STR";
    }
    if((longComment) && EOF){
        return "ERR_CMT";
    }
    if(nextChar=='?'){
      return "ERR_ILL";
    }
    return "ID";
}

char* createToken(char* current,char* file, int lineNumber, char* tokenType, char *tokenString){
    if(!strcmp(tokenType,"ERR_STR")){
        current = "Error: unexpected eof in string constant\0";
        tokenType = "ERR";
    }
    if(!strcmp(tokenType,"ERR_ILL")){
        current = "Error: illegal symbol in source file\0";
        tokenType = "ERR";
    }
    if(!strcmp(tokenType,"ERR_CMT")){
        current = "Error: unexpected eof in comment\0";
        tokenType = "ERR";
    }
    if (current[0]== '"'){
        current++;
        int len = strlen(current);
        current[len-1] = '\0';
        tokenType = "STRING";
    }
    char newString[1024];
    int counter =0;
    if(!strchr(current,'\0')){
        strcat(current,"\0");
    }
    if(current==NULL){
        strcpy(current,"\0");
    }
    for(int i=0;i<strlen(current);i++){
        if(current[i]=='\r'||current[i]=='\t'){

        }else{
            newString[counter] = current[i];
            counter++;
        }
    }
    newString[counter] = '\0';

    strcpy(tokenString,"< ");
    strcat(tokenString,file);
    strcat(tokenString,", ");
    char intStr[1024];
    sprintf(intStr, "%d", lineNumber);
    strcat(tokenString,intStr);
    strcat(tokenString,", ");
    strcat(tokenString,newString);
    strcat(tokenString,", ");
    strcat(tokenString,tokenType);
    strcat(tokenString," >\n\0");
    strcpy(newString,"");
    
    return tokenString;

}

//get all .jack files in directory
char** getDir(char* path){
    DIR *d;
    char** files;
    files = malloc(sizeof(char*) * MORE_MEMORY);
    struct dirent *dir;
    d = opendir(path);
    if(d){
        while((dir= readdir(d))!=NULL){
            if(strstr(dir->d_name,".jack_tokens")){
            }else{
                if(strstr(dir->d_name,".jack")){
                    files[totalFiles] = dir->d_name;
                    totalFiles +=1;
                }
                
            }
            
        }
        closedir(d);
    }
    return files;

}


//read from file and return an array of strings, each index in the array is a line of code.
char** getStrings (char* path){
    FILE *file = fopen(path,"r");

    if (file==NULL){
        printf("error opening file/n");
    }

    char** lines;
    lines = malloc(sizeof(char*) * MORE_MEMORY);

    int totalChars = 0;
    int totalLines = 0;
    int totalCharsAll =0;
    char c;

    bool scanning = true;
    while(scanning){
        c = fgetc(file);
        if(ferror(file)){
            printf("error reading file/n");
            scanning = false;
            
        }

        if(feof(file)){
            if (totalChars != 0){
                lines[totalLines] = realloc(lines[totalLines],totalChars+1);
                lines[totalLines][totalChars] = '\0';
                totalLines++;
                counter++;
            }
            scanning = false;
            
        }else{
            if(totalChars ==0){
                lines[totalLines] = malloc(MORE_MEMORY);
                }
            lines[totalLines][totalChars] = c;
            totalChars ++;
            totalCharsAll++;
            charsCounter++;
            if (c=='\n'){
                lines[totalLines] = realloc(lines[totalLines],totalChars+1);
                lines[totalLines][totalChars] = '\0';
                totalLines++;
                counter++;
                totalChars=0;

                if((totalLines % MORE_MEMORY)==0){
                    int size = totalLines + MORE_MEMORY;
                    lines = realloc(lines, sizeof(char*) * size);
                }
            }else if((totalChars % MORE_MEMORY)==0){
                int size = totalChars + MORE_MEMORY;
                lines[totalLines] = realloc(lines[totalLines], size);
            }
            
        }
    }

    lines = realloc(lines, sizeof(char*)*totalLines);
    fclose(file);

    return lines;

}