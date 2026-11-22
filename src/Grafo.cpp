#include "Grafo.h"
#include <iostream>
#include <queue>
#include <limits>
#include <cmath>
using namespace std;

Grafo::Grafo() {
    primerVertice = nullptr;
}

Grafo::~Grafo() {
    Vertice* actual = primerVertice;

    while (actual != nullptr) {
        Arco* arcoActual = actual->getPrimerArco();
        while (arcoActual != nullptr) {
            Arco* Aeliminado = arcoActual;
            arcoActual = arcoActual->getSig();
            delete Aeliminado;
        }
        Vertice* Veliminado = actual;
        actual = actual->getSig();
        delete Veliminado;
    }
}

bool Grafo::insertarVertice(string nombre, string tipo, string region) {
    if (nombre.empty() || tipo.empty() || region.empty()) {
        cout << "Error: Nombre, tipo o region vacios." << endl;
        return false; // datos invalidos
    }

    if (existeVertice(nombre)) {
        cout << "Error: El vertice con nombre '" << nombre << "' ya existe." << endl;
        return false; // vertice ya existe
    }

    if (tipo != "generador" && tipo != "subestacion" && tipo != "subestacion_local") {
        cout << "Error: Tipo de vertice invalido. Debe ser 'generador', 'subestacion' o 'subestacion_local'." << endl;
        return false; // tipo invalido
    }


    Vertice* nuevoVertice = new Vertice(nombre, tipo, region);
    if (primerVertice == nullptr) {
        primerVertice = nuevoVertice;
    } 
    else {
        Vertice* actual = primerVertice;
        while (actual->getSig() != nullptr) {
            actual = actual->getSig();
        }
        actual->setSig(nuevoVertice);
    }
    return true;
}

bool Grafo::valoresArcoValidos(double distancia, double capacidad, double perdida) {
    return distancia > 0 && capacidad > 0 && perdida >= 0 && perdida <= 100;
}

bool Grafo::insertarArco(string origen, string destino, string medio, double distancia, double capacidad, double perdida, int usos) {
    Vertice* temp = buscarVertice(origen);
    if (temp == nullptr) {
        cout << "Error: El vertice de origen '" << origen << "' no existe." << endl;
        return false; // origen no existe
    }

    Vertice* destinoV = buscarVertice(destino);
    if (destinoV == nullptr) {
        cout << "Error: El vertice de destino '" << destino << "' no existe." << endl;
        return false; // destino no existe
    }

    if (temp == destinoV) {
        cout << "Error: Un arco no puede ir de '" << origen << "' a si mismo." << endl;
        return false; // lazo
    }

    if (medio != "alta_tension" && medio != "subterraneo" && medio != "red_local") {
        cout << "Error: Medio de transporte invalido. Debe ser 'alta_tension', 'subterraneo' o 'red_local'." << endl;
        return false; // medio invalido
    }

    if (!valoresArcoValidos(distancia, capacidad, perdida) || usos < 0) {
        cout << "Error: Valores de distancia, capacidad, perdida o usos invalidos." << endl;
        return false; // valores invalidos
    }

    // Revisar repetidos: mismo destino y mismo medio
    Arco* a = temp->getPrimerArco();
    while (a != nullptr) {
        if (a->getDestino() == destinoV && a->getMedio() == medio) {
            cout << "Error: Ya existe una linea de '" << origen << "' a '" << destino
                 << "' por " << medio << "." << endl;
            return false; // arco repetido
        }
        a = a->getSig();
    }

    Arco* nuevo = new Arco(destinoV, medio, distancia, capacidad, perdida, usos);

    if (temp->getPrimerArco() == nullptr) {
        temp->setPrimerArco(nuevo);
    }
    else {
        Arco* actual = temp->getPrimerArco();
        while (actual->getSig() != nullptr) {
            actual = actual->getSig();
        }
        actual->setSig(nuevo);
    }
    return true;
}

bool Grafo::existeVertice(string nombre) {
    Vertice *vertice = buscarVertice(nombre);
    if (vertice != nullptr) {
        return true; // vertice existe
    }
    return false; // vertice no existe
}

