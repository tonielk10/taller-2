#include <iostream>
#include <fstream>
#include <string>
#include "color.h"

using namespace std;

//estructura aplicando multilista de adyacencia

//estructura para las aristas del grafo
struct Arista{
    int destino;
    int peso;
    Arista* next;
};

//estructura para cada vertice del grafo
struct Node_vertice{
    int value;
    int peso;
    Node_vertice* next;
    Arista* down;
};

//funcion para crear un nodo Vertice en el grafo
Node_vertice* crearNodo(int valor) {
    Node_vertice* nuevo = new Node_vertice;
    nuevo->value = valor;
    nuevo->next = NULL;
    nuevo->down = NULL;
    return nuevo;
}

//funcion para buscar un nodo vertice en el grafo
Node_vertice* buscarNodo(Node_vertice* cabeza, int valor) {
    Node_vertice* actual = cabeza;
    while (actual != NULL) {
        if (actual->value == valor) return actual;
        actual = actual->next;
    }
    return NULL; 
}

//funcion para agregar una arista entre vertices
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

//funcion para leer el archivo .txt el cual contiene la estructura del grafo
int total_nodos = 0;
Node_vertice* leerArchivo(){
     // Intenta diferentes rutas para el archivo
    ifstream archivo("estructura.txt");
    if (!archivo.is_open()) {
        archivo.open("./estructura.txt");
    }
    if (!archivo.is_open()) {
        archivo.open("/Users/enr.g/Documents/UCAB/Taller_2/taller-2/estructura.txt");
    }
    
    if (!archivo.is_open()) {
        colorP(12, "Error al abrir el archivo estructura.txt\n");
        colorP(14, "Asegúrate de que el archivo esté en el directorio del programa\n");
        return NULL;
    }

    archivo >> total_nodos;

    Node_vertice* cabeza = NULL;//inicializa la cabeza en NULL
    int origen, destino, peso;

    while (archivo >> origen >> destino >> peso) {  //para cada linea del archivo que va recorriendo, va reconociendo esos valores en orden
        agregarArista(&cabeza, origen, destino, peso);//va armando el grafo
    }

    archivo.close();//cierra el archivo
    colorP(2, "Archivo leído correctamente\n");
    return cabeza;
}


void imprimirGrafo(Node_vertice* cabeza){
    Node_vertice* actualNodo = cabeza; 
    colorP(6, "\n=== GRAFO IMPLEMENTADO CON LISTA DE ADYACENCIA ===\n"); 
    colorP(7, "(Vértice conectado, Peso)\n");
    cout << endl;

    while (actualNodo != NULL) { //imprime el valor del nodo en el que esta
        colorP(3, "Nodo ");
        colorP(1, to_string(actualNodo->value));
        colorP(7, " -> "); 

        Arista* actualArista = actualNodo->down; //actualArista es un puntero hacia las adyacencias del Vertice
        while (actualArista != NULL) {  //imprime el destino hacia el que se dirige la arista, y el peso de la misma
            colorP(10, " ( ");
            colorP(4, to_string(actualArista->destino));
            colorP(10, " , ");
            colorP(14, to_string(actualArista->peso));
            colorP(10, " ) ");
            actualArista = actualArista->next; //cambia a la siguinte arista para ser procesada
        }
        cout << endl;
        actualNodo = actualNodo->next; //cambia al siguiente vertice para ser procesado
    }
}

//funcion para liberar la memoria reservada para cada Nodo del grafo
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

