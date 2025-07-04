#include <windows.h>
#include <iostream>
#include<string.h>
using namespace std;
void colorP(int n_color,string texto)
{
	
	for (int colour = 0x00; colour <= 0xff; colour ++)
	{
		if(colour == n_color){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour),	std::cout << texto;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7),	std::cout << " ";
		}	
	}
}

void color_siguiente(int n_color,string texto)
{
	
	for (int colour = 0x00; colour <= 0xff; colour ++)
	{
		if(colour == n_color){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colour),	std::cout << texto<< std::endl;
		}	
	}
}
//creador GHOTER_PROM 