Vertice* Grafo::getPrimerVertice() {
    return primerVertice;
}

Vertice* Grafo::buscarVertice(string nombre) {
    Vertice* actual = primerVertice;

    while (actual != nullptr) {
        if (actual->getNombre() == nombre) {
            return actual;
        }
        actual = actual->getSig();
    }
    return nullptr;
}

bool Grafo::modificarVertice(string nombreActual, string nuevoNombre, string nuevoTipo, string nuevaRegion) {
    Vertice* temp = buscarVertice(nombreActual);
    if (temp == nullptr) {
        cout << "Error: El vertice '" << nombreActual << "' no existe." << endl;
        return false; // vertice no encontrado
    }

    if (nuevoNombre.empty() || nuevoTipo.empty() || nuevaRegion.empty()) {
        cout << "Error: Nombre, tipo o region vacios." << endl;
        return false; // datos invalidos
    }

    // Solo es repetido si cambia el nombre y el nuevo ya lo usa OTRO vertice
    if (nuevoNombre != nombreActual && existeVertice(nuevoNombre)) {
        cout << "Error: El vertice con nombre '" << nuevoNombre << "' ya existe." << endl;
        return false; // nombre repetido
    }

    if (nuevoTipo != "generador" && nuevoTipo != "subestacion" && nuevoTipo != "subestacion_local") {
        cout << "Error: Tipo de vertice invalido. Debe ser 'generador', 'subestacion' o 'subestacion_local'." << endl;
        return false; // tipo invalido
    }

    temp->setNombre(nuevoNombre);
    temp->setTipo(nuevoTipo);
    temp->setRegion(nuevaRegion);
    return true;
}


bool Grafo::eliminarVertice(string nombre) {
    Vertice* objetivo = buscarVertice(nombre);
    if (objetivo == nullptr) {
        cout << "Error: El vertice '" << nombre << "' no existe." << endl;
        return false; // vertice no encontrado
    }

    // borrar los arcos de otros vertices que llegan al objetivo
    Vertice* actualVertice = primerVertice;
    while (actualVertice != nullptr) {
        Arco* arcoActual = actualVertice->getPrimerArco();
        Arco* arcoAnterior = nullptr;
        while (arcoActual != nullptr) {
            Arco* siguiente = arcoActual->getSig();   // guardar antes de un posible delete
            if (arcoActual->getDestino() == objetivo) {
                if (arcoAnterior == nullptr) {
                    actualVertice->setPrimerArco(siguiente);
                }
                else {
                    arcoAnterior->setSig(siguiente);
                }
                delete arcoActual;   // arcoAnterior no avanza
            }
            else {
                arcoAnterior = arcoActual;   // solo avanza si el arco se quedo
            }
            arcoActual = siguiente;
        }
        actualVertice = actualVertice->getSig();
    }

    // borrar los arcos que salen del objetivo
    while (objetivo->getPrimerArco() != nullptr) {
        Arco* arcoEliminar = objetivo->getPrimerArco();
        objetivo->setPrimerArco(arcoEliminar->getSig());
        delete arcoEliminar;
    }

    // desenlazar y borrar el vertice
    Vertice* actual = primerVertice;
    Vertice* anterior = nullptr;
    while (actual != nullptr) {
        if (actual == objetivo) {
            if (anterior == nullptr) {
                primerVertice = actual->getSig();
            }
            else {
                anterior->setSig(actual->getSig());
            }
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->getSig();
    }
    return false;
}

bool Grafo::modificarArco(string origen, string destino, string medio, double distancia, double capacidad, double perdida) {
    Vertice* verticeOrigen = buscarVertice(origen);
    if (verticeOrigen == nullptr) {
        cout << "Error: El vertice de origen '" << origen << "' no existe." << endl;
        return false; // origen no encontrado
    }

    if (!valoresArcoValidos(distancia, capacidad, perdida)) {
        cout << "Error: Valores de distancia, capacidad o perdida invalidos." << endl;
        return false; // valores invalidos
    }

    Arco* actual = verticeOrigen->getPrimerArco();
    while (actual != nullptr) {
        if (actual->getDestino()->getNombre() == destino && actual->getMedio() == medio) {
            actual->setDistancia(distancia);
            actual->setCapacidad(capacidad);
            actual->setPerdida(perdida);
            return true; // arco modificado
        }
        actual = actual->getSig();
    }

    cout << "Error: No existe una linea de '" << origen << "' a '" << destino
         << "' por " << medio << "." << endl;
    return false; // arco no encontrado
}

bool Grafo::eliminarArco(string origen, string destino, string medio) {
    Vertice* verticeOrigen = buscarVertice(origen);

    if (verticeOrigen == nullptr) {
        cout << "Error: El vertice de origen '" << origen << "' no existe." << endl;
        return false;  // vertice origen no encontrado
    }

    Arco* actual = verticeOrigen->getPrimerArco();
    Arco* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->getDestino()->getNombre() == destino && actual->getMedio() == medio) {
            if (anterior == nullptr) {
                verticeOrigen->setPrimerArco(actual->getSig());
            } else {
                anterior->setSig(actual->getSig());
            }
            delete actual;
            return true;   // arco eliminado
        }
        anterior = actual;
        actual = actual->getSig();
    }

    cout << "Error: No existe una linea de '" << origen << "' a '" << destino
         << "' por " << medio << "." << endl;
    return false;    // arco no encontrado
}

