#include <iostream>
#include <Windows.h>

#pragma once
class Mhyprot2SCManager
{

private:
	//nada '-'


public:
	//Método para instalar o driver
	void instalarDriver();
	//Método para desinstalar o driver
	void desinstalarDriver();
	//Método para iniciar o driver
	void iniciarMhyprot2();
	//Método para parar o driver
	void pararMhyprot2();

};

