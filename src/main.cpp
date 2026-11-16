#include <iostream>
#include "Grafo.h"
#include "ListaClientes.h"
#include "ListaBeneficios.h"
#include "ArchivoJSON.h"
#include "Menu.h"
using namespace std;

int main() {
    Grafo red;
    ListaClientes clientes;
    ListaBeneficios beneficios;
    ArchivoJSON archivo("data/red_electrica.json", "data/clientes.json", "data/beneficios.json");

    if (!archivo.cargarTodo(red, clientes, beneficios)) {
        return 1; // ArchivoJSON ya mostro el error
    }

    Menu menu(red, clientes, beneficios, archivo);
    menu.ejecutar();

    if (!archivo.guardarTodo(red, clientes, beneficios)) { // guardado obligatorio al cerrar
        cout << "ERROR: no se pudieron guardar los cambios" << endl;
        return 1;
    }
    cout << "Programa terminado" << endl;
    return 0;
}
