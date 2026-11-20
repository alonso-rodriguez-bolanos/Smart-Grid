#include "ArchivoJSON.h"
#include "json.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;
using json = nlohmann::json;

// ======================================================================
// Funciones de apoyo. Viven solo en este .cpp para no exponer json.hpp
// ======================================================================
namespace {

// Nombres de los campos de los JSON, iguales a los del enunciado
const char* K_REGIONES     = "regiones";
const char* K_NODOS        = "puntos_interconexion";
const char* K_LINEAS       = "lineas_transmision";
const char* K_NOMBRE       = "nombre";
const char* K_TIPO         = "tipo";
const char* K_ORIGEN       = "origen";
const char* K_DESTINO      = "destino";
const char* K_REGION       = "region";
const char* K_PUNTO        = "punto";
const char* K_MEDIO        = "medio";
const char* K_DISTANCIA    = "distancia_km";
const char* K_CAPACIDAD    = "capacidad_max";
const char* K_PERDIDA      = "perdida_porcentaje";
const char* K_USOS         = "usos";
const char* K_CLIENTES     = "clientes";
const char* K_TOKENS       = "tokens";
const char* K_SUBESTACIONES = "subestaciones_conectadas";
const char* K_CANJEADOS    = "beneficios";            // beneficios canjeados de cada cliente
const char* K_BENEFICIOS   = "beneficios";
const char* K_COSTO        = "costo";

// Valores por defecto si una linea no trae capacidad_max o usos (el JSON del enunciado no los trae)
const double CAPACIDAD_POR_DEFECTO = 100.0;
const int USOS_POR_DEFECTO = 0;

enum EstadoLectura { LEIDO, NO_EXISTE, MAL_FORMADO };

// Abre y analiza un archivo JSON. Solo informa el error de sintaxis;
// que hacer si no existe lo decide quien llama (las reglas cambian por archivo)
EstadoLectura leerArchivo(const string& ruta, json& destino) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        return NO_EXISTE;
    }
    try {
        destino = json::parse(archivo);
    } catch (const json::parse_error& e) {
        cout << "ERROR: " << ruta << " esta mal formado (" << e.what() << ")" << endl;
        return MAL_FORMADO;
    }
    return LEIDO;
}

// Lectores de campos obligatorios. Si falta el campo o el tipo no coincide
// lanzan runtime_error: el archivo se considera mal formado
const json& campo(const json& objeto, const char* clave) {
    if (!objeto.is_object()) {
        throw runtime_error("se esperaba un objeto { } con el campo \"" + string(clave) + "\"");
    }
    if (!objeto.contains(clave)) {
        throw runtime_error("falta el campo \"" + string(clave) + "\"");
    }
    return objeto.at(clave);
}

const json& campoLista(const json& objeto, const char* clave) {
    const json& valor = campo(objeto, clave);
    if (!valor.is_array()) {
        throw runtime_error("el campo \"" + string(clave) + "\" debe ser una lista [ ]");
    }
    return valor;
}

string campoTexto(const json& objeto, const char* clave) {
    const json& valor = campo(objeto, clave);
    if (!valor.is_string()) {
        throw runtime_error("el campo \"" + string(clave) + "\" debe ser texto");
    }
    return valor.get<string>();
}

double campoNumero(const json& objeto, const char* clave) {
    const json& valor = campo(objeto, clave);
    if (!valor.is_number()) {
        throw runtime_error("el campo \"" + string(clave) + "\" debe ser un numero");
    }
    return valor.get<double>();
}

int campoEntero(const json& objeto, const char* clave) {
    const json& valor = campo(objeto, clave);
    if (!valor.is_number_integer()) { // usos, tokens y costos son enteros
        throw runtime_error("el campo \"" + string(clave) + "\" debe ser un numero entero");
    }
    return valor.get<int>();
}

// Campo numerico opcional: si no esta, devuelve el valor por defecto y lo indica en falto
double campoNumeroOpcional(const json& objeto, const char* clave, double porDefecto, bool& falto) {
    falto = !objeto.contains(clave);
    if (falto) {
        return porDefecto;
    }
    return campoNumero(objeto, clave);
}

int campoEnteroOpcional(const json& objeto, const char* clave, int porDefecto) {
    if (!objeto.contains(clave)) {
        return porDefecto;
    }
    return campoEntero(objeto, clave);
}

// Extremo de una linea en el formato del enunciado: {"region": "...", "punto": "..."}
// Devuelve el nombre del punto, que es el nombre del vertice
string campoPunto(const json& linea, const char* clave) {
    const json& extremo = campo(linea, clave);
    if (!extremo.is_object()) {
        throw runtime_error("el campo \"" + string(clave) + "\" debe ser un objeto con region y punto");
    }
    campoTexto(extremo, K_REGION); // obligatorio aunque la region ya esta en el vertice
    return campoTexto(extremo, K_PUNTO);
}