//declaramos una variable como maximo
int MAX=9999999;
void primAMC(Node_vertice* grafo) {
    if (grafo == NULL) { //validamos
        colorP(12, "El grafo está vacío\n");
        return;
    }

    if (total_nodos == 0) {
        colorP(12, "No hay vértices en el grafo\n");
        return;
    }

    bool enAMC[total_nodos];  //para marcar el nodo como "procesado"
    for (int i = 0; i < total_nodos; i++) {
        enAMC[i] = false;  //inicializa el arreglo con las marcas de cada vertice en falso
    }
    int pesoMinimo[total_nodos]; //arreglo para almacenar el peso minimo para conectar a los nodos
    int verticePadre[total_nodos]; //guarda el padre de cada nodo

    for (int i = 0; i < total_nodos; i++) { //guardandoc ada peso minimo
        pesoMinimo[i] = MAX;  
    }
    pesoMinimo[0] = 0;
    verticePadre[0] = -1; 

    for (int count = 0; count < total_nodos - 1; count++) { //
        int minPeso=MAX; //guarda el valor minimo encontrado en esa iteracion
        int u = -1; //indice del nodo agregado al AMC
        
        for (int v = 0; v < total_nodos; v++) {
            if (!enAMC[v] && pesoMinimo[v] < minPeso) { //verifica que el nodo "v" no este todavia en el AMC y busca el vertice con el menor peso
                minPeso = pesoMinimo[v]; //si encuentra uno con mejor peso, se actualiza a ese
                u = v;
            }
        }

        if (u == -1) { //significa que no hay ningún nodo accesible desde los ya incluidos
            colorP(12, "El grafo no es conexo, no se puede completar el AMC.\n");

            return;
        }

        enAMC[u] = true; //marca el vertice "u" como procesado

        Node_vertice* verticeU = buscarNodo(grafo, u); //agrega a la lista de adyacencia
        Arista* arista = verticeU->down;
        
        while (arista != NULL) {
            int v = arista->destino; 
            int peso = arista->peso;

            if (!enAMC[v] && peso < pesoMinimo[v]) { //si "v" no esta en el AMC y el peso es menor al peso minimo ya guardado, entonces el padre y y el peso se actualizan
                verticePadre[v] = u;
                pesoMinimo[v] = peso;
            }
            arista = arista->next;
        }
    }

    colorP(5, "\n=== ALGORITMO DE PRIM ===\n");
    colorP(6, "Árbol Mínimo Cobertor (AMC):\n");
    colorP(7, "Arista (Rama) \tPeso");
    cout << "\n";
    
    //impresion del AMC
    int pesoTotal = 0;
    for (int i = 1; i < total_nodos; i++) {
        colorP(10, "  ( ");
        colorP(4, to_string(verticePadre[i]));
        colorP(7, " - ");
        colorP(4, to_string(i));
        colorP(10, " ) ");
        colorP(14, to_string(pesoMinimo[i]));
        cout << "\n";
        pesoTotal += pesoMinimo[i];
    }
    
    colorP(2, "\nPeso total del Árbol Mínimo Cobertor: ");
    colorP(1, to_string(pesoTotal));
}

// Función para imprimir el camino más corto desde el vertice indicado hasta el vertice destinatario
void imprimirCamino(int destino, int verticePadre[], int origen) {
    if (destino == origen) {
        colorP(4, to_string(origen));
        return;
    }
    if (verticePadre[destino] == -1) {
        colorP(12, "No hay camino\n");
        return;
    }
    imprimirCamino(verticePadre[destino], verticePadre, origen);
    colorP(7, " -> ");
    colorP(4, to_string(destino));
    
}

