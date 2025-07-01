#include <iostream>
#include <fstream>
#include <string>

using namespace std;


struct Arista{
    int destino;
    int peso;
    Arista* next;
};

struct Node_lista{
    int value;
    int peso;
    Node_lista* next;
    Arista* down;
};

Node_lista* crearNodo(int valor) {
    Node_lista* nuevo = new Node_lista;
    nuevo->value = valor;
    nuevo->next = nullptr;
    nuevo->down = nullptr;
    return nuevo;
}

Node_lista* buscarNodo(Node_lista* cabeza, int valor) {
    Node_lista* actual = cabeza;
    while (actual != nullptr) {
        if (actual->value == valor) return actual;
        actual = actual->next;
    }
    return nullptr; 
}

void agregarArista(Node_lista** cabeza, int origen, int destino, int peso){
    Node_lista* nodoOrigen = buscarNodo(*cabeza, origen);
    if (nodoOrigen == nullptr) {
        nodoOrigen = crearNodo(origen);
        nodoOrigen->next = *cabeza;
        *cabeza = nodoOrigen;
    }

    Arista* nuevaArista = new Arista;
    nuevaArista->destino = destino;
    nuevaArista->peso = peso;
    nuevaArista->next = nodoOrigen->down;
    nodoOrigen->down = nuevaArista;
}

Node_lista* leerArchivo(){
    ifstream archivo("estructura.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return nullptr;
    }

    int total_nodos;
    archivo >> total_nodos;

    Node_lista* cabeza = nullptr;
    int origen, destino, peso;

    while (archivo >> origen >> destino >> peso) {
        agregarArista(&cabeza, origen, destino, peso);
    }

    archivo.close();
    return cabeza;
}


void imprimirGrafo(Node_lista* cabeza){
    Node_lista* actualNodo = cabeza;
    cout << "El grafo implementado con Lista de Adyacencia seria:\n";
    while (actualNodo != nullptr) {
        cout << "Nodo " << actualNodo->value << " -> ";
        Arista* actualArista = actualNodo->down;
        while (actualArista != nullptr) {
            cout << "( Conectado con: " << actualArista->destino << ", Peso: " << actualArista->peso << ") ";
            actualArista = actualArista->next;
        }
        cout << endl;
        actualNodo = actualNodo->next;
    }
}

void liberarGrafo(Node_lista* cabeza){
    Node_lista* actualNodo = cabeza;
    while (actualNodo != nullptr){
        Arista* actualArista = actualNodo->down;
        while (actualArista != nullptr) {
            Arista* temp = actualArista;
            actualArista = actualArista->next;
            delete temp;
        }
        Node_lista* tempNodo = actualNodo;
        actualNodo = actualNodo->next;
        delete tempNodo;
    }
}

int main(){
    Node_lista* grafo = leerArchivo();  
    if (grafo != nullptr) {
        imprimirGrafo(grafo);
        liberarGrafo(grafo);
    }
    return 0;
}