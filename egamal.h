#include "bigInt.h"

void GeneratedKey (char *p_fname, char *x_fname, char *name);
int FoundElementaryNum(int p);
char* getSimpleNum(std::ifstream &ifst);
void Encoding(char *M_fname, char *p_fname, char *g_fname, char *y_fname);
void Decoding(char *M_fname, char *p_fname, char *a_fname, char *b_fname, char *x_fname);
void CheckFiles(char *hash_fname, char *dec_fname);
void EDS(char *M_fname, char *p_fname, char *g_fname, char *x_fname);
void CheckEDS(char *M_fname, char *p_fname, char *g_fname, char *y_fname, char *r_fname, char *s_fname);
