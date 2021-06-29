#pragma once
#include <iostream>
#include <list>
#include <string>
#include <fstream>
using namespace std;

	class Componente {
		private:
			string nombre;
			fstream *archivo;
			bool modificado;
			int hash;
		public:
			Componente() {
			}
			Componente(string nombre, fstream *archivo) {
				this->nombre = nombre;
				this->archivo = archivo;
				modificado = false;
				hash = 0;
			}
			fstream *getArchivo() {
				return archivo;
			}
			string getNombre() {
				return nombre;
			}
			friend ostream& operator<<(ostream& os,  Componente& comp){
				os << comp.getNombre();
				return os;
			}
			bool getModificado() {
				return modificado;
			}
			void setModificacion(bool valor) {
				this->modificado = valor;
			}
			void setHash(int hashcreado) {
				hash = hashcreado;
			}
			int getHash() {
				return hash;
			}

	};