#pragma once
#include "Arbol.h"
#include "Componente.h"
#include <vector>
#include <functional>
using namespace std;

class Indexacion {
	private:
		vector<Arbol<Componente>*> vectorArboles;
	public:
		Indexacion() {

		}
		~Indexacion() {
		}
		void setVectorArboles(vector<Arbol<Componente>*> vectorArboles) {
			this->vectorArboles = vectorArboles;
		}
		void mostrarFiltro(string dato, int opcionCommit, function<void(Componente*)> comp) {
			vectorArboles.at(opcionCommit - 1)->mostrar(comp);
		}
		void mostrarFiltroPorChar(char dato, int opcionCommit, function<void(Componente*)> comp) {
			vectorArboles.at(opcionCommit - 1)->mostrar(comp);
		}
		vector<Arbol<Componente>*> getVectorArboles() {
			return vectorArboles;
		}
};