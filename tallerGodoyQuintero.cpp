#include <iostream>
#include <fstream>
#include <string>

using namespace std;


struct Arista{
    int destino;
    int peso;
    Arista* next;
};

struct Node_vertice{
    int value;
    int peso;
    Node_vertice* next;
    Arista* down;
};

Node_vertice* crearNodo(int valor) {
    Node_vertice* nuevo = new Node_vertice;
    nuevo->value = valor;
    nuevo->next = NULL;
    nuevo->down = NULL;
    return nuevo;
}

Node_vertice* buscarNodo(Node_vertice* cabeza, int valor) {
    Node_vertice* actual = cabeza;
    while (actual != NULL) {
        if (actual->value == valor) return actual;
        actual = actual->next;
    }
    return NULL; 
}

void agregarArista(Node_vertice** cabeza, int origen, int destino, int peso){
    Node_vertice* nodoOrigen = buscarNodo(*cabeza, origen);
    if (nodoOrigen == NULL) {
        nodoOrigen = crearNodo(origen);
        nodoOrigen->next = *cabeza;
        *cabeza = nodoOrigen;
    }

    Node_vertice* nodoDestino = buscarNodo(*cabeza, destino);
    if (nodoDestino == NULL) {
        nodoDestino = crearNodo(destino);
        nodoDestino->next = *cabeza;
        *cabeza = nodoDestino;
    }

    Arista* arista1 = new Arista{destino, peso, nodoOrigen->down};
    nodoOrigen->down = arista1;

    Arista* arista2 = new Arista{origen, peso, nodoDestino->down};
    nodoDestino->down = arista2;
}

Node_vertice* leerArchivo(){
    ifstream archivo("estructura.txt");
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return NULL;
    }

    int total_nodos;
    archivo >> total_nodos;

    Node_vertice* cabeza = NULL;
    int origen, destino, peso;

    while (archivo >> origen >> destino >> peso) {
        agregarArista(&cabeza, origen, destino, peso);
    }

    archivo.close();
    return cabeza;
}


void imprimirGrafo(Node_vertice* cabeza){
    Node_vertice* actualNodo = cabeza;
    cout << "El grafo implementado con Lista de Adyacencia seria:\n";
    cout << "\t(Vertice conectado, Peso)\n\n";
    while (actualNodo != NULL) {
        cout << "Nodo " << actualNodo->value << " -> ";
        Arista* actualArista = actualNodo->down;
        while (actualArista != NULL) {
            cout << " ( " << actualArista->destino << " , " << actualArista->peso << " )  ";
            actualArista = actualArista->next;
        }
        cout << endl;
        actualNodo = actualNodo->next;
    }
}

void liberarGrafo(Node_vertice* cabeza){
    Node_vertice* actualNodo = cabeza;
    while (actualNodo != NULL){
        Arista* actualArista = actualNodo->down;
        while (actualArista != NULL) {
            Arista* temp = actualArista;
            actualArista = actualArista->next;
            delete temp;
        }
        Node_vertice* tempNodo = actualNodo;
        actualNodo = actualNodo->next;
        delete tempNodo;
    }
}

int contarVertices(Node_vertice* grafo) {
    int count = 0;
    Node_vertice* actual = grafo;
    while (actual != NULL) {
        count++;
        actual = actual->next;
    }
    return count;
}

int MAX=9999;
void primAMC(Node_vertice* grafo) {
    if (grafo == NULL) {
        cout << "El grafo está vacío" << endl;
        return;
    }

    int numVertices = contarVertices(grafo);

    if (numVertices == 0) {
        cout << "No hay vértices en el grafo" << endl;
        return;
    }

    bool enAMC[numVertices]; 
    for (int i = 0; i < numVertices; i++) {
        enAMC[i] = false;  
    }
    int pesoMinimo[numVertices]; 
    int verticePadre[numVertices]; 

    for (int i = 0; i < numVertices; i++) {
        pesoMinimo[i] = MAX;  
    }
    pesoMinimo[0] = 0;
    verticePadre[0] = -1; 

    for (int count = 0; count < numVertices - 1; count++) {
        int minPeso=MAX;
        int u = -1;
        
        for (int v = 0; v < numVertices; v++) {
            if (!enAMC[v] && pesoMinimo[v] < minPeso) {
                minPeso = pesoMinimo[v];
                u = v;
            }
        }

        if (u == -1) {
            cout << "El grafo no es conexo, no se puede completar el AMC." << endl;

            return;
        }

        enAMC[u] = true;

        Node_vertice* verticeU = buscarNodo(grafo, u);
        Arista* arista = verticeU->down;
        
        while (arista != NULL) {
            int v = arista->destino; 
            int peso = arista->peso;

            if (!enAMC[v] && peso < pesoMinimo[v]) {
                verticePadre[v] = u;
                pesoMinimo[v] = peso;
            }
            arista = arista->next;
        }
    }

    cout << "\n\nAlgoritmo de Prim:\n";
    cout << "\n\tArbol Minimo Cobertor (AMC):\n\n";
    cout << "\tArista (Rama) \tPeso\n";
    
    int pesoTotal = 0;
    for (int i = 1; i < numVertices; i++) {
        cout << "\t( " << verticePadre[i] << " - " << i << " )\t\t" << pesoMinimo[i] << endl;
        pesoTotal += pesoMinimo[i];
    }
    
    cout << "\nPeso total del Arbol Minimo Cobertor: " << pesoTotal << endl;

}

int main(){
    Node_vertice* grafo = leerArchivo();  
    if (grafo != NULL) {
        imprimirGrafo(grafo);
        primAMC(grafo);
        liberarGrafo(grafo);
    }
    return 0;
}