void Grafo::reiniciarVisitados() {
    Vertice* actual = primerVertice;
    while (actual != nullptr) {
        actual->setVisitado(false);
        actual = actual->getSig();
    }
}

void Grafo::imprimirConexiones(Vertice* v) {
    cout << v->getNombre() << " (" << v->getTipo() << ", " << v->getRegion() << "):";

    Arco* arcoActual = v->getPrimerArco();
    bool primero = true;
    while (arcoActual != nullptr) {
        if (primero) {
            cout << " "; // antes del primero solo un espacio
            primero = false;
        } else {
            cout << ", "; // entre conexiones una coma
        }
        cout << arcoActual->getDestino()->getNombre()
             << " [" << arcoActual->getMedio() << ", " << arcoActual->getDistancia() << " km]"; // el medio distingue arcos repetidos
        arcoActual = arcoActual->getSig();
    }
    cout << endl;
}

void Grafo::imprimirBFS(string inicio) {
    reiniciarVisitados(); // todos empiezan sin visitar

    Vertice* verticeInicio = buscarVertice(inicio);
    if (verticeInicio == nullptr) { // el nombre no existe
        cout << "Error: El vertice '" << inicio << "' no existe." << endl;
        return;
    }

    queue<Vertice*> cola;
    verticeInicio->setVisitado(true); // se marca al meterlo
    cola.push(verticeInicio);

    // Recorrido en amplitud desde el inicio
    cout << "Recorrido en amplitud desde " << inicio << ":" << endl;
    while (!cola.empty()) {
        Vertice* actual = cola.front(); // primero se lee
        cola.pop();                     // despues se saca
        imprimirConexiones(actual);

        Arco* arcoActual = actual->getPrimerArco();
        while (arcoActual != nullptr) {
            Vertice* destino = arcoActual->getDestino();
            if (!destino->isVisitado()) {
                destino->setVisitado(true); // se marca al descubrirlo
                cola.push(destino);
            }
            arcoActual = arcoActual->getSig();
        }
    }

    // No alcanzables al final
    imprimirNoAlcanzables();
}

void Grafo::imprimirNoAlcanzables() {
    cout << "No alcanzables:" << endl;
    int noAlcanzables = 0;
    Vertice* actual = primerVertice;
    while (actual != nullptr) {
        if (!actual->isVisitado()) {
            imprimirConexiones(actual);
            noAlcanzables++;
        }
        actual = actual->getSig();
    }
    if (noAlcanzables == 0) {
        cout << "Ninguno" << endl; // todos se alcanzaron
    }
}

