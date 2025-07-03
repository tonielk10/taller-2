#include <iostream>
#include <fstream>
#include <string>
#include "color.h"

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
    colorP (10, "El grafo implementado con Lista de Adyacencia seria:");
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

int MAX=9999999;
void primAMC(Node_vertice* grafo) {
    if (grafo == NULL) {
        cout << "El grafo está vacío" << endl;
        return;
    }

    int numVertices = contarVertices(grafo);

    if (numVertices == 0) {
        cout << "No hay vertices en el grafo" << endl;
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

    colorP (13, "\n\nAlgoritmo de Prim:");
    colorP (6, "\tArbol Minimo Cobertor (AMC):");
    cout << "\tArista (Rama) \tPeso\n";
    
    int pesoTotal = 0;
    for (int i = 1; i < numVertices; i++) {
        cout << "\t  ( " << verticePadre[i] << " - " << i << " )\t " << pesoMinimo[i] << endl;
        pesoTotal += pesoMinimo[i];
    }
    
    cout << "\nPeso total del Arbol Minimo Cobertor: " << pesoTotal << endl;

}

// Función para imprimir el camino más corto desde el vertice indicado hasta el vertice destinatario
void imprimirCamino(int destino, int verticePadre[], int origen) {
    if (destino == origen) {
        cout << origen;
        return;
    }
    if (verticePadre[destino] == -1) {
        cout << "No hay camino";
        return;
    }
    imprimirCamino(verticePadre[destino], verticePadre, origen);
    cout << " -> " << destino;
    
}

// Algoritmo de Dijkstra para encontrar el camino mas corto desde un vertice origen
void djikstra (Node_vertice* grafo, int verticeOrigen){
    if (grafo == NULL) {
        cout << "El grafo está vacío" << endl;
        return;
    }

    // Contar numero de vertices
     int numVertices = contarVertices(grafo);

    if (numVertices == 0) {
        cout << "No hay vertices en el grafo" << endl;
        return;
    }

    // Verificar que el vertice de origen existe
    if (buscarNodo(grafo, verticeOrigen) == NULL) {
        cout << "El vertice origen " << verticeOrigen << " no existe en el grafo" << endl;
        return;
    }

    bool procesado[numVertices]; // Arreglo de booleanos para validar que ha sido procesado/marcado
    for (int i = 0; i < numVertices; i++) {
        procesado[i] = false;  // Inicializa todas las posiciones del arreglo como falso
    }

    int distancia[numVertices]; // Arreglo que guarda las distancias mínimas con respecto al origen
    int verticePadre[numVertices]; // Guarda el vertice padre para reconstruir el camino

    // Inicializar distancias como el maximo (9999999) y padres como -1
    for (int i = 0; i < numVertices; i++) {
        distancia[i] = MAX;  // Inicializa todas las distancias como el "maximo"
        verticePadre[i] = -1;
    }
    
    distancia[verticeOrigen] = 0; // La distancia entre el origen y si mismo, es 0

    // Procesar todos los vértices
    for (int count = 0; count < numVertices; count++) {
        // Encontrar el vértice no procesado con la distancia mínima
        int minDistancia = MAX;
        int u = -1;
        
        // Busca el vértice no procesado con la menor distancia
        for (int v = 0; v < numVertices; v++) {
            if (!procesado[v] && distancia[v] < minDistancia) {
                minDistancia = distancia[v];
                u = v;
            }
        }

        // Si no encontramos un vértice disponible, entonces el grafo no es conexo desde el origen
        if (u == -1) {
            break;
        }

        procesado[u] = true; // Marcar el vértice como procesado

        // Actualizar las distancias de los vértices adyacentes
        Node_vertice* verticeU = buscarNodo(grafo, u);
        Arista* arista = verticeU->down;
        
        while (arista != NULL) {
            int v = arista->destino; // El nodo destino
            int peso = arista->peso;

            // Si encontramos un camino más corto hacia v a través de u
            if (!procesado[v] && distancia[u] != MAX && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                verticePadre[v] = u;
            }
            arista = arista->next;
        }
    }

    // Mostrar los resultados
    cout << "\nCaminos más cortos partiendo desde el vértice " << verticeOrigen << ":\n";
    cout << "Destino\tDistancia\tCamino\n";
    
    for (int i = 0; i < numVertices; i++) {
        cout << i << "\t";
        if (distancia[i] == MAX) {
            cout << "\t\tNo se puede acceder.";
        } else {
            cout << distancia[i] << "\t\t";
            imprimirCamino(i, verticePadre, verticeOrigen);
        }
        cout << endl;
    }

}


int obtenerIndice(Node_vertice* grafo, int vertice) {
    int indice = 0;
    Node_vertice* actual = grafo;
    while (actual != NULL) {
        if (actual->value == vertice) { 
            return indice;
        }
        actual = actual->next;
        indice++;
    }
    return -1; // No fue encontrado
}


// Función para comprobar si un color esta disponible para el vertice
bool colorDisponible(Node_vertice* vertice, int colores[], int color, Node_vertice* grafo) {
    Arista* arista = vertice->down;
    while (arista != NULL) {
        int idxVecino = obtenerIndice(grafo, arista->destino);
        if (idxVecino != -1 && colores[idxVecino] == color) {
            return false;
        }
        arista = arista->next;
    }
    return true;
}

void colorearGrafo(Node_vertice* grafo, int numVertices) {
int* colores = new int[numVertices];
const char* nombresColores[] = {
    "Rojo", "Verde", "Azul", "Amarillo", "Naranja",
    "Morado", "Cian", "Rosa", "Gris", "Marrón"
};

int codigosColores[] = {
    12, // Rojo
    10, // Verde
    9,  // Azul
    14, // Amarillo
    6,  // Naranja 
    13, // Morado
    11, // Cian
    13, // Rosa 
    8,  // Gris
    4   // Marrón 
};

    
    // Inicializar todos los colores como no asignados (-1)
    for (int i = 0; i < numVertices; i++) {
        colores[i] = -1;
    }

    Node_vertice* actual = grafo;
    while (actual != NULL) {
        int indice = obtenerIndice(grafo, actual->value);

        // Probar colores desde 0 hasta numVertices - 1
        for (int c = 0; c < numVertices; c++) {
            if (colorDisponible(actual, colores, c, grafo)) {
                colores[indice] = c;
                break;
            }
        }

        actual = actual->next;
    }

    // Mostrar grafo coloreado
    cout << "Coloración del grafo:\n";
    actual = grafo;
    while (actual != NULL) {
        int indice = obtenerIndice(grafo, actual->value);
        int color = colores[indice];
        cout << "Vértice " << actual->value << " -> ";
        colorP(codigosColores[color], nombresColores[color]);
        actual = actual -> next;
}

    delete[] colores;
}

int main(){
   int verticeInicio, op;
    Node_vertice* grafo = leerArchivo();  
        if (grafo != NULL) {
            int numVertices = contarVertices(grafo);
            cout << "TALLER #2 Grafos. Realizado por Enrique Godoy y Jose Quintero\n";

        while (op != 5){
            colorP(9, "\n      MENU");
            cout << "1.  Imprimir Grafo\n";
            cout << "2.  Ver Arbol Minimo CObertor (Algoritmo de Prim)\n";
            cout << "3.  Ver el Camino de Costo Minimo partiendo desde un Vertice (Algoritmo de Djikstra)\n";
            cout << "4.  Ver el coloreado de cada vertice del Grafo (Algoritmo Boraz de Coloracion)\n";
            cout << "5.  Cerrar Programa\n";
            cout << "\n\t Indique el numero de operacion que desea realizar: ";
            cin >> op;
            color_siguiente(7, " ");


                switch (op){
            case 1:
                imprimirGrafo(grafo);
                break;

            case 2:
                primAMC(grafo);
                break;

            case 3:
                colorP (1, "\n\nAlgoritmo de Djikstra:");
                    do{
                    cout << "\n\tIndique el Vertice desde donde se desea conocer el camino de menor costo hacia todos los vertices: \n";
                    cout << "\tVertices disponibles: ( 0 - " << numVertices-1 << " ): ";
                    cin >> verticeInicio;
                    if (verticeInicio < 0 || verticeInicio > numVertices){
                        cout << "ERROR. El vertice debe estar entre el rango indicado ( 0 - " << numVertices-1 << " )\n";
                        cout << "Intentelo nuevamente\n";
                    }
                    } while (verticeInicio < 0 || verticeInicio > numVertices);
                    djikstra(grafo, verticeInicio);
                break;

            case 4:
                colorP(5, "Algoritmo Boraz de coloración");
                colorearGrafo(grafo, numVertices);
                break;

            case 5:
                liberarGrafo(grafo);
                cout << "\tLiberando memoria...\n";
                cout << "\tCerrando el programa...";
                break;

            default:
                cout << "Opcion no valida";
                break;
            }
        }  
    return 0;
    }
}
