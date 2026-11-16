#pragma once
#include <string>
using namespace std;

class Beneficio {
private:
    string nombre;
    int costo;
    Beneficio* sig;

public:
    Beneficio(string nombre, int costo);

    string getNombre();
    int getCosto();
    Beneficio* getSig();

    void setNombre(string nombre);
    void setCosto(int costo);
    void setSig(Beneficio* sig);
};