void Grafo::dfsRecursivo(Vertice* actual) {
    actual->setVisitado(true); // se marca al entrar

    Arco* arcoActual = actual->getPrimerArco();
    while (arcoActual != nullptr) {
        Vertice* destino = arcoActual->getDestino();
        cout << actual->getNombre() << " -> " << destino->getNombre()
             << " [" << arcoActual->getMedio() << ", " << arcoActual->getDistancia() << " km]" << endl;
        if (!destino->isVisitado()) {
            dfsRecursivo(destino); // baja por esa ruta
        }
        arcoActual = arcoActual->getSig();
    }
    // sin arcos pendientes la funcion termina y regresa sola
}

void Grafo::imprimirDFS(string inicio) {
    reiniciarVisitados(); // todos empiezan sin visitar

    Vertice* verticeInicio = buscarVertice(inicio);
    if (verticeInicio == nullptr) { // el nombre no existe
        cout << "Error: El vertice '" << inicio << "' no existe." << endl;
        return;
    }

    // Recorrido en profundidad desde el inicio
    cout << "Recorrido en profundidad desde " << inicio << ":" << endl;
    if (verticeInicio->getPrimerArco() == nullptr) { // solo se revisa el inicio y no las hojas
        cout << inicio << " no tiene lineas de salida" << endl;
    }
    dfsRecursivo(verticeInicio);

    // No alcanzables al final
    imprimirNoAlcanzables();
}
bool Grafo::medioPermitido(string medio, bool altaTension, bool subterraneo, bool redLocal) {
    return ((medio == "alta_tension" && altaTension) || (medio == "subterraneo" && subterraneo) || (medio == "red_local" && redLocal));
}

int Grafo::contarVertices() {
    int contador = 0;
    Vertice* actual = primerVertice;
    while (actual != nullptr) {
        contador++;
        actual = actual->getSig();
    }
    return contador;
}

void Grafo::buscarRutas(Vertice* inicio, Vertice* actual, Vertice* destino, bool altaTension, bool subterraneo, bool redLocal, Arco** ruta, int largo, int& cantidad, bool puntuar, Arco** mejorRuta, int& mejorLargo, double& mejorPuntaje) {
    //  se llego al destino
    if (actual == destino) {
        cantidad++;
        if (!puntuar) {
            imprimirRuta(inicio, ruta, largo, cantidad); // modo todas las rutas
        } 
        else {
            double puntaje = 0; // modo Eco-Tokens
            for (int k = 0; k < largo; k++) {
                puntaje += puntajeArco(ruta[k]);
            }
            if (mejorLargo == 0 || puntaje > mejorPuntaje) { // es la primera o supera a la mejor
                for (int k = 0; k < largo; k++) {
                    mejorRuta[k] = ruta[k]; // se copia porque ruta se va a sobrescribir
                }
                mejorLargo = largo;
                mejorPuntaje = puntaje;
            }
        }
        return; // el destino no se marca para que otras rutas lleguen
    }

    // Se marca para no pasar dos veces por el mismo vertice
    actual->setVisitado(true);

    // Se prueba cada arco que pase el filtro
    Arco* arcoActual = actual->getPrimerArco();
    while (arcoActual != nullptr) {
        Vertice* siguiente = arcoActual->getDestino();
        if (medioPermitido(arcoActual->getMedio(), altaTension, subterraneo, redLocal) && !siguiente->isVisitado()) {
            ruta[largo] = arcoActual; // se agrega el arco al camino
            buscarRutas(inicio, siguiente, destino, altaTension, subterraneo, redLocal,
                        ruta, largo + 1, cantidad, puntuar, mejorRuta, mejorLargo, mejorPuntaje);
        }
        arcoActual = arcoActual->getSig();
    }

    // Se desmarca para que otra ruta pueda pasar por aqui
    actual->setVisitado(false);
}

