#ifndef PLANILHA_H
#define PLANILHA_H

#include <fstream>

using namespace std;

class Planilha
{
	public:
		Planilha(char nome[]);
		~Planilha();
		
		void escrever(float numero);
		void escrever(char palavra[]);
		void escreverCaracter(char palavra);
		void novaColuna();
		void novaLinha();
	protected:
		ofstream arquivo;
};

#endif
