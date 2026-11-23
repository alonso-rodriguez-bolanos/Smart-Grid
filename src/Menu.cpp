#include "Menu.h"
#include <iostream>
using namespace std;

Menu::Menu(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios, ArchivoJSON& archivo): red(red), clientes(clientes), beneficios(beneficios), archivo(archivo) {
}

// ======================================================================
// Lectura desde el teclado
// ======================================================================

int Menu::leerEntero(int minimo, int maximo) {
    int valor;
    while (true) {
        cout << "Ingrese un valor entre " << minimo << " y " << maximo << ": ";
        if (!(cin >> valor)) { // no escribio un numero
            cout << "Valor invalido: debe ser un numero." << endl;
            cin.clear();            // quita el estado de error
            cin.ignore(1000, '\n'); // descarta lo que escribio
        } else if (valor < minimo || valor > maximo) { // numero fuera del rango
            cout << "Valor fuera de rango. Intente de nuevo." << endl;
            cin.ignore(1000, '\n');
        } else {
            cin.ignore(1000, '\n'); // limpia el Enter antes de salir
            return valor;
        }
    }
}

double Menu::leerDecimal() {
    double valor;
    while (true) {
        if (!(cin >> valor) || cin.peek() != '\n') {
            cout << "Valor invalido: escriba un numero y use punto para decimales. Intente de nuevo: ";
            cin.clear();
            cin.ignore(1000, '\n');
        } else {
            cin.ignore(1000, '\n'); // limpia el Enter antes de salir
            return valor; // los rangos los valida Grafo
        }
    }
}

string Menu::leerTexto(string mensaje) {
    cout << mensaje;
    string texto;
    getline(cin, texto);
    return texto;
}

string Menu::elegirTipo() {
    cout << "Tipo del vertice" << endl;
    cout << "1. Generador" << endl;
    cout << "2. Subestacion" << endl;
    cout << "3. Subestacion local" << endl;
    int opcion = leerEntero(1, 3);
    if (opcion == 1) {
        return "generador";
    }
    if (opcion == 2) {
        return "subestacion";
    }
    return "subestacion_local";
}

string Menu::elegirMedio() {
    cout << "Medio de la linea" << endl;
    cout << "1. Alta tension" << endl;
    cout << "2. Subterraneo" << endl;
    cout << "3. Red local" << endl;
    int opcion = leerEntero(1, 3);
    if (opcion == 1) {
        return "alta_tension";
    }
    if (opcion == 2) {
        return "subterraneo";
    }
    return "red_local";
}

string Menu::elegirVertice(string mensaje) {
    cout << mensaje << endl;

    int n = 0;
    Vertice* v = red.getPrimerVertice();
    while (v != nullptr) {
        n++;
        cout << n << ". " << v->getNombre() << " (" << v->getTipo() << ", " << v->getRegion() << ")" << endl;
        v = v->getSig();
    }
    if (n == 0) {
        cout << "No hay vertices en la red." << endl;
        return "";
    }
    cout << "0. Cancelar" << endl;
    int opcion = leerEntero(0, n);
    if (opcion == 0) {
        return "";
    }

    v = red.getPrimerVertice();
    for (int k = 1; k < opcion; k++) {
        v = v->getSig();
    }
    return v->getNombre();
}

string Menu::elegirCliente(string mensaje) {
    cout << mensaje << endl;

    int n = 0;
    Cliente* c = clientes.getPrimero();
    while (c != nullptr) {
        n++;
        cout << n << ". " << c->getNombre() << " (" << c->getTokens() << " tokens)" << endl;
        c = c->getSig();
    }
    if (n == 0) {
        cout << "No hay clientes registrados." << endl;
        return "";
    }
    cout << "0. Cancelar" << endl;
    int opcion = leerEntero(0, n);
    if (opcion == 0) {
        return "";
    }

    c = clientes.getPrimero();
    for (int k = 1; k < opcion; k++) {
        c = c->getSig();
    }
    return c->getNombre();
}

