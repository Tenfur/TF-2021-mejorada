#pragma once
template <class T>
class Nodo {
	public:
		T *valor;
		Nodo<T> *izquierda;
		Nodo<T> *derecha;
		Nodo(T *valor, Nodo<T> *izquierda = nullptr, Nodo<T> *derecha = nullptr) {
			this->valor = valor;

		}
};



