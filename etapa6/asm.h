 #include <stdio.h>
 #include "tac.h"
 #include <stdbool.h>

void generateASM(TAC *tac, FILE* fout);
void printASMInitStatement(FILE* fout);
void printASMElement(FILE* fout, char* element);
void printASMBeginOfFunction(FILE* fout, char* identifier);
void printASMEndOfFunction(FILE* fout, bool isMain);
bool isMainFunction(char *identifier);
void printASMEndStatement(FILE *fout);