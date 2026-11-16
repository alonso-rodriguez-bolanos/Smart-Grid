#pragma once
#include <string>
#include "Beneficio.h"
using namespace std;

// Catalogo de beneficios: lista simple enlazada
class ListaBeneficios {
private:
    Beneficio* primero;
    void imprimirDesde(Beneficio* actual); // recursiva (B3)

public:
    ListaBeneficios();
    ~ListaBeneficios(); // libera todos los beneficios

    // No se permite copiar la estructura: siempre se pasa con &
    ListaBeneficios(const ListaBeneficios&) = delete;
    ListaBeneficios& operator=(const ListaBeneficios&) = delete;

    // ===== CONTRATO: lo usa Persona 2 para cargar =====
    // Si rechaza un dato imprime el motivo y devuelve false
    bool insertarBeneficio(string nombre, int costo);

    // ===== CONTRATO: lo usa Persona 2 para guardar =====
    Beneficio* getPrimero();

    // ===== Solo Persona 1 =====
    Beneficio* buscarBeneficio(string nombre);
    bool modificarBeneficio(string nombreActual, string nuevoNombre, int nuevoCosto);
    bool eliminarBeneficio(string nombre);
    void imprimirRecursivo(); // llama a imprimirDesde(primero)
};