// Lista de textos opcional: si el campo no esta, se toma como lista vacia
const json& listaTextosOpcional(const json& objeto, const char* clave) {
    static const json vacia = json::array();
    if (!objeto.contains(clave)) {
        return vacia;
    }
    const json& lista = campoLista(objeto, clave);
    for (const json& elemento : lista) {
        if (!elemento.is_string()) {
            throw runtime_error("la lista \"" + string(clave) + "\" solo puede tener textos");
        }
    }
    return lista;
}

// Escribe primero en un archivo temporal y luego lo pone en su lugar:
// si algo falla a medias, el archivo original no queda danado
bool escribirArchivo(const string& ruta, const json& datos) {
    string temporal = ruta + ".tmp";
    {
        ofstream salida(temporal);
        if (!salida.is_open()) {
            cout << "ERROR: no se pudo crear " << temporal << endl;
            return false;
        }
        // replace: si se escribio una tilde desde la consola de Windows no revienta
        salida << datos.dump(2, ' ', false, json::error_handler_t::replace) << endl;
        if (!salida.good()) {
            cout << "ERROR: fallo la escritura de " << temporal << endl;
            return false;
        }
    }
    remove(ruta.c_str()); // en Windows rename falla si el destino ya existe
    if (rename(temporal.c_str(), ruta.c_str()) != 0) {
        cout << "ERROR: no se pudo reemplazar " << ruta << " (los datos quedaron en " << temporal << ")" << endl;
        return false;
    }
    return true;
}

} // namespace

// ======================================================================
// Clase ArchivoJSON
// ======================================================================
ArchivoJSON::ArchivoJSON(string rutaRed, string rutaClientes, string rutaBeneficios) {
    this->rutaRed = rutaRed;
    this->rutaClientes = rutaClientes;
    this->rutaBeneficios = rutaBeneficios;
}

bool ArchivoJSON::cargarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios) {
    cout << "Cargando datos..." << endl;
    // Orden obligatorio: la red primero, porque las subestaciones de los clientes se validan contra sus vertices
    if (!cargarRed(red)) {
        return false;
    }
    if (!cargarBeneficios(beneficios)) {
        return false;
    }
    if (!cargarClientes(clientes, red)) {
        return false;
    }
    cout << "Carga terminada" << endl << endl;
    return true;
}

// ---------------------------------------------------------------- red
bool ArchivoJSON::cargarRed(Grafo& red) {
    json datos;
    EstadoLectura estado = leerArchivo(rutaRed, datos);
    if (estado == NO_EXISTE) {
        cout << "ERROR: no se encontro " << rutaRed << ". Sin la red el programa no puede continuar" << endl;
        return false;
    }
    if (estado == MAL_FORMADO) {
        cout << "El programa termina sin modificar ningun archivo" << endl;
        return false;
    }

    string ubicacion = "la raiz del archivo";
    int vertices = 0, verticesRechazados = 0, arcos = 0, arcosRechazados = 0, sinCapacidad = 0;
    try {
        const json& regiones = campoLista(datos, K_REGIONES);
        const json& lineas = campoLista(datos, K_LINEAS);

        // 1) Todos los vertices antes que los arcos: se aplanan las regiones y la region queda como texto
        for (size_t r = 0; r < regiones.size(); r++) {
            ubicacion = "la region " + to_string(r + 1);
            string region = campoTexto(regiones[r], K_NOMBRE);
            const json& nodos = campoLista(regiones[r], K_NODOS);
            for (size_t n = 0; n < nodos.size(); n++) {
                ubicacion = "el nodo " + to_string(n + 1) + " de la region " + region;
                string nombre = campoTexto(nodos[n], K_NOMBRE);
                string tipo = campoTexto(nodos[n], K_TIPO);
                if (red.insertarVertice(nombre, tipo, region)) {
                    vertices++;
                } else { // Persona 1 ya imprimio el motivo
                    cout << "  AVISO: se omitio el nodo \"" << nombre << "\" de " << rutaRed << endl;
                    verticesRechazados++;
                }
            }
        }

        // 2) Arcos
        for (size_t i = 0; i < lineas.size(); i++) {
            ubicacion = "la linea de transmision " + to_string(i + 1);
            const json& linea = lineas[i];
            string origen = campoPunto(linea, K_ORIGEN);
            string destino = campoPunto(linea, K_DESTINO);
            string medio = campoTexto(linea, K_MEDIO);
            double distancia = campoNumero(linea, K_DISTANCIA);
            bool faltoCapacidad = false;
            double capacidad = campoNumeroOpcional(linea, K_CAPACIDAD, CAPACIDAD_POR_DEFECTO, faltoCapacidad);
            if (faltoCapacidad) {
                sinCapacidad++;
            }
            double perdida = campoNumero(linea, K_PERDIDA);
            int usos = campoEnteroOpcional(linea, K_USOS, USOS_POR_DEFECTO);
            if (red.insertarArco(origen, destino, medio, distancia, capacidad, perdida, usos)) {
                arcos++;
            } else {
                cout << "  AVISO: se omitio la linea " << origen << " -> " << destino
                     << " (" << medio << ") de " << rutaRed << endl;
                arcosRechazados++;
            }
        }
    } catch (const exception& e) {
        cout << "ERROR: " << rutaRed << " no tiene el formato esperado en " << ubicacion
             << ": " << e.what() << endl;
        cout << "El programa termina sin modificar ningun archivo" << endl;
        return false;
    }

    cout << "  Red: " << vertices << " nodos y " << arcos << " lineas cargadas" << endl;
    if (sinCapacidad > 0) {
        cout << "  AVISO: " << sinCapacidad << " lineas no traian capacidad_max. Se les asigno "
             << CAPACIDAD_POR_DEFECTO << endl;
    }
    if (verticesRechazados + arcosRechazados > 0) {
        cout << "  AVISO: se omitieron " << verticesRechazados << " nodos y " << arcosRechazados
             << " lineas invalidas. Al guardar ya no apareceran en " << rutaRed << endl;
    }
    return true;
}

