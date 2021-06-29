#pragma once
#include "Nodo.h"
#include "Componente.h"
#include <functional>

	template <class T>
	class Arbol {
		Nodo<T> *root;
		private:

			void _agregar(T *valor, Nodo<T> *&n, function<bool(T*,T*)> comp) {
				if (n == nullptr) {
					n = new Nodo<T>(valor);
				}
				else if (comp(valor, n->valor)) {
					_agregar(valor, n->izquierda, comp);
				}
				else {
					_agregar(valor, n->derecha, comp);
				}
			}

			void _mostrar(Nodo<T> *&n, function<void(T*)> criterio) {
				if (n == nullptr) {
					return;
				}
				else {
					_mostrar(n->izquierda, criterio);
					criterio(n->valor);
					_mostrar(n->derecha, criterio);
				}
			}

			void _imprimir(Nodo<T> *n) {
				if (n == nullptr) {
					return;
				}
				_imprimir(n->izquierda);
				cout << *n->valor << endl;
				_imprimir(n->derecha);
			}


		public:
			Arbol() {
				root = nullptr;
			}
			void imprimir() {
				_imprimir(root);
				cout << endl;
			}
			void agregar(T *valor, function<bool(T*, T*)> comp) {
				_agregar(valor, root, comp);
			}
			void mostrar(function<void(T*)> criterio) {
				_mostrar(root, criterio);
			}
	};