void Grafo::imprimirRuta(Vertice* inicio, Arco** ruta, int largo, int numero) {
    cout << "Ruta " << numero << ":" << endl;
    double total = 0;
    for (int i = 0; i < largo; i++) {
        string origenArco;
        if (i == 0) {
            origenArco = inicio->getNombre(); // el primer arco sale del inicio
        } 
        else {
            origenArco = ruta[i - 1]->getDestino()->getNombre(); // sale de donde llego el anterior
        }
        cout << "  " << origenArco << " -> " << ruta[i]->getDestino()->getNombre()
             << " [" << ruta[i]->getMedio() << ", " << ruta[i]->getDistancia() << " km]" << endl;
        total += ruta[i]->getDistancia();
    }
    cout << "  Distancia total: " << total << " km" << endl;
}

void Grafo::imprimirTodasLasRutas(string origen, string destino, bool altaTension, bool subterraneo, bool redLocal) {
    Vertice* verticeOrigen = buscarVertice(origen);
    Vertice* verticeDestino = buscarVertice(destino);
    if (verticeOrigen == nullptr) {
        cout << "Error: El vertice '" << origen << "' no existe." << endl;
        return;
    }
    if (verticeDestino == nullptr) {
        cout << "Error: El vertice '" << destino << "' no existe." << endl;
        return;
    }
    if (verticeOrigen == verticeDestino) {
        cout << "Error: El origen y el destino son el mismo vertice." << endl;
        return;
    }

    reiniciarVisitados();
    Arco** ruta = new Arco*[contarVertices()]; // un camino simple tiene menos arcos que vertices
    int cantidad = 0;

    cout << "Rutas de " << origen << " a " << destino << ":" << endl;
    int mejorLargo = 0;      // no se usan en este modo
    double mejorPuntaje = 0;
    buscarRutas(verticeOrigen, verticeOrigen, verticeDestino, altaTension, subterraneo, redLocal,
                ruta, 0, cantidad, false, nullptr, mejorLargo, mejorPuntaje);
    delete[] ruta;

    if (cantidad == 0) {
        cout << "No hay rutas de " << origen << " a " << destino << " con los medios elegidos" << endl;
    } else {
        cout << "Total: " << cantidad << (cantidad == 1 ? " ruta" : " rutas") << endl;
    }
}

int Grafo::posicionDe(Vertice** vertices, int n, Vertice* buscado) {
    for (int i = 0; i < n; i++) {
        if (vertices[i] == buscado) {
            return i;
        }
    }
    return -1; // no encontrado
}