// ---------------------------------------------------------------- beneficios
bool ArchivoJSON::cargarBeneficios(ListaBeneficios& beneficios) {
    json datos;
    EstadoLectura estado = leerArchivo(rutaBeneficios, datos);
    if (estado == NO_EXISTE) {
        cout << "  AVISO: no se encontro " << rutaBeneficios << ". Se inicia con el catalogo vacio" << endl;
        return true;
    }
    if (estado == MAL_FORMADO) {
        cout << "El programa termina sin modificar ningun archivo" << endl;
        return false;
    }

    string ubicacion = "la raiz del archivo";
    int cargados = 0, rechazados = 0;
    try {
        const json& lista = campoLista(datos, K_BENEFICIOS);
        for (size_t i = 0; i < lista.size(); i++) {
            ubicacion = "el beneficio " + to_string(i + 1);
            string nombre = campoTexto(lista[i], K_NOMBRE);
            int costo = campoEntero(lista[i], K_COSTO);
            if (beneficios.insertarBeneficio(nombre, costo)) {
                cargados++;
            } else {
                cout << "  AVISO: se omitio el beneficio \"" << nombre << "\" de " << rutaBeneficios << endl;
                rechazados++;
            }
        }
    } catch (const exception& e) {
        cout << "ERROR: " << rutaBeneficios << " no tiene el formato esperado en " << ubicacion
             << ": " << e.what() << endl;
        cout << "El programa termina sin modificar ningun archivo" << endl;
        return false;
    }

    cout << "  Beneficios: " << cargados << " cargados" << endl;
    if (rechazados > 0) {
        cout << "  AVISO: se omitieron " << rechazados << " beneficios invalidos" << endl;
    }
    return true;
}

// ---------------------------------------------------------------- clientes
bool ArchivoJSON::cargarClientes(ListaClientes& clientes, Grafo& red) {
    json datos;
    EstadoLectura estado = leerArchivo(rutaClientes, datos);
    if (estado == NO_EXISTE) {
        cout << "  AVISO: no se encontro " << rutaClientes << ". Se inicia sin clientes" << endl;
        return true;
    }
    if (estado == MAL_FORMADO) {
        cout << "El programa termina sin modificar ningun archivo" << endl;
        return false;
    }

    string ubicacion = "la raiz del archivo";
    int cargados = 0, rechazados = 0, subestacionesOmitidas = 0;
    try {
        const json& lista = campoLista(datos, K_CLIENTES);
        for (size_t i = 0; i < lista.size(); i++) {
            ubicacion = "el cliente " + to_string(i + 1);
            const json& cliente = lista[i];
            string nombre = campoTexto(cliente, K_NOMBRE);
            int tokens = campoEntero(cliente, K_TOKENS);
            // Se revisan las sublistas antes de insertar para no dejar un cliente a medias
            const json& subestaciones = listaTextosOpcional(cliente, K_SUBESTACIONES);
            const json& canjeados = listaTextosOpcional(cliente, K_CANJEADOS);

            if (!clientes.insertarCliente(nombre, tokens)) {
                cout << "  AVISO: se omitio el cliente \"" << nombre << "\" de " << rutaClientes << endl;
                rechazados++;
                continue;
            }
            cargados++;

            for (const json& s : subestaciones) {
                string subestacion = s.get<string>();
                if (!red.existeVertice(subestacion)) { // subestacion inexistente: se ignora con aviso
                    cout << "  AVISO: el cliente \"" << nombre << "\" tiene la subestacion \"" << subestacion
                         << "\" que no existe en la red. Se ignora" << endl;
                    subestacionesOmitidas++;
                    continue;
                }
                clientes.agregarSubestacionACliente(nombre, subestacion);
            }
            for (const json& b : canjeados) {
                // No se valida contra el catalogo: si el beneficio se borro, el cliente lo conserva
                clientes.agregarBeneficioACliente(nombre, b.get<string>());
            }
        }
    } catch (const exception& e) {
        cout << "ERROR: " << rutaClientes << " no tiene el formato esperado en " << ubicacion
             << ": " << e.what() << endl;
        cout << "El programa termina sin modificar ningun archivo" << endl;
        return false;
    }

    cout << "  Clientes: " << cargados << " cargados" << endl;
    if (rechazados > 0) {
        cout << "  AVISO: se omitieron " << rechazados << " clientes invalidos" << endl;
    }
    if (subestacionesOmitidas > 0) {
        cout << "  AVISO: se ignoraron " << subestacionesOmitidas << " subestaciones inexistentes" << endl;
    }
    return true;
}