// Algoritmo de Dijkstra para encontrar el camino mas corto desde un vertice origen
void djikstra (Node_vertice* grafo, int verticeOrigen){
    if (grafo == NULL) {
        colorP(12, "El grafo está vacío");
        return;
    }

    if (total_nodos == 0) {
        colorP(12, "No hay vértices en el grafo");
        return;
    }

    bool procesado[total_nodos]; // Arreglo de booleanos para validar que ha sido procesado/marcado
    for (int i = 0; i < total_nodos; i++) {
        procesado[i] = false;  // Inicializa todas las posiciones del arreglo como falso
    }

    int distancia[total_nodos]; // Arreglo que guarda las distancias mínimas con respecto al origen
    int verticePadre[total_nodos]; // Guarda el vertice padre para reconstruir el camino

    // Inicializar distancias como el maximo (9999999) y padres como -1
    for (int i = 0; i < total_nodos; i++) {
        distancia[i] = MAX;  // Inicializa todas las distancias como el "maximo"
        verticePadre[i] = -1;
    }
    
    distancia[verticeOrigen] = 0; // La distancia entre el origen y si mismo, es 0

    // Procesar todos los vértices
    for (int count = 0; count < total_nodos; count++) {
        // Encontrar el vértice no procesado con la distancia mínima
        int minDistancia = MAX;
        int u = -1;
        
        // Busca el vértice no procesado con la menor distancia
        for (int v = 0; v < total_nodos; v++) {
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
    colorP(6, "\n=== CAMINOS MÁS CORTOS ===");
    colorP(7, "\nPartiendo desde el vértice ");
    colorP(4, to_string(verticeOrigen));
    cout << endl;
    colorP(7, "\nDestino\tDistancia\tCamino");
    cout << endl;
    
    for (int i = 0; i < total_nodos; i++) {
        colorP(4, to_string(i));
        colorP(7, "\t");
        if (distancia[i] == MAX) {
            colorP(12, "\t\tNo se puede acceder.\n");
        } else {
            colorP(14, to_string(distancia[i]));
            colorP(7, "\t\t");
            imprimirCamino(i, verticePadre, verticeOrigen);
        }
        cout << endl;
    }

}


int obtenerIndice(Node_vertice* grafo, int vertice) {
    int indice = 0;
    Node_vertice* actual = grafo;
    while (actual != NULL) {
        if (actual->value == vertice) { //busca el indice hasta que coincida
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
        if (idxVecino != -1 && colores[idxVecino] == color) { //si el indice es valido y es igual que el del vecino, entonces no esta disponible el color
            return false;
        }
        arista = arista->next;
    }
    return true;
}

void colorearGrafo(Node_vertice* grafo, int total_nodos) {
int* colores = new int[total_nodos];
const char* nombresColores[] = { //arreglo con los nombres de varios colores
    "Rojo", "Verde", "Azul", "Amarillo", "Naranja",
    "Morado", "Cian", "Rosa", "Gris", "Marrón"
};

int codigosColores[] = { //codigo de cada color con respeto a la libreria "color.h"
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
    for (int i = 0; i < total_nodos; i++) {
        colores[i] = -1;
    }

    Node_vertice* actual = grafo;
    while (actual != NULL) {
        int indice = obtenerIndice(grafo, actual->value);

        // Probar colores desde 0 hasta total_nodos - 1
        for (int c = 0; c < total_nodos; c++) {
            if (colorDisponible(actual, colores, c, grafo)) {
                colores[indice] = c;
                break;
            }
        }

        actual = actual->next;
    }

    // Mostrar grafo coloreado
    cout << "\nColoración del grafo:\n";
    actual = grafo;
    while (actual != NULL) {
        int indice = obtenerIndice(grafo, actual->value);
        int color = colores[indice];
        cout << "Vértice " << actual->value << " -> ";
        colorP(codigosColores[color], nombresColores[color]);
        cout << endl;
        actual = actual -> next;
}

    delete[] colores;
}

int main(){
   int verticeInicio, op;
    Node_vertice* grafo = leerArchivo();  
        if (grafo != NULL) {
            // Título principal con colores
        colorP(11, "╔════════════════════════════════════════════════════════════════╗\n");
        colorP(11, "║            TALLER #2 - TEORÍA DE GRAFOS                        ║\n");
        colorP(11, "║        Realizado por: Enrique Godoy y Jose Quintero            ║\n");
        colorP(11, "╚════════════════════════════════════════════════════════════════╝\n");

        while (op != 5){
            cout << endl << endl;
            colorP(6, " ┌─────────────── MENÚ PRINCIPAL ───────────────┐\n");
            colorP(7, "│ 1. Imprimir Grafo                            │\n");
            colorP(7, "│ 2. Ver Árbol Mínimo Cobertor (Prim)          │\n");
            colorP(7, "│ 3. Camino de Costo Mínimo (Dijkstra)         │\n");
            colorP(7, "│ 4. Coloración del Grafo (Algoritmo Boraz)    │\n");
            colorP(7, "│ 5. Cerrar Programa                           │\n");
            colorP(6, "└──────────────────────────────────────────────┘\n");
            colorP(14, "\nIndique el número de operación que desea realizar: ");
            cin >> op;

        switch (op){
            case 1:
                imprimirGrafo(grafo);
                break;

            case 2:
                primAMC(grafo);
                break;

            case 3:
                colorP (5, "\n=== ALGORITMO DE DIJKSTRA ===");
                    do{
                    colorP(7, "\nIndique el vértice desde donde desea conocer el camino de menor costo\n");
                    colorP(6, "\nVértices disponibles: ( 0 - ");
                    colorP(6, to_string(total_nodos-1));
                    colorP(6, "): ");
                    cin >> verticeInicio;

                    if (verticeInicio < 0 || verticeInicio > total_nodos){ //validacion hasta que el numero sea introducido de manera correcta
                        colorP(12, "\nERROR: El vértice debe estar entre el rango indicado\n");
                            colorP(14, "Rango válido: ( 0 - ");
                            colorP(14, to_string(total_nodos-1));
                            colorP(14, " )");
                            colorP(14, "Inténtelo nuevamente...\n");
                    }
                    } while (verticeInicio < 0 || verticeInicio > total_nodos);
                    djikstra(grafo, verticeInicio);
                break;

            case 4:
                colorP(5, "\n=== ALGORITMO BORAZ DE COLORACIÓN ===\n");
                colorearGrafo(grafo, total_nodos);
                break;

            case 5:
                colorP(14, "\nLiberando memoria...\n");
                liberarGrafo(grafo);
                colorP(2, "Memoria liberada correctamente\n");
                colorP(6, "¡Gracias por usar el programa!\n");
                colorP(7, "Cerrando...\n");
                break;

            default:
                colorP(12, "Opción no válida. Por favor, seleccione una opción del 1 al 5.\n");
                break;
            }
        }  
        } else {
        colorP(12, "No se pudo cargar el grafo. Verifique el archivo 'estructura.txt'\n");
        }
    return 0;
    }