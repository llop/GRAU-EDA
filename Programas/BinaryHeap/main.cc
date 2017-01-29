#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> IV;

// Hundir el nodo 'pos' hasta que su padre tenga un valor mayor o igual
// Complejidad: O(log n)
void sink(IV &vec, int pos, int end) {
  while (pos * 2 + 1 < end) {
    int tmp = pos;
    int pos_lef = 2 * pos + 1;
    int pos_rig = 2 * pos + 2;
    if (pos_rig < end && vec[pos_rig] > vec[tmp]) tmp = pos_rig;
    if (vec[pos_lef] > vec[tmp]) tmp = pos_lef;
    if (tmp == pos) return;
    swap(vec[pos], vec[tmp]);
    pos = tmp;
  }
}

// Reorganiza los valores de 'vec'
// de forma que represente el arbol binario de un max-heap
// Complejidad: O(n)
void make_heap(IV &vec) {
  // encontrar la posicion del ultimo nodo que tiene algun hijo
  int pos = (vec.size() - 2) / 2;
  while (pos >= 0) {
    sink(vec, pos, vec.size());
    --pos;
  }
}

// Ordenacion por heap
// Consiste en transformar el vecor en un heap binario
// y sacar repetidamente el mayor valor, anadiendolo al final del vector
// El vector al final del proceso queda ordenado de menor a mayor
// Complejidad: O(n * log n)
void heap_sort(IV &vec) {
  // transforma vec en heap
  make_heap(vec);
  int pos = vec.size() - 1;
  while (pos > 0) {
    // en cada iteracion pone el mayor valor 
    // en la ultima posicion de la parte del vector que es heap
    swap(vec[0], vec[pos]);
    sink(vec, 0, pos);
    --pos;
  }
}

// Implementacion de un max-heap binario
struct MaxHeap {
  
  // Arbol binario representado con un vector
  vector<int> tree = { 0 }; 
  
  // posicion del nodo padre de 'pos' = pos / 2
  inline int parent(int pos) {
    return pos / 2;
  }
  
  // posicion del hijo izquierdo de 'pos' = 2 * pos
  inline int left(int pos) {
    return 2 * pos;
  }
  
  // posicion del hijo derecho de 'pos' = 2 * pos + 1
  inline int right(int pos) {
    return 2 * pos + 1;
  }
  
  // Devuelve true si el heap esta vacio
  bool empty() {
    return tree.size() <= 1;
  }
  
  // Asciende el valor en la posicion 'pos'
  // Mientras el valor del nodo padre de 'pos'
  // sea menor o igual que el valor del nodo 'pos'
  // intercambia sus valores
  inline void rise(int pos) {
    // mientras el nodo tenga padre
    while (pos / 2 > 0) {
      // posicion del padre
      int pos_par = parent(pos);
      // si el valor en pos no es mayor que el de su padre, hemos terminado
      if (tree[pos] <= tree[pos_par]) return;
      // si es mayor, intercambiar sus valores y repetir la operacion
      swap(tree[pos], tree[pos_par]);
      pos = pos_par;
    }
  }
  // Inserta un valor en el heap
  // Anade el valor en la siguiente posicion libre
  // y lo 'asciende' por el arbol hasta que se cumpla la propiedad de heap
  // Complejidad: O(log n)
  void push(int x) {
    tree.push_back(x);
    rise(tree.size() - 1);
  }
  
  // Hunde el nodo en la posicion 'pos'
  // Mientras el valor del nodo 'pos' sea menor que el de alguno de sus hijos,
  // intercambia su valor con el del hijo de mayor valor
  inline void sink(int pos) {
    // mientras el nodo tenga al menos 1 hijo
    while (pos * 2 < (int)tree.size()) {
      int tmp = pos;  // almacenara el nodo de mayor valor
      int pos_lef = left(pos);   // hijo izquierdo
      int pos_rig = right(pos);  // hijo derecho
      // comparar el valor de pos con el del hijo derecho
      if (pos_rig < (int)tree.size() && tree[pos_rig] > tree[tmp]) tmp = pos_rig;
      // y ahora con el del hijo izquierdo
      if (tree[pos_lef] > tree[tmp]) tmp = pos_lef;
      // tmp seguira siendo igual a 'pos' si el valor en 'pos' es el mayor
      // en este caso hemos terminado
      if (tmp == pos) return;
      // de lo contrario, intercambiar los valores y repetir la operacion
      swap(tree[pos], tree[tmp]);
      pos = tmp;
    }
  }
  // Elimina el nodo de mayor valor del heap
  // Consiste en intercambiar el valor en la raiz con el del ultimo nodo,
  // luego eliminar ese ultimo nodo,
  // y hundir el valor en la raiz hasta que el arbol cumpla la propiedad de heap
  // Complejidad: O(log n)
  void pop() {
    if (!empty()) {
      swap(tree[1], tree[tree.size() - 1]);
      tree.pop_back();
      sink(1);
    }
  }
  
  // Consulta el mayor valor del heap
  // Si el heap esta vacio, devuelve -(2^31)
  // Por definicion de heap, la raiz del arbol contiene ese maximo
  // Complejidad: O(1)
  int top() {
    return empty() ? 1 << 31 : tree[1];
  }
  
};

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  MaxHeap heap;
  heap.push(1);
  heap.push(2);
  heap.push(3);
  heap.push(4);
  heap.push(5);
  heap.push(6);
  heap.push(7);
  heap.push(8);
  while (!heap.empty()) {
    cout << heap.top() << ' ';
    heap.pop();
  }
  cout << endl;
  
  IV vec = { 3, 1, 4, 2, 7, 6, 8, 5 };
  heap_sort(vec);
  for (int i = 0; i < (int)vec.size(); ++i) {
    if (i) cout << ' ';
    cout << vec[i];
  }
  cout << endl;
  
  IV vec2 = { 2, 9, 7, 6, 5, 8, 1, 8, 6, 5, 3, 7, 4 };
  make_heap(vec2);
  for (int x : vec2) cout << x << ' ';
  cout << endl;
  
  return 0;
}

