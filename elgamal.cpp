#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "elgamal.h"
#include "generationSimpleFile.h"
#include "hash.h"

bool Found(int *Table, int A)
{
	bool good;
	for(int i = 1; i < A; i++)
	{
		good = false;
		for(int j = 0; j < A; j++)
		{
			if(Table[j] == i)
			{
				good = true;
				break;
			}
		}
		if(!good)
			return false;
	}
	return true;
}

int FoundElementaryNum(int p)
{
	int g;
	int *Table = new int[p];
	int r;
	for(g = 2; g < p; g++)
	{
		r = 1;
		for(int j = 0; j < p; j++)
		{
			r = r * g % p;
			Table[j] = r;
		}
		if(Found(Table, p))
			return g;
	}
}

char* getSimpleNum(std::ifstream &ifst)
{
	char *simple_num = new char[10];
	ifst>>simple_num;
	return simple_num;
}

int Space(std::ifstream &ifst)
{
	char c;
	int  amount = 0;
	while(!ifst.eof())
	{
		ifst.get(c);
		if(c == ' ')
			amount++;
	}
	return amount;
}

char* intToChar(int a)
{
	char *num = new char[10];
	if(!a) return "0";
	int max_d = 0, k = a, j = 0, i = 0, p = 1;
	while(k)
	{
		max_d++;
		k /= 10;
	}
	do
	{
		for(j = 1; j < max_d; j++)
			p *= 10;
		k = a / p;
		a %= p;
		p = 1;
		num[i++] = itoa(k);
		max_d--;
	}while(max_d);
	//num = itoa(a, num, 10);
	num[i] = '\0';
	return num;
}

int charToInt(char *a)
{
	int num = 0;
	for(int i = 0; i < strlen(a); i++)
    {
        num *= 10;
        num += atoi(a[i]);
    }
	//num = atoi(a);
	return num;
}

void GeneratedKey (char *p_fname, char *x_fname, char *name)
{
    BigInt G, Y("1");
	BigInt P(p_fname, false), X(x_fname, false);// X < P-1!!! �� ��������
    int p;
	std::ifstream ifst(p_fname);
	ifst>>p;
    ifst.close();
	G = BigInt(intToChar(FoundElementaryNum(p)));
	BigInt DegreeNet[30];
	DegreeNet[0] = G;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
        DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
        DegreeNet[i] %= P;
	}
	BigInt degreeNum[30];
	degreeNum[0] = BigInt("1");
	for(int i = 1; i < 30; i++)
		degreeNum[i] = degreeNum[i-1] * BigInt("2");
    BigInt I("0");
    for(int j = 29; j >= 0;)
    {
        if(X >= I + degreeNum[j])
		{
            Y *= DegreeNet[j];
			Y %= P;
			I += degreeNum[j];
		}
		else
			j--;
    }
	char fname[32];
	strcpy(fname, name);
	strcat(fname, "g.txt");
	G.TextWrite(fname);
	strcpy(fname, name);
	strcat(fname, "y.txt");
	Y.TextWrite(fname);
}

