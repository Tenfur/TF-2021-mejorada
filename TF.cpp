#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include "Entidad.h"
#include "Componente.h"
#include "Arbol.h"
#include "Indexacion.h"
using namespace std;

namespace fs = std::experimental::filesystem;

int menu() {
	int opcion;
	cout << "\t Control de Versiones" << endl;
	cout << "1) Ingresar comando" << endl;
	cout << "2) Generar archivos" << endl;
	cout << "3) Modificar archivos" << endl;
	cout << "4) Operaciones en archivos" << endl;
	cout << "5) Salir" << endl;
	cout << "Ingrese opcion: ";
	cin >> opcion;
	return opcion;
}
int menuOperaciones() {
	int opcionFiltrado;
	cout << "\t Filtrados" << endl;
	cout << "1) Igual a" << endl;
	cout << "2) Inicia con" << endl;
	cout << "3) Finaliza con" << endl;
	cout << "4) Esta contenido en" << endl;
	cout << "5) No esta contenido en" << endl;
	cout << "6) Salir" << endl;
	cout << "Ingrese opcion: ";
	cin >> opcionFiltrado;
	return opcionFiltrado;
}

int seleccionarVersion(vector<string> nombresCommit) {
	int opcion;
	cout << "Versiones disponibles" << endl;
	for (int i = 0; i < nombresCommit.size(); i++) {
		cout << i + 1 << " )" << nombresCommit.at(i) << endl;
	}
	cout << "Seleccione una version: ";
	cin >> opcion;
	return opcion;
}

