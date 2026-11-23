#pragma once
#include <string>
#include "Beneficio.h"
using namespace std;

// Catalogo de beneficios
class ListaBeneficios {
private:
    Beneficio* primero;
    void imprimirDesde(Beneficio* actual); // recursiva 

public:
    ListaBeneficios();
    ~ListaBeneficios(); // libera todos los beneficios

    ListaBeneficios(const ListaBeneficios&) = delete;
    ListaBeneficios& operator=(const ListaBeneficios&) = delete;

    // Si rechaza un dato imprime el motivo y devuelve false
    bool insertarBeneficio(string nombre, int costo);

    Beneficio* getPrimero();

    Beneficio* buscarBeneficio(string nombre);
    bool modificarBeneficio(string nombreActual, string nuevoNombre, int nuevoCosto);
    bool eliminarBeneficio(string nombre);
    void imprimirRecursivo(); // llama a imprimirDesde(primero)
};
