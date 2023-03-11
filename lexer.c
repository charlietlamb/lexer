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

// define memory for malloc/realloc
#define MORE_MEMORY 1024

// define total lines as global variable
// reset totalLines each time


int totalFiles = 0;
int counter = 0;
int charsCounter;
bool setString = false;
bool setDigit = false;

// header file for the lexer module
// #define TEST		// uncomment this line to run the self-grader

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
char** getStrings(char* path);
char** getDir(char* path);
char* getTokens(char**file,int limit, char* path);
bool isFinished(char thisChar, char nextChar, bool firstDigit, int stringCount, char nextNextChar);
char *getType(char *current, char nextChar, int stringCount, bool longComment, bool EOFile);
char* createToken(char* current,char* file, int lineNumber, char* tokenType, char *tokenString);
bool create(char* current);
bool isSymbolRet(char thisChar);
Token *make(char* tokens);


const char *keywords[] = {"class","method","function","constructor","int","boolean","char","void","var","static","field",
"let","do","if","else","while","return","true","false","null","this"};
int keywordSize = 21;

const char *symbols[] = {"(",")","[","]","{","}",",",";","=",".","+","-","*","/","&","|","~","<",">"};

const char symbolsChar[19] = {"()[]{};,=.+-*/&|~<>"};

int symbolSize = 19;

int InitLexer (char* file);
Token GetNextToken ();
Token PeekNextToken ();
int StopLexer ();
Token *make(char* tokens);
int fileCount = 0;
int tokenCount = 0;
int fileNum =0;
Token *myTokenArray;
int lineCount;
char**files;
bool gotFiles = false;

//for testing
void myPrintToken (Token t);
char* myTokenTypeString (TokenType t);

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer(char *file_name)
{
    //check if file in dir
    if(!gotFiles){
        files = getDir("./");
        gotFiles = true;
    }
    bool found = false;
    
    for(int i=0;i<totalFiles;i++){
        char* temp = malloc(sizeof(char)*1024);
        strcpy(temp,files[i]);
        if(!strcmp(file_name,temp)){
            found = true;
            break;
        }
        free(temp);
    }
    if(!found){
        return false;
    }
    char path[1024];
    strcpy(path,"./");
    strcat(path, file_name);
    char **tokens;
    tokens = malloc(sizeof(char *) * MORE_MEMORY);
    char **file;
    file = getStrings(path);
    char *tokTemp = malloc(sizeof(getTokens(file, counter, path)) + 1);
    tokTemp = getTokens(file, counter, path);
    lineCount = 0;
    int stringLength = strlen(tokTemp);
    for (int i = 0; i < stringLength; i++)
    {
        if (tokTemp[i] == '\n')
        {
            lineCount++;
        }
    }
    myTokenArray = malloc(sizeof(Token) * lineCount);
    myTokenArray = make(tokTemp);
    for (int j=0;j<lineCount;j++){
        
        counter = 0;
        charsCounter = 0;
        
        //printf("%d\n",myTokenArray->ln);
        
    }
    fileNum++;
    
    tokenCount=0;
    return 1;
}