int main() {
	// Variables - vectores - vector de arboles
	srand(time(NULL));
	Entidad *control = new Entidad();
	fstream archivo;
	int opcion, opcionFiltrado, numArchivos, opcionPreparacion, opcionCommit;
	string rutaOriginal = fs::current_path().u8string() + "\\";
	string comando, nombreArchivo, rutaRepositorio = rutaOriginal + "gitRepo\\", aux;
	string rutaNombresCommits = rutaRepositorio + "nombresCommit.txt";;


	vector<Componente*> vectorComponentes;

	vector<string> vectorDatosConfig;
	Indexacion *objetoIndexacion = new Indexacion();
	vector<string> vectorNombresCommits;

	// Comprobamos si ya hay un repositorio existente
	if (fs::exists("gitRepo")) {
		control->setRutaOriginal(rutaOriginal);
		control->setRutaRepositorio(rutaRepositorio);
		control->setRutaArchivosCommits(rutaNombresCommits);
	}


	// Indexamos todos los archivos
	objetoIndexacion->setVectorArboles(control->indexarTodosLosArchivosDeCadaVersion());

	do {
		// Cargamos la configuración
		vectorDatosConfig = control->conseguirInfoConfiguracion();
		// Conseguimos los nombres de todos los commits
		vectorNombresCommits = control->conseguirNombresCommits();
		system("cls");
		opcion = menu();
		if (opcion == 1) {
			system("cls");
			cout << "\t Ingrese comando" << endl;
			cout << "Comando: ";
			cin >> comando;
			if (comando == "config") {
				system("cls");
				string info;
				int opcionConfiguracion;
				cout << "\t Configuracion" << endl;
				cout << "Que desea agregar o modificar:  " << endl;
				cout << "1) Nombre" << endl;
				cout << "2) Correo" << endl;
				cout << "3) Pais" << endl;
				cout << "Ingrese opcion: ";
				cin >> opcionConfiguracion;
				if (opcionConfiguracion == 1) {
					cout << "Nombre: ";
					cin >> info;
				}
				else if (opcionConfiguracion == 2) {
					cout << "Correo: ";
					cin >> info;
				}
				else if (opcionConfiguracion == 3) {
					cout << "Pais: ";
					cin >> info;
				}
				control->agregarInformacion(opcionConfiguracion, info);
				cout << "Configuracion hecha correctamente!" << endl;
				_getch();
			}
			else if (comando == "init") {
				if (fs::exists("gitRepo")) {
					cout << "Error, el repositorio ya ha sido inicializado!" << endl;
					control->setRutaOriginal(rutaOriginal);
					control->setRutaRepositorio(rutaRepositorio);
					control->setRutaArchivosCommits(rutaNombresCommits);
				}
				else {
					system("mkdir gitRepo");
					control->setRutaOriginal(rutaOriginal);
					control->setRutaRepositorio(rutaRepositorio);
					control->setRutaArchivosCommits(rutaNombresCommits);

					fstream archivo2;
					archivo2.open(rutaNombresCommits, ios::out | ios::app);
					archivo2.close();

					cout << "Repositorio creado!" << endl;
					_getch();
				}
			}
			else if (comando == "clone") {
				system("cls");
				string opcion;
				cout << "\t Versiones creadas" << endl;
				for (int i = 0; i < vectorNombresCommits.size(); i++) {
					cout << i + 1 << ") " << vectorNombresCommits.at(i) << endl;
				}
				cout << "Clonar version: ";
				cin >> opcion;
				vectorComponentes = control->clonarDirectorio(opcion);
				_getch();
			}
			else if (comando == "status") {
				system("cls");
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion" << endl;
				}
				else {
					cout << "Archivos agregados al area de preparacion: " << endl << endl;
					for (int i = 0; i < vectorComponentes.size(); i++) {
						cout << vectorComponentes.at(i)->getNombre() << endl;
					}
					cout << endl;
					cout << "Archivos modificados: " << endl << endl;
					for (int i = 0; i < vectorComponentes.size(); i++) {
						if (vectorComponentes.at(i)->getModificado() == true) {
							cout << vectorComponentes.at(i)->getNombre() << endl;
						}
					}
				}
				_getch();
			}
			else if (comando == "add") {
				do {
					cout << "\t Que deseas agregar al area de preparacion: " << endl;
					cout << "1) Un archivo en especifico" << endl;
					cout << "2) Todos los archivos" << endl;
					cout << "Opcion: ";
					cin >> opcionPreparacion;
				} while (opcionPreparacion < 0);
				if (opcionPreparacion == 1) {
					vectorComponentes = control->agregarUnSoloArchivo();
					cout << "Archivo agregado correctamente a la zona de preparacion!" << endl;
					_getch();
				}
				else if (opcionPreparacion == 2) {
					// Me agrega todos los archivos a un vector(area de preparacion)
					vectorComponentes = control->agregarTodosLosArchivos();
					cout << "Archivo agregado correctamente a la zona de preparacion!" << endl;
					_getch();
				}
			}
			else if (comando == "reset") {
				system("cls");
				vectorComponentes = control->eliminarArchivoAreaPreparacion(vectorComponentes);
				_getch();
			}
			else if (comando == "diff") {
				system("cls");
				control->compararMismosArchivosDiferenesVersiones(objetoIndexacion->getVectorArboles());
				_getch();
			}
			else if (comando == "commit") {
				control->realizarCommit(vectorComponentes);
				cout << "Commit realizado correctamente!" << endl;
				vectorComponentes.clear(); // Eliminamos todos los objetos que había en el vector
				_getch();
			}
			else if (comando == "hash") {
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion" << endl;
					_getch();
				}
				else {
					for (int i = 0; i < vectorComponentes.size(); i++) {
						control->generahash(vectorComponentes[i]);
						cout << vectorComponentes[i]->getNombre() << "->" << vectorComponentes[i]->getHash() << endl;
					}
					_getch();

				}
			}
			else if (comando == "show") {
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion";
				}
				else {
					control->show(vectorComponentes);
				}
				_getch();
			}
			else if (comando == "stash") {
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion" << endl;
					_getch();
				}
				else {
					cout << "Cambios almacenados correctamente" << endl;
					_getch();

				}

			}
			else if (comando == "stash-list") {
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion" << endl;
					_getch();
				}
				else {
					control->listarcambios();
					_getch();

				}

			}
			else if (comando == "stash-pop") {
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion" << endl;
					_getch();
				}
				else {
					control->ultimocambio();
					_getch();

				}
			}
			else if (comando == "stash-drop") {
				if (vectorComponentes.size() == 0) {
					cout << "No hay archivos en el area de preparacion" << endl;
					_getch();
				}
				else {
					control->eliminarcambios();
					cout << "Archivos eliminados correctamente";
					_getch();

				}
			}
			else if (comando == "log") {
				system("cls");
				cout << "\t Commits realizados por el usuario" << endl;
				fstream commits;
				commits.open(rutaNombresCommits, ios::in);
				string campoLeer;
				int contador = 1;
				while (getline(commits, campoLeer)) {
					cout << "Nombre del commit numero " << contador << ": " << campoLeer << endl;
					cout << "Nombre del usuario que hizo el commit: " << vectorDatosConfig.at(0) << endl << endl;
					contador++;
				}
				commits.close();
				_getch();
			}
			else {
				cout << "Comando incorrecto" << endl;
				_getch();
			}
		}
		else if (opcion == 2) {
			system("cls");
			cout << "\t Creacion de archivos" << endl;
			cout << "Numero de archivos que desea: ";
			cin >> numArchivos;
			control->crearArchivos(numArchivos);
			_getch();
		}
		else if (opcion == 3) {
			system("cls");
			if (vectorComponentes.size() == 0) {
				control->modificarArchivoSinAreaDePreparacion();
			}
			else {
				control->modificarArchivosEnAreaDePreparacion(vectorComponentes);
			}
			_getch();

		}
		else if (opcion == 4) {
			do {

				system("cls");
				opcionFiltrado = menuOperaciones();
				if (opcionFiltrado == 1) {
					system("cls");
					string dato;
					cout << "\t Igual a" << endl;
					opcionCommit = seleccionarVersion(vectorNombresCommits);
					cout << "Archivo que desea buscar: ";
					cin >> dato;
					cout << "Archivos iguales al dato que se ingreso: " << endl;
					auto criterioIgualA = [=](Componente *a) {
						if (a->getNombre() == dato) {
							cout << a->getNombre() << endl;
						}
					};
					objetoIndexacion->mostrarFiltro(dato, opcionCommit, criterioIgualA);
					_getch();
				}
				else if (opcionFiltrado == 2) {
					system("cls");
					cout << "\t Inicia con" << endl;
					char dato;
					opcionCommit = seleccionarVersion(vectorNombresCommits);
					cout << "Valor: ";
					cin >> dato;
					cout << "Archivos que inican iguales al dato que se ingreso: " << endl;
					auto criterioIniciaCon = [=](Componente *a) {
						string nombre = a->getNombre();
						if (nombre[0] == dato) {
							cout << a->getNombre() << endl;
						}
					};
					objetoIndexacion->mostrarFiltroPorChar(dato, opcionCommit, criterioIniciaCon);
					_getch();
				}
				else if (opcionFiltrado == 3) {
					system("cls");
					cout << "\t Finaliza con" << endl;
					char dato;
					opcionCommit = seleccionarVersion(vectorNombresCommits);
					cout << "Valor: ";
					cin >> dato;
					cout << "Archivos que finalizan con el dato que se ingreso: " << endl;
					auto criterioFinalizaCon = [=](Componente *a) {
						string nombre = a->getNombre();
						int size = nombre.length();
						if (nombre[size - 1] == dato) {
							cout << a->getNombre() << endl;
						}
						//objetoIndexacion->mostrarFiltroPorChar(dato, opcionCommit);
					_getch();
				}
				else if (opcionFiltrado == 4) {
					system("cls");
					cout << "\t Esta contenido en" << endl;
					string dato;
					opcionCommit = seleccionarVersion(vectorNombresCommits);
					cout << "Valor: ";
					cin >> dato;
					cout << "Archivos que finalizan con el dato que se ingreso: " << endl;
					auto criterioContenidoEn = [=](Componente *a) {
						string nombre = a->getNombre();
						if (nombre.find(dato) != std::string::npos) {
							cout << a->getNombre() << endl;
						}
					};
					//objetoIndexacion.at(opcionCommit - 1)->mostrar(criterioContenidoEn);
					_getch();
				}
				else if (opcionFiltrado == 5) {
					system("cls");
					cout << "\t No esta contenido en" << endl;
					string dato;
					opcionCommit = seleccionarVersion(vectorNombresCommits);
					cout << "Valor: ";
					cin >> dato;
					cout << "Archivos que finalizan con el dato que se ingreso: " << endl;
					auto criterioNoEstaContenidoEn = [=](Componente *a) {
						string nombre = a->getNombre();
						if (!(nombre.find(dato) != std::string::npos)) {
							cout << a->getNombre() << endl;
						}
					};
					//objetoIndexacion.at(opcionCommit - 1)->mostrar(criterioNoEstaContenidoEn);
					_getch();
				}
			} while (opcionFiltrado != 6);
		}
	} while (opcion != 5);


	return 0;
}
