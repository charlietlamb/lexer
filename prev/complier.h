#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
char** getStrings(char* path);
char** getDir(char* path);
char* getTokens(char**file,int limit, char* path);
bool isFinished(char thisChar, char nextChar, bool firstDigit, int stringCount, char nextNextChar);
char* getType(char* current, char nextChar, bool stringStart, bool longComment, bool EOFile);
char* createToken(char* current,char* file, int lineNumber, char* tokenType, char *tokenString);
bool create(char* current);
bool isSymbolRet(char thisChar);

const char *keywords[] = {"class","method","function","constructor","int","boolean","char","void","var","static","field",
"let","do","if","else","while","return","true","false","null","this"};
int keywordSize = 21;

const char *symbols[] = {"(",")","[","]","{","}",",",";","=",".","+","-","*","/","&","|","~","<",">"};

const char symbolsChar[19] = {"()[]{};,=.+-*/&|~<>"};

int symbolSize = 19;