string Menu::elegirBeneficio(string mensaje) {
    cout << mensaje << endl;

    int n = 0;
    Beneficio* b = beneficios.getPrimero();
    while (b != nullptr) {
        n++;
        cout << n << ". " << b->getNombre() << " | costo: " << b->getCosto() << " tokens" << endl;
        b = b->getSig();
    }
    if (n == 0) {
        cout << "No hay beneficios en el catalogo." << endl;
        return "";
    }
    cout << "0. Cancelar" << endl;
    int opcion = leerEntero(0, n);
    if (opcion == 0) {
        return "";
    }

    b = beneficios.getPrimero();
    for (int k = 1; k < opcion; k++) {
        b = b->getSig();
    }
    return b->getNombre();
}

bool Menu::elegirArco(string& origen, string& destino, string& medio) {
    origen = elegirVertice("Vertice de origen de la linea:");
    if (origen == "") {
        return false;
    }
    Vertice* v = red.buscarVertice(origen);


    cout << "Lineas que salen de " << origen << ":" << endl;
    int n = 0;
    Arco* a = v->getPrimerArco();
    while (a != nullptr) {
        n++;
        cout << n << ". " << origen << " -> " << a->getDestino()->getNombre()
             << " [" << a->getMedio() << ", " << a->getDistancia() << " km]" << endl;
        a = a->getSig();
    }
    if (n == 0) {
        cout << origen << " no tiene lineas de salida." << endl;
        return false;
    }
    cout << "0. Cancelar" << endl;
    int opcion = leerEntero(0, n);
    if (opcion == 0) {
        return false;
    }

    a = v->getPrimerArco();
    for (int k = 1; k < opcion; k++) {
        a = a->getSig();
    }
    destino = a->getDestino()->getNombre();
    medio = a->getMedio();
    return true;
}

// ======================================================================
// Submenus
// ======================================================================

