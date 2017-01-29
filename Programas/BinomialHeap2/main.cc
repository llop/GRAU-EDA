#include <iostream>
#include <vector>
using namespace std;

struct Node {
  int val;            // valor del nodo
  int ord;            // orden arbol binomial
  vector<Node*> chi;  // vector de subarboles:
                      // debe haber una correspondencia directa entre
                      // el orden del subarbol y su posicion en 'chi', es decir,
                      // en la posicion k solo puede haber un arbol de orden k
                      
  // Limite: permitir arboles binomiales hasta orden 31 (2^31 = 2147483648 nodos)
  Node(int val) : val(val), ord(0), chi(vector<Node*>(32, NULL)) {}
};

typedef vector<Node*> NV;

// Implementacion simple de un max-heap binomial para 'int's
struct MaxHeap {
  
  int ord;    // orden del arbol binomial mas grande
  Node* max;  // puntero al arbol con mayor valor
  NV trees;   // vector de arboles del heap; igual que en Node,
	      // en la posicion k solo puede haber un arbol de orden k (o NULL)
  
  // Limitar cantidad de valores igual que Node (maximo 2^31 = 2147483648 valores)
  MaxHeap() : ord(-1), max(NULL), trees(NV(32, NULL)) {}
  
  // Devuelve true si el heap esta vacio
  bool empty() {
    return ord < 0;
  }
  
  // Actualizar el puntero al arbol con mayor valor
  // y el orden del arbol mas grande
  inline void update_max(Node* nod) {
    if (nod != NULL) {
      if (max == NULL || max->val < nod->val) max = nod;
      if (ord < nod->ord) ord = nod->ord;
    }
  }
  
  // Fusion de dos arboles t1 y t2 no nulos del mismo orden
  // El arbol con mayor valor en la raiz sera el padre,
  // y el ortro arbol se anadira al vector de subarboles
  // El arbol padre incrementa en 1 su orden
  // ya que la fusion de dos arboles de orden k resulta en uno de orden (k + 1)
  inline Node* merge_trees(Node* &t1, Node* &t2) {
    if (t1->val > t2->val) {
      t1->chi[t2->ord] = t2;
      ++t1->ord;
      return t1;
    } else {
      t2->chi[t1->ord] = t1;
      ++t2->ord;
      return t2;
    }
  }
  
  // Fusion de arboles 't1' y 't2' del mismo orden
  // 'car' es un arbol del mismo orden que 't1' y 't2'
  // Operacion analoga a una suma de numeros en base 2:
  //  - si 't1' o 't2' son NULL, su valor respectivo seria 0; si no, 1
  //  - 'car' representa el carry: valdria 0 o 1 segun si es NULL o no
  inline Node* merge_trees(Node* &t1, Node* &t2, Node* &car) {
    Node* res = NULL;
    if (t1 != NULL && t2 != NULL) {
      // 1 + 1
      // el resultado sera 0 o 1 en funcion de si hay carry o no
      // y el nuevo carry sera siempre 1 ('t1' y 't2' fusionados)
      res = car;
      car = merge_trees(t1, t2);
    } else if (t1 != NULL) {
      // 1 + 0
      // el resultado sera 1 si no hay carry (devolver 't1'), o 0 si lo habia
      // si habia carry, hay que fusionar 'car' y 't1' para obtener el nuevo carry
      res = car == NULL ? t1 : NULL;
      car = car == NULL ? NULL : merge_trees(t1, car);
    } else if (t2 != NULL) {
      // 0 + 1
      // analoga a la suma del anterior if, pero tratando 't2' en vez de 't1'
      res = car == NULL ? t2 : NULL;
      car = car == NULL ? NULL : merge_trees(t2, car);
    } else {
      // 0 + 0
      // el resultado es igual al carry, ya que 't1' y 't2' son NULL
      res = car;
      car = NULL;
    }
    return res;
  }
  
  // Funcion de fusion del conjunto de arboles de este heap
  // con el conjunto de arboles en 'nodes'
  // 'ord_nod' debe ser el orden del mayor arbol de 'nodes'
  void merge(NV &nodes, int ord_nod) {
    // trataremos los arboles de orden 0 hasta 'max_ord', teniendo en cuenta que
    // la operacion con los arboles de mayor orden puede producir carry
    int max_ord = (ord > ord_nod ? ord : ord_nod) + 1;
    if (max_ord > 31) max_ord = 31;  // respetar limites estructura de datos
    max = NULL;  // resetear 'max' y 'ord' -'update_max' lo actualizara
    ord = -1;
    
    Node* car = NULL;  // carry inicial = 0
    for (int i = 0; i <= max_ord; ++i) {
      // iterar desde 0 hasta el orden maximo,
      // fusionando los arboles binomiales del mismo orden
      trees[i] = merge_trees(trees[i], nodes[i], car);
      // actualizamos tambien puntero al arbol con valor maximo, y orden del heap
      update_max(trees[i]);
    }
  }
  
  // Fusionar este heap con 'heap'
  // Complejidad: O(log n)
  void merge(MaxHeap &heap) {
    merge(heap.trees, heap.ord);
  }
  
  // Consultar el valor maximo en este heap
  // Si el heap esta vacio, devuelve -(2^31)
  // Complejidad: O(1)
  int top() {
    return max != NULL ? max->val : 1 << 31;
  }
  
  // Anadir el valor 'x' a este heap
  // Basicamente fusiona este heap con un arbol binomial de orden 0
  // y cuya raiz tiene valor 'x'
  // Aunque su complejidad sea en principio O(log n),
  // su coste amortizado (despues de muchas inserciones) es O(1)
  void push(int x) {
    NV nodes(32, NULL);
    nodes[0] = new Node(x);
    merge(nodes, 0);
  }
  
  // Eliminar del heap su maximo valor
  // El truco es:
  //  - quitar del heap el arbol que contiene el valor maximo,
  //  - considerar los subarboles de ese arbol como un segundo heap,
  //  - y finalmente fusionar ambos heaps
  // Complejidad: O(log n)
  void pop() {
    if (max != NULL) {
      int nod_ord = max->ord - 1;  // orden del mayor subarbol de 'max'
      Node* del = max;             // puntero a 'max' para luego hacer el delete
      NV &nods = max->chi;         // vector de arboles para la fusion (2º heap)
      trees[max->ord] = NULL;      // quitar del heap el arbol con maximo valor
      merge(nods, nod_ord);        // fusionar este heap con 2º heap
      delete del;                  // liberar memoria
    }
  }
  
};

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  cout << (int)((1LL << 31) - 1) << endl;
  
  MaxHeap heap;
  heap.push(1);
  heap.push(2);
  heap.push(3);
  heap.push(4);
  heap.push(5);
  heap.push(6);
  heap.push(10);
  heap.push(11);
  heap.push(8);
  heap.push(9);
  heap.push(7);
  
  while (!heap.empty()) {
    cout << heap.top() << endl;
    heap.pop();
  }
  cout << heap.top() << endl;
  heap.push(22);
  cout << heap.top() << endl;
  
  return 0;
}