void Grafo::imprimirRutaMenorPerdida(string origen, string destino) {
    Vertice* verticeOrigen = buscarVertice(origen);
    Vertice* verticeDestino = buscarVertice(destino);
    if (verticeOrigen == nullptr) {
        cout << "Error: El vertice '" << origen << "' no existe." << endl;
        return;
    }
    if (verticeDestino == nullptr) {
        cout << "Error: El vertice '" << destino << "' no existe." << endl;
        return;
    }
    if (verticeOrigen == verticeDestino) {
        cout << "Error: El origen y el destino son el mismo vertice." << endl;
        return;
    }

    reiniciarVisitados();
    int n = contarVertices();
    Vertice** vertices = new Vertice*[n];
    double* valor = new double[n];
    Arco** vieneDe = new Arco*[n];
    int* anterior = new int[n];

    int i = 0;
    Vertice* actual = primerVertice;

    while (actual != nullptr) {
        vertices[i] = actual;
        valor[i] = numeric_limits<double>::infinity(); // valor inicial infinito
        vieneDe[i] = nullptr;         // no viene de nadie
        anterior[i] = -1;        // no tiene anterior
        actual = actual->getSig();
        i++;
    }

    valor[posicionDe(vertices, n, verticeOrigen)] = 0; // el origen tiene valor 0

    while (true) {
        int menor = -1;
        for (int k = 0; k < n; k++) { // k para no tapar a la i de afuera
            if (!vertices[k]->isVisitado() && valor[k] != numeric_limits<double>::infinity()
                && (menor == -1 || valor[k] < valor[menor])) { // se queda con el mas pequeno
                menor = k;
            }
        }

        if (menor == -1) {
            break; // todos los vertices han sido visitados o no hay mas alcanzables
        }

        Vertice* m = vertices[menor];
        m->setVisitado(true);
        if (m == verticeDestino) {
            break; // el destino ya quedo fijado
        }

        Arco* arcoActual = m->getPrimerArco();
        while (arcoActual != nullptr) {

            Vertice* vertice = arcoActual->getDestino();
            int j = posicionDe(vertices, n, vertice);

            if (!vertice->isVisitado()) {
                double suma = valor[menor] + arcoActual->getPerdida(); // perdida acumulada

                if (suma < valor[j]){
                    valor[j] = suma;
                    vieneDe[j] = arcoActual;
                    anterior[j] = menor;
                }
            }
            arcoActual = arcoActual->getSig();
        }
    }

    int posicionDestino = posicionDe(vertices, n, verticeDestino);
    if (valor[posicionDestino] == numeric_limits<double>::infinity()) {
        cout << "No hay ruta de " << origen << " a " << destino << endl;
    }
    else {
        // Contar los arcos de la ruta
        int largo = 0;
        int p = posicionDestino;
        while (anterior[p] != -1) {
            largo++;
            p = anterior[p];
        }

        // Llenar ruta de atras hacia adelante
        Arco** ruta = new Arco*[largo];
        p = posicionDestino;
        for (int k = largo - 1; k >= 0; k--) {
            ruta[k] = vieneDe[p];  // el arco con que se llego a p
            p = anterior[p];       // se retrocede un vertice
        }

        // Imprimir de inicio a fin
        cout << "Ruta de menor perdida de " << origen << " a " << destino << ":" << endl;
        for (int k = 0; k < largo; k++) {
            string nombreOrigen;
            if (k == 0) {
                nombreOrigen = verticeOrigen->getNombre();              // el primer arco sale del origen
            } else {
                nombreOrigen = ruta[k - 1]->getDestino()->getNombre();  // sale de donde llego el anterior
            }
            cout << "  " << nombreOrigen << " -> " << ruta[k]->getDestino()->getNombre()
                 << " [" << ruta[k]->getMedio() << ", " << ruta[k]->getPerdida() << " %]" << endl;
        }
        cout << "  Perdida total: " << valor[posicionDestino] << " %" << endl;

        delete[] ruta;
    }

    delete[] vertices;
    delete[] valor;
    delete[] vieneDe;
    delete[] anterior;
}

double Grafo::puntajeArco(Arco* arco) {
    double base = 5; // red_local
    if (arco->getMedio() == "alta_tension") {
        base = 10;
    } else if (arco->getMedio() == "subterraneo") {
        base = 8;
    }
    return base - arco->getPerdida() - 0.05 * arco->getDistancia();
}