void Menu::menuCRUD() {
    int opcion = -1;
    while (opcion != 0) {
        cout << endl << "========== Gestion de datos ==========" << endl;
        cout << "--- Vertices ---" << endl;
        cout << "1. Agregar vertice" << endl;
        cout << "2. Modificar vertice" << endl;
        cout << "3. Eliminar vertice" << endl;
        cout << "--- Lineas ---" << endl;
        cout << "4. Agregar linea" << endl;
        cout << "5. Modificar linea" << endl;
        cout << "6. Eliminar linea" << endl;
        cout << "--- Clientes ---" << endl;
        cout << "7. Agregar cliente" << endl;
        cout << "8. Buscar cliente" << endl;
        cout << "9. Eliminar cliente" << endl;
        cout << "10. Asignar tokens" << endl;
        cout << "--- Beneficios ---" << endl;
        cout << "11. Agregar beneficio" << endl;
        cout << "12. Modificar beneficio" << endl;
        cout << "13. Eliminar beneficio" << endl;
        cout << "0. Regresar" << endl;
        opcion = leerEntero(0, 13);

        switch (opcion) {
            case 1: {
                string nombre = leerTexto("Nombre del vertice: ");
                string tipo = elegirTipo();
                string region = leerTexto("Region del vertice: ");
                if (red.insertarVertice(nombre, tipo, region)) {
                    cout << "Vertice agregado." << endl;
                }
                break;
            }
            case 2: {
                string actual = elegirVertice("Vertice a modificar:");
                if (actual == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string nuevo = leerTexto("Nombre nuevo: ");
                string tipo = elegirTipo();
                string region = leerTexto("Region nueva: ");
                if (red.modificarVertice(actual, nuevo, tipo, region)) {
                    if (nuevo != actual) {
                        clientes.renombrarSubestacionEnTodos(actual, nuevo); // cascada a los clientes
                    }
                    cout << "Vertice modificado." << endl;
                }
                break;
            }
            case 3: {
                string nombre = elegirVertice("Vertice a eliminar:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                if (red.eliminarVertice(nombre)) {
                    clientes.quitarSubestacionDeTodos(nombre); // cascada a los clientes
                    cout << "Vertice eliminado." << endl;
                }
                break;
            }
            case 4: {
                string origen = elegirVertice("Vertice de origen:");
                if (origen == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string destino = elegirVertice("Vertice de destino:");
                if (destino == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string medio = elegirMedio();
                cout << "Distancia en km: ";
                double distancia = leerDecimal();
                cout << "Capacidad maxima: ";
                double capacidad = leerDecimal();
                cout << "Perdida en porcentaje: ";
                double perdida = leerDecimal();
                if (red.insertarArco(origen, destino, medio, distancia, capacidad, perdida, 0)) { // una linea nueva empieza con 0 usos
                    cout << "Linea agregada." << endl;
                }
                break;
            }
            case 5: {
                string origen, destino, medio;
                if (!elegirArco(origen, destino, medio)) { // la linea se elige de la lista
                    cout << "Cancelado." << endl;
                    break;
                }
                cout << "Distancia nueva en km: ";
                double distancia = leerDecimal();
                cout << "Capacidad maxima nueva: ";
                double capacidad = leerDecimal();
                cout << "Perdida nueva en porcentaje: ";
                double perdida = leerDecimal();
                if (red.modificarArco(origen, destino, medio, distancia, capacidad, perdida)) {
                    cout << "Linea modificada." << endl;
                }
                break;
            }
            case 6: {
                string origen, destino, medio;
                if (!elegirArco(origen, destino, medio)) {
                    cout << "Cancelado." << endl;
                    break;
                }
                if (red.eliminarArco(origen, destino, medio)) {
                    cout << "Linea eliminada." << endl;
                }
                break;
            }
            case 7: {
                string nombre = leerTexto("Nombre del cliente: ");
                cout << "Tokens iniciales" << endl;
                int tokens = leerEntero(0, 1000000);
                if (clientes.insertarCliente(nombre, tokens)) {
                    cout << "Cliente agregado." << endl;
                }
                break;
            }
            case 8: {
                string nombre = elegirCliente("Cliente a buscar:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                clientes.imprimirCliente(clientes.buscarCliente(nombre));
                break;
            }
            case 9: {
                string nombre = elegirCliente("Cliente a eliminar:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                if (clientes.eliminarCliente(nombre)) {
                    cout << "Cliente eliminado." << endl;
                } else {
                    cout << "No existe el cliente " << nombre << endl; // eliminarCliente no imprime nada
                }
                break;
            }
            case 10: {
                string nombre = elegirCliente("Cliente al que se le asignan tokens:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                cout << "Cantidad de tokens a sumar o restar con signo menos" << endl;
                int cantidad = leerEntero(-1000000, 1000000);
                if (clientes.asignarTokens(nombre, cantidad)) {
                    cout << "Tokens asignados. Saldo nuevo: " << clientes.buscarCliente(nombre)->getTokens() << endl;
                }
                break;
            }
            case 11: {
                string nombre = leerTexto("Nombre del beneficio: ");
                cout << "Costo en tokens" << endl;
                int costo = leerEntero(1, 1000000);
                if (beneficios.insertarBeneficio(nombre, costo)) {
                    cout << "Beneficio agregado." << endl;
                }
                break;
            }
            case 12: {
                string actual = elegirBeneficio("Beneficio a modificar:");
                if (actual == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string nuevo = leerTexto("Nombre nuevo: ");
                cout << "Costo nuevo en tokens" << endl;
                int costo = leerEntero(1, 1000000);
                if (beneficios.modificarBeneficio(actual, nuevo, costo)) {
                    cout << "Beneficio modificado." << endl;
                }
                break;
            }
            case 13: {
                string nombre = elegirBeneficio("Beneficio a eliminar:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                if (beneficios.eliminarBeneficio(nombre)) {
                    cout << "Beneficio eliminado." << endl;
                }
                break;
            }
            case 0:
                cout << "Regresando al menu principal..." << endl;
                break;
        }
    }
}

void Menu::menuReportes() {
    int opcion = -1;
    while (opcion != 0) {
        cout << endl << "========== Reportes ==========" << endl;
        cout << "1. Recorrido en amplitud" << endl;
        cout << "2. Recorrido en profundidad" << endl;
        cout << "3. Catalogo de beneficios" << endl;
        cout << "4. Reporte de clientes" << endl;
        cout << "5. Nodos inactivos" << endl;
        cout << "0. Regresar" << endl;
        opcion = leerEntero(0, 5);

        switch (opcion) {
            case 1: {
                string inicio = elegirVertice("Vertice de inicio:");
                if (inicio == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                red.imprimirBFS(inicio);
                break;
            }
            case 2: {
                string inicio = elegirVertice("Vertice de inicio:");
                if (inicio == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                red.imprimirDFS(inicio);
                break;
            }
            case 3:
                beneficios.imprimirRecursivo();
                break;
            case 4:
                clientes.imprimirReporte();
                break;
            case 5:
                red.imprimirNodosInactivos(clientes);
                break;
            case 0:
                cout << "Regresando al menu principal..." << endl;
                break;
        }
    }
}

void Menu::menuConsultas() {
    int opcion = -1;
    while (opcion != 0) {
        cout << endl << "========== Consultas ==========" << endl;
        cout << "1. Todas las rutas" << endl;
        cout << "2. Ruta de menor perdida" << endl;
        cout << "3. Ruta de mayor Eco-Tokens" << endl;
        cout << "4. Canjear tokens" << endl;
        cout << "5. Ruta frecuente y unica" << endl;
        cout << "0. Regresar" << endl;
        opcion = leerEntero(0, 5);

        switch (opcion) {
            case 1: {
                string origen = elegirVertice("Vertice de origen:");
                if (origen == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string destino = elegirVertice("Vertice de destino:");
                if (destino == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                cout << "Usar alta tension? 1 = si y 0 = no" << endl;
                int alta = leerEntero(0, 1);
                cout << "Usar subterraneo? 1 = si y 0 = no" << endl;
                int sub = leerEntero(0, 1);
                cout << "Usar red local? 1 = si y 0 = no" << endl;
                int local = leerEntero(0, 1); // no se llama red porque choca con el grafo
                int elegidos = alta + sub + local;
                if (elegidos == 0 || elegidos == 3) { // acuerdo de uno o dos medios
                    cout << "Elija uno o dos medios." << endl;
                } else {
                    red.imprimirTodasLasRutas(origen, destino, alta == 1, sub == 1, local == 1);
                }
                break;
            }
            case 2: {
                string origen = elegirVertice("Vertice de origen:");
                if (origen == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string destino = elegirVertice("Vertice de destino:");
                if (destino == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                red.imprimirRutaMenorPerdida(origen, destino);
                break;
            }
            case 3: {
                string nombre = elegirCliente("Cliente:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string origen = elegirVertice("Vertice de origen:");
                if (origen == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                string destino = elegirVertice("Vertice de destino:");
                if (destino == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                cout << "Es hora no pico? 1 = si y 0 = no" << endl;
                int horaNoPico = leerEntero(0, 1);
                red.consultarRutaMayorTokens(origen, destino, clientes.buscarCliente(nombre), horaNoPico == 1);
                break;
            }
            case 4: {
                string nombre = elegirCliente("Cliente que canjea:");
                if (nombre == "") {
                    cout << "Cancelado." << endl;
                    break;
                }
                clientes.canjearBeneficio(nombre, beneficios);
                break;
            }
            case 5:
                red.imprimirRutaFrecuenteYUnica();
                break;
            case 0:
                cout << "Regresando al menu principal..." << endl;
                break;
        }
    }
}

// ======================================================================
// Menu principal
// ======================================================================

void Menu::ejecutar() {
    int opcion = -1;
    while (opcion != 0) {
        cout << endl << "========== Menu Principal ==========" << endl;
        cout << "1. Gestion de datos" << endl;
        cout << "2. Reportes" << endl;
        cout << "3. Consultas" << endl;
        cout << "4. Guardar cambios" << endl;
        cout << "0. Salir" << endl;
        opcion = leerEntero(0, 4);

        switch (opcion) {
            case 1:
                menuCRUD();
                break;
            case 2:
                menuReportes();
                break;
            case 3:
                menuConsultas();
                break;
            case 4:
                if (!archivo.guardarTodo(red, clientes, beneficios)) { // si sale bien ArchivoJSON ya avisa
                    cout << "Error al guardar los cambios." << endl;
                }
                break;
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
        }
    }
}