void Encoding(char *M_fname, char *p_fname, char *g_fname, char *y_fname)
{
	std::ifstream in(M_fname);
	int *M_hash = new int[4], *buf , i, p, amount = 5761456;
	buf = (int*)md5(&in);
	for(i = 0; i < 4; i++) M_hash[i] =  buf[i];
	BigInt M(intToChar(M_hash[3])), P(p_fname, false), G(g_fname, false), Y(y_fname, false);
	M *= BigInt("10000000000"); M += BigInt(intToChar(M_hash[2]));
	M *= BigInt("10000000000"); M += BigInt(intToChar(M_hash[1]));
	M *= BigInt("10000000000"); M += BigInt(intToChar(M_hash[0]));
	BigInt A("1"), B("1"), Fi;
	std::ifstream ifst(p_fname);
	ifst>>p;
	ifst.close();
	Fi = P - BigInt("1");
	// random of BigInt member so pechalka
	srand(time(0));
	ifst.open("simpleNum.txt");
	int **degree;
	char *buf_ch = new char[10];
	BigInt C = BigInt(&Fi);
	degree = new int *[2];
	for(i = 0; i < 2; i ++)
		degree[i] = new int [amount];
	memset(degree[0], 0, 4 * amount);
	memset(degree[1], 0, 4 * amount);
	i = 0;
	bool end = true;
	while(end)
	{
		buf_ch = getSimpleNum(ifst);
		if((C % BigInt(buf_ch)) == BigInt("0"))
		{
			C /= BigInt(buf_ch);
			degree[0][i] = charToInt(buf_ch);
			degree[1][i]++;
			i = 0;
			ifst.seekg (0, ifst.beg);
		}
		else
			i++;
		if(C == BigInt("1"))
			end = false;
	}
	ifst.close();
	int key =  0;
	bool good_key;
	while(true)
	{
		good_key = true;
		key = rand()%p-1;
		if(key < 2)
			continue;
		for(int i = 0; i < amount && degree[0][i] != 0 && key >= degree[0][i]; i++)
		{
			if(degree[1][i] != 0)
				if(key % degree[0][i] == 0)
				{
					good_key = false;
					break;
				}
		}
		if(good_key)
			break;
	}
	BigInt KEY(intToChar(key));
	BigInt DegreeNet[30];
	DegreeNet[0] = G;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
	BigInt degreeNum[30];
	degreeNum[0] = BigInt("1");
	for(int i = 1; i < 30; i++)
		degreeNum[i] = degreeNum[i-1] * BigInt("2");
	BigInt I("0");
	for(int j = 29; j >= 0;)
	{
		if(KEY >= I + degreeNum[j])
		{
			A *= DegreeNet[j];
			A %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	DegreeNet[0] = Y;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
	I = BigInt("0");
	for(int j = 29; j >= 0;)
	{
		if(KEY >= I + degreeNum[j])
		{
			B *= DegreeNet[j];
			B %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	M %= P;
	B *= M;
	B %= P;
	A.TextWrite("a.txt");
	B.TextWrite("b.txt");
	M.TextWrite("hash.txt");
}

void Decoding(char *M_fname, char *p_fname, char *a_fname, char *b_fname, char *x_fname)
{
	BigInt A(a_fname, false), B(b_fname, false), X(x_fname, false), P(p_fname, false);
	BigInt DEC;
	BigInt one = "1", degree = P - one - one, buf1("1"), buf2("1");
	//find (A^X)*DEC = B mod P
	// DEC = (A^X)^(FI(P)-1)*B mod P;
	DEC = B % P;
	BigInt DegreeNet[30];
	DegreeNet[0] = A;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
	BigInt degreeNum[30];
	degreeNum[0] = BigInt("1");
	for(int i = 1; i < 30; i++)
		degreeNum[i] = degreeNum[i-1] * BigInt("2");
	BigInt I("0");
	for(int j = 29; j >= 0;)
	{
		if(X >= I + degreeNum[j])
		{
			buf1 *= DegreeNet[j];
			buf1 %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	DegreeNet[0] = buf1;
	DegreeNet[0] %= P;
	for(int i = 1; i < 30; i++)
	{
		DegreeNet[i] = DegreeNet[i-1] * DegreeNet[i-1];
		DegreeNet[i] %= P;
	}
	degreeNum[0] = BigInt("1");
	for(int i = 1; i < 30; i++)
		degreeNum[i] = degreeNum[i-1] * BigInt("2");
	I = BigInt("0");
	for(int j = 29; j >= 0;)
	{
		if(degree >= I + degreeNum[j])
		{
			buf2 *= DegreeNet[j];
			buf2 %= P;
			I += degreeNum[j];
		}
		else
			j--;
	}
	DEC *= buf2;
	DEC %= P;
	DEC.TextWrite("dec.txt");
}

void CheckFiles(char *hash_fname, char *dec_fname)
{
	BigInt HASH(hash_fname, false), DEC(dec_fname, false);
	if( HASH == DEC)
		std::cout<<"Hash and dec are coincide\n";
	else
		std::cout<<"Hash and dec are NOT coincide\n";
}

