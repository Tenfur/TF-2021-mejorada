#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <math.h>
#include <iterator>
#include <fstream>
#include <filesystem>
#include <vector>
#include "Componente.h"
#include "Arbol.h"
using namespace std;

namespace fs = std::experimental::filesystem;

		class Entidad {
			private:
				string rutaRepositorio;
				string rutaOriginal;
				string rutaArchivoCommits;
				list <string>listcambios;
				list<string>::iterator it;
			public:
				Entidad() {

				}
				~Entidad() {

				}
				// Me crea archivos de manera automática
				void crearArchivos(int numArchivos) {
					int  indice, cont = 0;
					char nombreArchivo[9];
					char extensionTxt[4] = { '.', 't', 'x', 't' };
					string nombreCrear = "";
					fstream fileGuardar("nombresArchivos.txt", ios::out | ios::app);
					for (int i = 0; i < numArchivos; i++) {
						for (int j = 0; j < 5; j++) { // Generamos un nombre de cinco letras para cada archivo
							indice = rand() % (122 - 97 + 1) + 97; //  a-z
							nombreArchivo[j] = indice;
						}
						for (int z = 5; z < 9; z++) { // Le agregamos la extension .txt al arreglo del nombre
							nombreArchivo[z] = extensionTxt[cont];
							cont++;
						}
						for (int i = 0; i < 9; i++) { // Pasamos todo el arreglo a una cadena string
							nombreCrear += nombreArchivo[i];
						}
						fstream archivoCrear;
						archivoCrear.open(nombreCrear, ios::out | ios::app);
						archivoCrear.close();
						fileGuardar << nombreCrear << endl;
						cont = 0;
						nombreCrear = "";
					}
					fileGuardar.close();
					cout << "Archivos creados correctamente!" << endl;
				}

				// Agregar archivos al area de preparacion
				vector<Componente*> agregarUnSoloArchivo() {
					vector<Componente*> vectorComponentes;
					string auxRutaRepo = rutaRepositorio, nombreArchivo;
					cout << "Nombre del archivo: ";
					cin >> nombreArchivo;
					nombreArchivo += ".txt";
					auxRutaRepo += nombreArchivo;
					fstream *file = new fstream(nombreArchivo);
					Componente *comp = new Componente(nombreArchivo, file);
					vectorComponentes.push_back(comp);
					return vectorComponentes;
				}
				vector<Componente*> agregarTodosLosArchivos() {
					string nombreArchivo;
					vector<Componente*>vectorComponentes;
					fstream archivoAgregar;
					archivoAgregar.open("nombresArchivos.txt", ios::in);
					while (getline(archivoAgregar, nombreArchivo)) {
						// Creamos los objetos fstream
						fstream *file = new fstream(nombreArchivo);
						Componente *comp = new Componente(nombreArchivo, file);
						vectorComponentes.push_back(comp); // Agregamos todos componentes al vector
						nombreArchivo = " "; // Eliminamos el contenido de la variable que guarda el nombre del archivo
					}
					archivoAgregar.close();
					return vectorComponentes;
				}

				// Commit
				void realizarCommit(vector<Componente*> vectorComponentes) {
					string auxRepo, auxArchivo, auxOriginal, nombreCommit; 

					auxOriginal = rutaOriginal; 
					auxRepo = rutaRepositorio;
					auxArchivo = rutaRepositorio;

					cout << "Nombre del commit: ";
					cin >> nombreCommit;

					//Agregamos el nombre del commit a una archivo por separado, de esa forma tendremos el nombre de cada carpeta
					fstream archivo2;
					auxArchivo += "nombresCommit.txt"; 
					archivo2.open(auxArchivo, ios::out | ios::app);
					archivo2 << nombreCommit << endl;
					archivo2.close();

					auxRepo += nombreCommit;
					fs::create_directory(auxRepo); // Creamos la carpeta donde se guardarán los archivos
					for (int i = 0; i < vectorComponentes.size(); i++) {
							auxOriginal += vectorComponentes.at(i)->getNombre();
							fs::copy(auxOriginal, auxRepo, fs::copy_options::overwrite_existing);
							auxOriginal = rutaOriginal;
					}
				}

				// Indexar todos los archivos de cada carpeta en un árbol
				vector<Arbol<Componente>*> indexarTodosLosArchivosDeCadaVersion() {
					system("cls");
					cout << "Indexando..." << endl;
					vector<Arbol<Componente>*> vectorArboles;
					fstream archivoI;
					string commit, auxRepositorio = rutaRepositorio, info;
					archivoI.open(rutaArchivoCommits, ios::in);

					// Lambdas
					auto criterioString = [](Componente *a, Componente *b) {
						return a->getNombre().compare(b->getNombre()) < 0;
					};

					while (getline(archivoI, commit)) { 
						Arbol<Componente> *arbol = new Arbol<Componente>();
						auxRepositorio += commit;
						for (const auto & entry : fs::directory_iterator(auxRepositorio)) {
							fs::path p = entry; // ruta completa
							string nombre = p.stem().string(); // conseguimos el nombre del archivo
							fstream *dato = new fstream(entry);
							Componente *componente = new Componente(nombre, dato);
							arbol->agregar(componente, criterioString); 
						}
						vectorArboles.push_back(arbol); // se agrega el arbol al vector de arboles
						auxRepositorio = rutaRepositorio; // reiniciamos la ruta
					}
					archivoI.close();
					return vectorArboles;
				}

				//Configuración - Nombres, correo, pais, etc
				void agregarInformacion(int tipo, string dato) {
					fstream config;
					if (tipo == 1) {
						config.open("cnombre.txt", ios::out);
						config << dato << endl;
					}
					else if (tipo == 2) {
						config.open("ccorreo.txt", ios::out);
						config << dato << endl;
					}
					else if (tipo == 3) {
						config.open("cpais.txt", ios::out);
						config << dato << endl;
					}
					config.close();
				}
				vector<string> conseguirInfoConfiguracion() {
					fstream config;
					string linea;
					vector<string> vectorInfo;

					config.open("cnombre.txt", ios::in);
					while (getline(config, linea)) {
						vectorInfo.push_back(linea);
					}
					config.close();

					config.open("ccorreo.txt", ios::in);
					while (getline(config, linea)) {
						vectorInfo.push_back(linea);
					}
					config.close();

					config.open("cpais.txt", ios::in);
					while (getline(config, linea)) {
						vectorInfo.push_back(linea);
					}
					config.close();

					return vectorInfo;
				}

				// Conseguir nombre commits
				vector<string>conseguirNombresCommits() {
					vector<string> vectorCommits;
					fstream archivoCommit;
					string linea;
					archivoCommit.open(rutaArchivoCommits, ios::in);
					while (getline(archivoCommit, linea)) {
						vectorCommits.push_back(linea);
					}
					archivoCommit.close();
					return vectorCommits;
				}

				// Clonar
				vector<Componente*> clonarDirectorio(string version) {
					vector<Componente*>vectorC;
					fstream archivoI;
					string campo, auxRepositorio = rutaRepositorio, aux;
					auxRepositorio += version;
					archivoI.open(rutaArchivoCommits, ios::in);
					for (const auto & entry : fs::directory_iterator(auxRepositorio)) {
						fs::path p = entry;
						string nombre = p.stem().string();
						nombre += ".txt";
						fstream *dato = new fstream(nombre);
						Componente *componente = new Componente(nombre, dato);
						vectorC.push_back(componente);
					}
					cout << "Carpeta clonada correctamente!" << endl;
					return vectorC;
				}

				//Modificar archivos que pueden estar en el area de preparacion o no
				void modificarArchivosEnAreaDePreparacion(vector<Componente*>vectorComponentes) {
					cout << "\t Modificar archivos" << endl;
					int opcionArchivo;
					string contenido;
					for (int i = 0; i < vectorComponentes.size(); i++) {
						cout << i + 1 << ") "<<vectorComponentes.at(i)->getNombre() << endl;
					}
					cout << "Numero del archivo que desea modificar: ";
					cin >> opcionArchivo;
					cout << "Contenido que desea agregar: ";
					cin >> contenido;
					fstream archivoModificar;
					archivoModificar.open(vectorComponentes.at(opcionArchivo - 1)->getNombre(), ios::out | ios::app);
					archivoModificar << contenido << endl;
					archivoModificar.close();

					vectorComponentes.at(opcionArchivo - 1)->setModificacion(true);
					cout << "Archivo modificado correctamente!" << endl;
				}
				void modificarArchivoSinAreaDePreparacion() {
					cout << "\t Modificar archivos" << endl;
					int contador = 1;
					string campo, contenido, archivoModificar;
					fstream modificar;
					modificar.open("nombresArchivos.txt", ios::in);
					while (getline(modificar, campo)) {
						cout << contador << ") " << campo << endl;
						contador++;
					}
					modificar.close();
					cout << "Nombre del archivo que deseas modificar: ";
					cin >> archivoModificar;
					cout << "Contenido que le deseas agregar: ";
					cin >> contenido;

					modificar.open(archivoModificar, ios::out | ios::app);
					modificar << contenido << endl;
					modificar.close();

					cout << "Archivo modificado correctamente!" << endl;
				}

				// Eliminar un archivo del area de preparacion
				vector<Componente*> eliminarArchivoAreaPreparacion(vector<Componente*> vectorComponentes) {
					cout << "\t Eliminar un archivo del area de preparacion" << endl;
					vector<Componente*> vectorC = vectorComponentes;
					int numArchivo;
					if (vectorC.size() == 0) {
						cout << "No hay archivos en el area de preparacion" << endl;
					}
					else {
						for (int i = 0; i < vectorC.size(); i++) {
							cout << i + 1 << ") " << vectorC.at(i)->getNombre() << endl;
						}
						cout << "Numero del archivo que desea eliminar: ";
						cin >> numArchivo;
						vectorC.erase(vectorC.begin() + numArchivo - 1);
						cout << "Archivo eliminado correctamente del area de preparacion!" << endl;
					}
					return vectorC;
				}

				// Comparar archivos de diferentes versiones
				Componente *conseguirComponente(string version, string nombreArchivo) {
					string aux = rutaRepositorio;
					aux += version;
					cout << aux << endl;
					for (const auto & entry : fs::directory_iterator(aux)) {
						fs::path p = entry;
						string nombre = p.stem().string();
						if (nombre == nombreArchivo) {
							fstream *dato = new fstream(entry);
							Componente *componente = new Componente(nombre, dato);
							return componente;
						}
					}
				}
				void compararMismosArchivosDiferenesVersiones(vector<Arbol<Componente>*> vectorArboles) {
					// Lambdas
					auto criterioBusqueda = [](Componente *a, string b) {
						cout << "Nombre: " <<a->getNombre() << endl;
						cout << "b: " << b << endl;
						return a->getNombre() == b ? true : false;
					};
					string archivo;
					Componente *primerComponente;
					Componente *segundoComponente;
					int contador = 1;
					string versionUno, versionDos, linea;
					cout << "\t Comparar archivos de diferentes versiones" << endl;
					cout << "Archivos disponibles: " << endl;
					vectorArboles.at(vectorArboles.size() - 1)->imprimir(); // Imprimimos el nombre de todos los archivos del último commit
					cout << "Nombre del archivo: ";
					cin >> archivo;
					cout << "Versiones disponibles: " << endl;
					fstream archivoC;
					archivoC.open(rutaArchivoCommits, ios::in);
					while (getline(archivoC, linea)) {
						cout << contador << ") " << linea << endl;
						contador++;
					}
					archivoC.close();
					cout << "Version uno: ";
					cin >> versionUno;
					cout << "Version dos: ";
					cin >> versionDos;

					// Conseguimos los dos archivos
					primerComponente = conseguirComponente(versionUno, archivo);
					segundoComponente = conseguirComponente(versionDos, archivo);

					// Realizamos la comparación
					string lineaUno, lineaDos;
					fstream *archivoP = primerComponente->getArchivo();
					fstream *archivoP2 = segundoComponente->getArchivo();
					while (getline(*archivoP, lineaUno)) {
						while (getline(*archivoP2, lineaDos)) {
							if (lineaUno.compare(lineaDos) == 0) {
								cout << "Iguales - " << lineaUno << " " << lineaDos << endl;
								break;
							}
							else if(lineaUno.empty() || lineaDos.empty()) {
								cout << "Diferentes - " << lineaUno << " " << lineaDos << endl;
								break;
							}
							else {
								cout << "Diferentes - " << lineaUno << " " << lineaDos << endl;
								break;
							}
						}
					}
			
				}

				// Genera hash
				void generahash(Componente* archivo) {
					int hashcreado;
					string s = archivo->getNombre();
					long sum = 0, mul = 1;
					for (int i = 0; i < s.length(); i++) {
						mul = (i % 4 == 0) ? 1 : mul * 256;
						sum += s.at(i) * mul;
					}
					hashcreado = (int)(abs(sum));
					archivo->setHash(hashcreado);


				}
				// Stash
				void listarcambios() {
					string texto;
					for (it = listcambios.begin(); it != listcambios.end(); ++it) {
						cout << *it << endl;
					}
				}
				void ultimocambio() {
					cout << listcambios.back();
				}
				void eliminarcambios() {
					listcambios.clear();
				}

				// Show
				void show(vector<Componente*>vectorComponentes) {
					cout << "\t Modificar archivos" << endl;
					int opcionArchivo;
					for (int i = 0; i < vectorComponentes.size(); i++) {
						cout << i + 1 << ") " << vectorComponentes.at(i)->getNombre() << endl;
					}
					cout << "Numero del archivo que desea visualizar: ";
					cin >> opcionArchivo;
					fstream archivo;
					string contenido;
					archivo.open(vectorComponentes.at(opcionArchivo - 1)->getNombre(), ios::in);
					while (!archivo.eof()) {
						getline(archivo, contenido);
						cout << contenido << endl;
					}
				}

				// Setters rutas
				void setRutaRepositorio(string rutaRepositorio) {
					this->rutaRepositorio = rutaRepositorio;
				}
				void setRutaOriginal(string rutaOriginal) {
					this->rutaOriginal = rutaOriginal;
				}
				void setRutaArchivosCommits(string rutaArchivosCommit) {
					this->rutaArchivoCommits = rutaArchivosCommit;
				}

		};