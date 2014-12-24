#include <fstream>
#include <iostream>
#include "elgamal.h"

int main(int argc, char *argv[])
{
    int menu = 0;
    //char *a_P = new char[24], *a_X = new char[24];
    //if (argc == 3)
    //{
        //strcpy(a_P, argv[1]);
        //strcpy(a_X, argv[2]);
        //GeneratedKey(a_P, a_X, "a");
        //Encoding("in.txt", a_P, "ag.txt", "ay.txt");
        //Decoding("hash.txt",a_P, "a.txt", "b.txt", a_X);
        GeneratedKey("ap.txt", "ax.txt", "a");
        Encoding("in.txt", "ap.txt", "ag.txt", "ay.txt");
        Decoding("hash.txt", "ap.txt", "a.txt", "b.txt", "ax.txt");
		CheckFiles("hash.txt", "dec.txt");
    /*}
    else if (argc == 2)
	{
		if(!strcmp("-h", argv[1]) || !strcmp("-H", argv[1]) || !strcmp("-help", argv[1]))
		{
			std::cout<<"This program implements El Gamal scheme: encoding mode\nExample of using:\n elgam.exe [file_P_for_A] [file_X_for_A] [file_P_for_B] [file_X_for_B]";
			return 1;
		}
	}
	else
	{
		std::cout<<"Not enough input data\n";
    }*/
	std::cin>>menu;
	return 0;
}