void Grafo::consultarRutaMayorTokens(string origen, string destino, Cliente* cliente, bool horaNoPico) {
    // Validar
    Vertice* verticeOrigen = buscarVertice(origen);
    Vertice* verticeDestino = buscarVertice(destino);
    if (verticeOrigen == nullptr) {
        cout << "Error: El vertice '" << origen << "' no existe." << endl;
        return;
    }
    if (verticeDestino == nullptr) {
        cout << "Error: El vertice '" << destino << "' no existe." << endl;
        return;
    }
    if (verticeOrigen == verticeDestino) {
        cout << "Error: El origen y el destino son el mismo vertice." << endl;
        return;
    }

    // Buscar la ruta de mayor puntaje con los tres medios
    reiniciarVisitados();
    int n = contarVertices();
    Arco** ruta = new Arco*[n];
    Arco** mejorRuta = new Arco*[n];
    int cantidad = 0;
    int mejorLargo = 0;
    double mejorPuntaje = 0;
    buscarRutas(verticeOrigen, verticeOrigen, verticeDestino, true, true, true, ruta, 0, cantidad, true, mejorRuta, mejorLargo, mejorPuntaje);

    // Encontro alguna ruta
    if (cantidad == 0) {
        cout << "No hay ruta de " << origen << " a " << destino << endl;
        delete[] ruta;
        delete[] mejorRuta;
        return;
    }

    // Calcular tokens
    double total = mejorPuntaje;
    if (horaNoPico && total > 0) {
        total = total * 1.5; // bono de hora no pico
    }
    int tokens = (int) round(total);

    // Mostrar la mejor ruta
    cout << "Ruta de mayor Eco-Tokens de " << origen << " a " << destino << ":" << endl;
    for (int k = 0; k < mejorLargo; k++) {
        string nombreOrigen;
        if (k == 0) {
            nombreOrigen = verticeOrigen->getNombre(); // el primer arco sale del origen
        } else {
            nombreOrigen = mejorRuta[k - 1]->getDestino()->getNombre(); // sale de donde llego el anterior
        }
        cout << "  " << nombreOrigen << " -> " << mejorRuta[k]->getDestino()->getNombre()
             << " [" << mejorRuta[k]->getMedio() << ", " << puntajeArco(mejorRuta[k]) << " puntos]" << endl;
    }
    cout << "  Puntaje total: " << mejorPuntaje;
    if (horaNoPico && mejorPuntaje > 0) {
        cout << " x 1.5 por hora no pico = " << total;
    }
    cout << endl;

    // Tokens mayores que 0
    if (tokens <= 0) {
        cout << "La ruta no otorga tokens." << endl;
        delete[] ruta;
        delete[] mejorRuta;
        return;
    }
    cout << "  Tokens: " << tokens << endl;

    // El cliente acepta
    cout << "Acepta la ruta " << cliente->getNombre() << "? 1 = si y 0 = no: ";
    int respuesta;
    while (!(cin >> respuesta) || (respuesta != 0 && respuesta != 1)) { // lectura segura
        cout << "Valor invalido. Escriba 1 o 0: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n'); // limpia el Enter

    if (respuesta == 0) {
        cout << "Sin cambios." << endl;
        delete[] ruta;
        delete[] mejorRuta;
        return;
    }

    // Actualizar cliente y usos
    cliente->setTokens(cliente->getTokens() + tokens);
    if (verticeOrigen->getTipo() != "generador" && !cliente->tieneSubestacion(verticeOrigen->getNombre())) {
        cliente->agregarSubestacion(verticeOrigen->getNombre());
    }
    for (int k = 0; k < mejorLargo; k++) {
        mejorRuta[k]->setUsos(mejorRuta[k]->getUsos() + 1); // cada linea suma un uso
        Vertice* llegada = mejorRuta[k]->getDestino();
        if (llegada->getTipo() != "generador" && !cliente->tieneSubestacion(llegada->getNombre())) {
            cliente->agregarSubestacion(llegada->getNombre()); // sin repetir
        }
    }
    cout << "Ruta aceptada. " << cliente->getNombre() << " ahora tiene " << cliente->getTokens() << " tokens." << endl;

    // Liberar
    delete[] ruta;
    delete[] mejorRuta;
}

void Grafo::imprimirRutaFrecuenteYUnica() {}

void Grafo::imprimirNodosInactivos(ListaClientes& clientes) {
    int inactivos = 0;
    cout << "========== Nodos inactivos ==========" << endl;
    Vertice* actual = primerVertice;
    while (actual != nullptr) {
        if (actual->getTipo() != "generador") {
            bool activo = false;
            Cliente* clienteActual = clientes.getPrimero();
            while (clienteActual != nullptr && !activo) {
                if (clienteActual->tieneSubestacion(actual->getNombre())) {
                    activo = true; // se encontro al menos un cliente que lo uso
                }
                clienteActual = clienteActual->getSig();
            }
            if (!activo) {
                cout << actual->getNombre() << " (" << actual->getTipo() << ")" << endl;
                inactivos++;
            }
        }
        actual = actual->getSig();
    }
    if (inactivos == 0) {
        cout << "Ninguno" << endl;
        return; // todos los nodos fueron usados por al menos un cliente
    }
    cout << "Total de nodos inactivos: " << inactivos << endl;
    return;
}