// Get the next token from the source file
Token GetNextToken()
{
    if(lineCount >= tokenCount){
        Token t = myTokenArray[tokenCount];
        tokenCount++;
        return t;
    }else{
        Token t;
        t.tp = ERR;
        return t;
    }
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken()
{
    if(lineCount >= tokenCount){
        Token t = myTokenArray[tokenCount];
        return t;
    }else{
        Token t;
        t.tp = ERR;
        return t;
    }
    
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer()
{
    return 0;
}

// do not remove the next line
#ifndef TEST

void myPrintToken (Token t)
{
	printf ("<%s, %i, %s, %s>\n", t.fl, t.ln , t.lx, myTokenTypeString (t.tp));
}

char* myTokenTypeString (TokenType t)
{
	switch (t)
	{
		case RESWORD: return "RESWORD";
		case ID: return "ID";
		case INT: return "INT";
		case SYMBOL: return "SYMBOL";
		case STRING: return "STRING";
		case EOFile: return "EOFile";
		case ERR: return "ERR";
		default: return "Not a recognised token type";
	}

}

int main(int argc, char *argv[])
{
	InitLexer ("IntegersOnly.jack");
	Token t = GetNextToken ();
	while (t.tp != EOFile)
	{
        myPrintToken (t);
		t = GetNextToken();
	}
	
    
}
// do not remove the next line
#endif

Token *make(char *tokens)
{
    int lineCount = 0;
    int stringLength = strlen(tokens);
    for (int i = 0; i < stringLength; i++)
    {
        if (tokens[i] == '\n')
        {
            lineCount++;
        }
    }
    myTokenArray = malloc(sizeof(Token) * lineCount);
    char *saveString, *saveAtt;
    int tokenCount = 0;
    char delim[2] = "\n";
    char *string;
    bool comma = false;
    string = strtok_r(tokens, delim, &saveString);
    
    while (string != NULL)
    {
        if (string != NULL)
        {
            
            Token t;
            int len = strlen(string);
            string[len-2] = '\0';
            string += 2;
            
            char *att; // current attribute
            char delim2[3] = ",";
            att = strtok_r(string, delim2, &saveAtt);
            
            
            if (att != NULL)
            {
                if(att[0] == '.'&&att[1]=='/'){
                    att +=2;
                }
                
                strcpy(t.fl, att);
                att = strtok_r(NULL, delim2, &saveAtt);
                att++;
                
            }
            if (att != NULL)
            {
                t.ln = atoi(att);
                att = strtok_r(NULL, delim2, &saveAtt);
                att++;
                
            }
            if (att != NULL)
            {
                
                if(strlen(att)==0){
                    strcpy(t.lx,",");
                    comma = true;
                    t.ln++;
                }else{
                    strcpy(t.lx, att);
                    att = strtok_r(NULL, delim2, &saveAtt);
                    att++;
                }
                
            }
            
            if (att != NULL)
            {
                if (!strcmp(att, "RESWORD"))
                {
                    t.tp = RESWORD;
                    t.ln++;
                }
                if (!strcmp(att, "ID"))
                {
                    t.tp = ID;
                    t.ln++;
                }
                if (!strcmp(att, "INT"))
                {
                    t.tp = INT;
                    t.ln++;
                }
                if (!strcmp(att, "SYMBOL"))
                {
                    t.tp = SYMBOL;
                    t.ln++;
                }
                if (!strcmp(att, "STRING"))
                {
                    t.tp = STRING;
                    t.ln++;
                }
                if (!strcmp(att, "EOFile"))
                {
                    t.tp = EOFile;
                }
                if (!strcmp(att, "ERR"))
                {
                    t.tp = ERR;
                    if (!strcmp(t.lx, "Error: unexpected eof in comment"))
                    {
                        t.ec = 1;
                        t.ln++;
                    }
                    if (!strcmp(t.lx, "Error: unexpected eof in string constant"))
                    {
                        t.ec = 2;
                    }
                    if (!strcmp(t.lx, "Error: unexpected eof in string constant"))
                    {
                        t.ec = 3;
                    }
                    if (!strcmp(t.lx, "Error: illegal symbol in source file"))
                    {
                        t.ec = 4;
                    }
                }
                else
                {
                    t.ec = 0;
                }
                if(comma){
                    t.tp = SYMBOL;
                    comma = false;
                }
            }
            myTokenArray[tokenCount] = t;
            //printf("%s\n",tokenArray[tokenCount].lx);
        }
        
        tokenCount++;
        string = strtok_r(NULL, delim, &saveString);
    }
    return myTokenArray;
}

char *getTokens(char **file, int limit, char *path)
{

    int lineNum = 0;
    int charNum = 0;
    char *line;                // current line
    char current[1024] = "\0"; // current string
    bool start = true;         // start of line?
    char type[1024] = "init";  // type of token?
    bool typeSame = true;      // are the characters the same type?
    char thisChar = ' ';
    char nextChar = ' ';
    bool firstDigit = false;                           // true if first char is a digit
    char endChar[1024] = "init";                       // if string or comment that need to find end char
    bool stringStart = false;                          // checks if token is a string
    bool tokenDone = false;                            // if end of token
    char temp[1024] = "";                              // used for adding \n to last line of file
    char *tokens;                                      // stores tokens in file
    tokens = malloc(sizeof(char *) * MORE_MEMORY * 8); // allocates memory to token array
    strcpy(tokens, "");
    int tokenIndex = 0; // used for indexing tokens
    bool commentMode = false;
    bool finished;           // checks if current token has finished creating
    bool skip = false;       // skip as line has ended.
    char nextNextChar = ' '; // used to check for //
    char *tokenString = malloc(sizeof(char) * 1024);
    bool longComment = false;
    bool EOFile = false;
    bool error = false;
    bool empty;
    bool illegalSymbol = false;
    int stringCount = 0; // used to see when "" start and close

    for (int i = 0; i < limit; i++)
    {        
        
        // prevents eof error
        
        // checks that line exists
        line = (file[i]);
        if (line != NULL)
        {
            //current[0]=thisChar;
            //current[1]='\0';
            empty = true;
            int len = strlen(line);
            for(int k=0;k<len;k++){
                if(line[k]!=' '){
                    empty = false;
                }
            }
            if (!strchr(line, '\n'))
            {
                strcat(line, "\n");
            }
            if (!strchr(current, '\0'))
            {
                strcat(current, "\0");
            }

            int j = 0;
            thisChar = 'a';
            nextChar = 'a';
            // iterates through characters in line
            skip = false;
            if (isSymbolRet(line[0]) && (line[0] != '/' && line[0] != '*') && (!commentMode))
            {
                char *add;
                char *tokenString = malloc(sizeof(char) * 1024);
                char tempStr[2] = {line[0], '\0'};
                add = createToken(tempStr, path, lineNum, "SYMBOL", tokenString);
                strcat(tokens, add);
                free(tokenString);
                strcpy(current, "\0");
                tokenIndex++;
            }
            if (strchr(line, '\n')==NULL && strchr(line, '\0')==NULL)
            {
                line[strlen(line)] = '\0';
            }
            if(empty){
                thisChar='\0';
            }
            while ((thisChar != '\n' && thisChar != '\0'))
            { // checks line hasn't finished
                
                thisChar = line[j];
                if (thisChar != '\n')
                {
                    nextChar = line[j + 1];
                    if (nextChar != '\n')
                    {
                        nextNextChar = line[j + 2];
                    }
                    else
                    {
                        nextNextChar = '\0';
                    }
                }
                else
                {
                    skip = true;
                }
                if (thisChar == '/' && nextChar == '/')
                {
                    skip = true;
                }
                if ((thisChar == '/' && nextChar == '*') )
                {
                    commentMode = true;
                    longComment = true;
                    skip=true;
                }
                if (commentMode)
                {
                    if (thisChar == '*' && nextChar == '/')
                    {
                        commentMode = false;
                        longComment = false;
                        skip = false;
                    }
                    else
                    {
                        skip = true;
                    }
                }

                if(thisChar=='?' && stringCount==0){
                    illegalSymbol = true;
                }
                
                if(isdigit(thisChar)&&nextChar=='\n'){
                    printf("CURRENT:%s\n",current);
                    /*
                    char *add;
                    char *tokenString = malloc(sizeof(char) * 1024);
                    char tempStr[2] = {line[0], '\0'};
                    add = createToken(tempStr, path, lineNum, "SYMBOL", tokenString);
                    strcat(tokens, add);
                    free(tokenString);
                    strcpy(current, "\0");
                    tokenIndex++;
                    */
                }

                // checks if the first character is a digit
                if (!skip && !error)
                {
                    if (isdigit(current[0]))
                    {
                        firstDigit = true;
                    }
                    else
                    {
                        firstDigit = false;
                    }

                    // checks if the token is a string
                    if (thisChar == '"')
                    {
                        stringCount += 1;
                    }
                    finished = isFinished(thisChar, nextChar, firstDigit, stringCount, nextNextChar); // checks if nextChar
                    
                    // is part of the same token as thisChar
                    
                    if (stringStart)
                    {
                        if (nextChar != '"')
                        {
                            if (!strchr(current, '\0'))
                            {
                                strcat(current, "\0");
                            }
                            int len = strlen(current);
                            current[len] = thisChar;
                            current[len + 1] = nextChar;
                            current[len + 2] = '\0';
                        }
                        else
                        {
                            
                            stringStart = false;
                            char *add;
                            char *tokenString = malloc(sizeof(char) * 1024);
                            
                            add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                            char *errorString = "ERR";
                            if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                error = true;
                            }
                            strcat(tokens, add);
                            tokens = realloc(tokens, sizeof(tokens) + sizeof(add));
                            free(tokenString);
                            tokenIndex++;
                            strcpy(current, "\0");
                            
                        }
                    }
                    else
                    {
                        
                        if (!finished)
                        { // if they're the same type then add to current token

                            if ((!strcmp(current, "//")) || ((nextChar == '/') && (nextNextChar == '/')))
                            { // checks if the current token is a line comment
                                thisChar = '\n';
                            }
                            if ((!strcmp(current, "/*")))
                            { // checks if the current token is a line comment
                                commentMode = true;
                                longComment = true;
                            }
                            // add char to end of string

                            bool whiteSpace = false;
                            if (!strchr(current, '\0'))
                            {
                                strcat(current, "\0");
                            }
                            for (int k; k < strlen(current); k++)
                            {
                                if (current[k] == ' ')
                                {
                                    whiteSpace = true;
                                }
                            }
                            if (stringCount == 1)
                            {
                                int len = strlen(current);
                                current[len] = thisChar;
                                current[len + 1] = '\0';
                            }
                            else
                            {

                                if ((nextChar != ' ' || (whiteSpace)))
                                {
                                    if (!strchr(current, '\0'))
                                    {
                                        strcat(current, "\0");
                                    }
                                    if (!strchr(current, '\0'))
                                    {
                                        strcat(current, "\0");
                                    }
                                    int len = strlen(current);
                                    if (j == 0 && nextChar != ' ')
                                    {
                                        current[len] = thisChar;
                                        current[len + 1] = nextChar;
                                        current[len + 2] = '\0';
                                    }
                                    else
                                    {
                                        if (j == 0)
                                        {
                                            if (!strchr(current, '\0'))
                                            {
                                                strcat(current, "\0");
                                            }
                                            len = strlen(current);
                                            current[len] = thisChar;
                                            current[len + 1] = '\0';
                                        }
                                        else
                                        {
                                            if (!strchr(current, '\0'))
                                            {
                                                strcat(current, "\0");
                                            }
                                            len = strlen(current);
                                            current[len] = nextChar;
                                            current[len + 1] = '\0';
                                        }
                                    }
                                    if (isSymbolRet(nextNextChar))
                                    {
                                        if(strcmp(current,"\"")){
                                            char *add;
                                            char *tokenString = malloc(sizeof(char) * 1024);
                                            add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                                            char *errorString = "ERR";
                                            if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                                error = true;
                                                }
                                            strcat(tokens, add);
                                            free(tokenString);
                                            tokenIndex++;
                                            strcpy(current, "\0");
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            
                            if(strlen(current)!=0){
                                
                                // if white space or if the two
                                // characters are of different types e.g. e and +
                                if (stringStart)
                                {
                                    stringStart = false;
                                }
                                if (!strchr(current, '\0'))
                                {
                                    strcat(current, "\0");
                                }
                                char *newString = (char *)malloc(strlen(current));
                                int j = 0;
                                for (int i = 0; i < strlen(current); i++)
                                {
                                    if (current[i] != '\n')
                                    {
                                        newString[j] = current[i];
                                        j++;
                                    }
                                }
                                strcpy(current, newString);
                                
                                if (create(current))
                                {
                                    char *add;
                                    char *tokenString = malloc(sizeof(char) * 1024);
                                    add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                                    char *errorString = "ERR";
                                    if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                        error = true;
                                    }
                                    strcat(tokens, add);
                                    free(tokenString);
                                    tokenIndex++;
                                    strcpy(current, "\0");
                                    if(nextChar==')'){
                                        current[0] = nextChar;
                                        current[1] = '\0';
                                        char *add;
                                        char *tokenString = malloc(sizeof(char) * 1024);
                                        add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                                        char *errorString = "ERR";
                                        if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                            error = true;
                                            }
                                        strcat(tokens, add);
                                        free(tokenString);
                                        tokenIndex++;
                                        strcpy(current, "\0");
                                    }
                                }
                                else
                                { // is symbol
                                
                                    if(current[0]=='"'){
                                        
                                        int len = strlen(current);
                                        current[len] = '"';
                                        current[len + 1] = '\0';
                                        char *add;
                                        char *tokenString = malloc(sizeof(char) * 1024);
                                        add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                                        char *errorString = "ERR";
                                        if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                            error = true;
                                        }
                                        strcat(tokens, add);
                                        free(tokenString);
                                        strcpy(current, "\0");
                                        tokenIndex++;
                                    }
                                    if (nextChar != ' ')
                                    {
                                        if (nextChar == '/' && nextNextChar == '/')
                                        {
                                            thisChar = '\n';
                                        }
                                        else if (nextChar == '/' && thisChar == '*')
                                        {
                                            // if there is an end of multi line comment
                                            strcpy(current, "\0");
                                        }
                                        else
                                        {
                                            strcpy(current, "\0");
                                            if (!strchr(current, '\0'))
                                            {
                                                strcat(current, "\0");
                                            }
                                            int len = strlen(current);
                                            current[len] = nextChar;
                                            current[len + 1] = '\0';
                                            char *add;
                                            char *tokenString = malloc(sizeof(char) * 1024);
                                            add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                                            char *errorString = "ERR";
                                            if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                                error = true;
                                            }
                                            strcat(tokens, add);
                                            free(tokenString);
                                            strcpy(current, "\0");
                                            tokenIndex++;
                                        }
                                    }
                                }
                            }else{
                                
                                bool cont = false;
                                for (int i = 0; i < symbolSize; i++)
                                {
                                    if (symbolsChar[i] == nextChar)
                                    {
                                        
                                        cont = true;
                                    }
                                }
                                if(cont){
                                    if (nextChar == '/' && nextNextChar == '/')
                                    {
                                        thisChar = '\n';
                                    }
                                    else if (nextChar == '/' && thisChar == '*')
                                    {
                                        // if there is an end of multi line comment
                                        strcpy(current, "\0");
                                    }
                                    else
                                    {
                                        bool go = true;
                                        if(nextChar == '/' && nextNextChar == '*'){
                                            go = false;
                                        }
                                        if(go){
                                            strcpy(current, "\0");
                                            if (!strchr(current, '\0'))
                                            {
                                                strcat(current, "\0");
                                            }
                                            int len = strlen(current);
                                            current[len] = nextChar;
                                            current[len + 1] = '\0';
                                            char *add;
                                            char *tokenString = malloc(sizeof(char) * 1024);
                                            add = createToken(current, path, lineNum, getType(current, nextChar, stringCount, longComment, EOFile), tokenString);
                                            char *errorString = "ERR";
                                            if(strstr(getType(current, nextChar, stringCount, longComment, EOFile),errorString)){
                                                error = true;
                                            }
                                            strcat(tokens, add);
                                            free(tokenString);
                                            strcpy(current, "\0");
                                            tokenIndex++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                j++;
                if (stringCount == 2)
                {
                    stringCount = 0;
                }
            }

            charNum++;
        }
        line = (file[i+1]);
        lineNum++;
    }
    char *tokenstring = "";
    if (stringCount == 1)
    {
        char *add;
        char *tokenString = malloc(sizeof(char) * 1024);
        add = createToken("Error: unexpected eof in string constant", path, lineNum, "ERR", tokenString);
        error = true;
        strcat(tokens, add);
        free(tokenString);
        tokenIndex++;
        strcpy(current, "\0");
        return tokens;
    }
    if (longComment)
    {
        char *add;
        char *tokenString = malloc(sizeof(char) * 1024);
        add = createToken("Error: unexpected eof in comment", path, lineNum, "ERR", tokenString);
        error = true;
        strcat(tokens, add);
        free(tokenString);
        tokenIndex++;
        strcpy(current, "\0");
        return tokens;
    }
    if (illegalSymbol)
    {
        char *add;
        char *tokenString = malloc(sizeof(char) * 1024);
        add = createToken("Error: illegal symbol in source file", path, lineNum, "ERR", tokenString);
        error = true;
        strcat(tokens, add);
        free(tokenString);
        tokenIndex++;
        strcpy(current, "\0");
        return tokens;
    }
    if(!error){
        char *add;
        char *tokenString2 = malloc(sizeof(char) * 1024);
        add = createToken("End of File", path, lineNum, "EOFile", tokenString2);
        strcat(tokens, add);
        free(tokenString);
        strcpy(current, "\0");
        tokenIndex++; 
    }
    return tokens;
    
}

bool create(char *current)
{
    if (strchr(current, '\n'))
    {
        return false;
    }
    if (!strchr(current, '\0'))
    {
        strcat(current, "\0");
    }
    if (current == NULL)
    {
        strcpy(current, "\0");
    }
    int length = strlen(current);
    bool empty = true;
    for (int i = 0; i < length; i++)
    {
        if (isdigit(current[i]) || isalpha(current[i]))
        {
            empty = false;
        }
    }
    if (empty)
    {
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

bool isFinished(char thisChar, char nextChar, bool firstDigit, int stringCount, char nextNextChar)
{ // checks if token is finished
    if (stringCount == 2)
    {
        return true;
    }else if(stringCount == 1){
        return false;
    }
    if (stringCount != 1)
    {
        if (nextChar == ' '||nextChar == '\n'||nextChar == '\t')
        { // checks for white space
            return true;
        }
        if (firstDigit)
        {
            if (thisChar == '(' || thisChar == ')')
            {
                return true;
            }
            if (isdigit(thisChar))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        for (int i = 0; i < symbolSize; i++)
        {
            if (symbolsChar[i] == nextChar)
            {
                
                return true;
            }
        }
    }

    return false;
}

bool isSymbolRet(char thisChar)
{
    for (int i = 0; i < symbolSize; i++)
    {
        if (symbolsChar[i] == thisChar)
        {
            return true;
        }
    }
    return false;
}

char *getType(char *current, char nextChar, int stringCount, bool longComment, bool EOFile)
{ // gets type of token str
    if ((stringCount>0) && (nextChar == '\n'))
    {
        return "ERR_STR_LN";
    }
    for (int i = 0; i < symbolSize; i++)
    {
        
        if (!strcmp(current, symbols[i]))
        {
            return "SYMBOL";
        }
    }
    if(stringCount>2){
        return "STRING";
    }

    char newString[1024];
    int counter = 0;
    if (!strchr(current, '\0'))
    {
        strcat(current, "\0");
    }
    if (current == NULL)
    {
        strcpy(current, "\0");
    }
    for (int i = 0; i < strlen(current); i++)
    {
        if (current[i] == '\r' || current[i] == '\t'|| current[i] == '\n')
        {
        }
        else
        {
            newString[counter] = current[i];
            counter++;
        }
    }
    newString[counter] = '\0';
    strcpy(current,newString);

    for (int i = 0; i < keywordSize; i++)
    {
        if (!strcmp(current, keywords[i]))
        {
            return "RESWORD";
        }
    }
    
    bool integer = true;
    if (!strchr(current, '\0'))
    {
        strcat(current, "\0");
    }
    if (current == NULL)
    {
        strcpy(current, "\0");
    }
    for (int i = 0; i < strlen(current); i++)
    {
        if (!isdigit(current[i]))
        {
            integer = false;
        }
    }
    if (integer)
    {
        return "INT";
    }

    

    if ((stringCount>0) && (EOFile))
    {
        return "ERR_STR";
    }


    if ((longComment) && EOF)
    {
        return "ERR_CMT";
    }
    if (strchr(current,'?')&&stringCount==0)
    {
        return "ERR_ILL";
    }
    return "ID";
}

char *createToken(char *current, char *file, int lineNumber, char *tokenType, char *tokenString)
{
    
    if (!strcmp(tokenType, "ERR_STR"))
    {
        current = "Error: unexpected eof in string constant\0";
        tokenType = "ERR";
    }

    if (!strcmp(tokenType, "ERR_STR_LN"))
    {
        current = "Error: new line in string constant\0";
        tokenType = "ERR";
    }

    if (!strcmp(tokenType, "ERR_ILL"))
    {
        current = "Error: illegal symbol in source file\0";
        tokenType = "ERR";
    }
    if (!strcmp(tokenType, "ERR_CMT"))
    {
        current = "Error: unexpected eof in comment\0";
        tokenType = "ERR";
    }
    if (current[0] == '"')
    {
        
        current++;
        if(current[0]=='"'){
            current++;
        }
        int len = strlen(current);
        if(current[len-1]!=' '){
            current[len-1] = '\0';
        }
        tokenType = "STRING";
    }
    char newString[1024];
    int counter = 0;
    if (!strchr(current, '\0'))
    {
        strcat(current, "\0");
    }
    if (current == NULL)
    {
        strcpy(current, "\0");
    }
    for (int i = 0; i < strlen(current); i++)
    {
        if (current[i] == '\r' || current[i] == '\t'|| current[i] == '\n')
        {
        }
        else
        {
            newString[counter] = current[i];
            counter++;
        }
    }
    newString[counter] = '\0';

    strcpy(tokenString, "< ");
    strcat(tokenString, file);
    strcat(tokenString, ", ");
    char intStr[1024];
    sprintf(intStr, "%d", lineNumber);
    strcat(tokenString, intStr);
    strcat(tokenString, ", ");
    strcat(tokenString, newString);
    strcat(tokenString, ", ");
    strcat(tokenString, tokenType);
    strcat(tokenString, " >\n\0");
    strcpy(newString, "");

    return tokenString;
}

// get all .jack files in directory
char **getDir(char *path)
{
    DIR *d;
    char **files;
    files = malloc(sizeof(char *) * MORE_MEMORY);
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".jack_tokens"))
            {
            }
            else
            {
                if (strstr(dir->d_name, ".jack"))
                {
                    files[totalFiles] = dir->d_name;
                    totalFiles += 1;
                }
            }
        }
        closedir(d);
    }
    return files;
}

// read from file and return an array of strings, each index in the array is a line of code.
char **getStrings(char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        printf("error opening file/n");
    }

    char **lines;
    lines = malloc(sizeof(char *) * MORE_MEMORY);

    int totalChars = 0;
    int totalLines = 0;
    int totalCharsAll = 0;
    char c;

    bool scanning = true;
    while (scanning)
    {
        c = fgetc(file);
        if (ferror(file))
        {
            printf("error reading file/n");
            scanning = false;
        }

        if (feof(file))
        {
            if (totalChars != 0)
            {
                lines[totalLines] = realloc(lines[totalLines], totalChars + 1);
                lines[totalLines][totalChars] = '\0';
                totalLines++;
                counter++;
            }
            scanning = false;
        }
        else
        {
            if (totalChars == 0)
            {
                lines[totalLines] = malloc(MORE_MEMORY);
            }
            lines[totalLines][totalChars] = c;
            totalChars++;
            totalCharsAll++;
            charsCounter++;
            if (c == '\n')
            {
                lines[totalLines] = realloc(lines[totalLines], totalChars + 1);
                lines[totalLines][totalChars] = '\0';
                totalLines++;
                counter++;
                totalChars = 0;

                if ((totalLines % MORE_MEMORY) == 0)
                {
                    int size = totalLines + MORE_MEMORY;
                    lines = realloc(lines, sizeof(char *) * size);
                }
            }
            else if ((totalChars % MORE_MEMORY) == 0)
            {
                int size = totalChars + MORE_MEMORY;
                lines[totalLines] = realloc(lines[totalLines], size);
            }
        }
    }

    lines = realloc(lines, sizeof(char *) * totalLines);
    fclose(file);

    return lines;
}