#pragma once
#include <string>
using namespace std;

// Nodo de las sublistas de un cliente: subestaciones y beneficios canjeados (D11c)
class NodoTexto {
private:
    string nombre;
    NodoTexto* sig;

public:
    NodoTexto(string nombre);

    string getNombre();
    NodoTexto* getSig();

    void setNombre(string nombre);
    void setSig(NodoTexto* sig);
};