// ======================================================================
// Guardado
// ======================================================================
bool ArchivoJSON::guardarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios) {
    // Se intentan los 3 aunque uno falle, para perder lo menos posible
    bool okRed = guardarRed(red);
    bool okBeneficios = guardarBeneficios(beneficios);
    bool okClientes = guardarClientes(clientes);
    if (okRed && okBeneficios && okClientes) {
        cout << "Datos guardados en " << rutaRed << ", " << rutaClientes << " y " << rutaBeneficios << endl;
        return true;
    }
    return false;
}

bool ArchivoJSON::guardarRed(Grafo& red) {
    json regiones = json::array();
    json lineas = json::array();

    for (Vertice* v = red.getPrimerVertice(); v != nullptr; v = v->getSig()) {
        // Se reagrupan los vertices por region, en el orden en que aparece cada region
        json* region = nullptr;
        for (json& r : regiones) {
            if (r[K_NOMBRE] == v->getRegion()) {
                region = &r;
                break;
            }
        }
        if (region == nullptr) {
            regiones.push_back({{K_NOMBRE, v->getRegion()}, {K_NODOS, json::array()}});
            region = &regiones.back();
        }
        (*region)[K_NODOS].push_back({{K_NOMBRE, v->getNombre()}, {K_TIPO, v->getTipo()}});

        for (Arco* a = v->getPrimerArco(); a != nullptr; a = a->getSig()) {
            lineas.push_back({
                {K_ORIGEN, {{K_REGION, v->getRegion()}, {K_PUNTO, v->getNombre()}}},
                {K_DESTINO, {{K_REGION, a->getDestino()->getRegion()}, {K_PUNTO, a->getDestino()->getNombre()}}},
                {K_MEDIO, a->getMedio()},
                {K_DISTANCIA, a->getDistancia()},
                {K_CAPACIDAD, a->getCapacidad()},
                {K_PERDIDA, a->getPerdida()},
                {K_USOS, a->getUsos()} // int
            });
        }
    }

    json datos;
    datos[K_REGIONES] = regiones;
    datos[K_LINEAS] = lineas;
    return escribirArchivo(rutaRed, datos);
}

bool ArchivoJSON::guardarBeneficios(ListaBeneficios& beneficios) {
    json lista = json::array();
    for (Beneficio* b = beneficios.getPrimero(); b != nullptr; b = b->getSig()) {
        lista.push_back({{K_NOMBRE, b->getNombre()}, {K_COSTO, b->getCosto()}});
    }
    json datos;
    datos[K_BENEFICIOS] = lista;
    return escribirArchivo(rutaBeneficios, datos);
}

bool ArchivoJSON::guardarClientes(ListaClientes& clientes) {
    json lista = json::array();
    for (Cliente* c = clientes.getPrimero(); c != nullptr; c = c->getSig()) {
        json subestaciones = json::array();
        for (NodoTexto* s = c->getPrimeraSubestacion(); s != nullptr; s = s->getSig()) {
            subestaciones.push_back(s->getNombre());
        }
        json canjeados = json::array();
        for (NodoTexto* b = c->getPrimerBeneficio(); b != nullptr; b = b->getSig()) {
            canjeados.push_back(b->getNombre());
        }
        lista.push_back({
            {K_NOMBRE, c->getNombre()},
            {K_TOKENS, c->getTokens()},
            {K_SUBESTACIONES, subestaciones},
            {K_CANJEADOS, canjeados}
        });
    }
    json datos;
    datos[K_CLIENTES] = lista;
    return escribirArchivo(rutaClientes, datos);
}
