#include "pch.h"
#include "Planilha.h"
#include <string.h>
#include <string>

Planilha::Planilha(char nome[])
{
	arquivo.open(nome);
}

Planilha::~Planilha()
{
	arquivo.close();
}

void Planilha::escrever(float numero)
{
	arquivo << numero << ";";
}

void Planilha::escrever(char palavra[])
{
	arquivo << palavra << ";";
}


void Planilha::escreverCaracter(char palavra)
{
	arquivo << palavra;
}

void Planilha::novaColuna()
{
	arquivo << ";";
}

void Planilha::novaLinha()
{
	arquivo << "\n";
}
