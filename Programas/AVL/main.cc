#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> IV;

// nodo del arbol
struct Node {
  int val;    // valor
  int hei;    // altura
  Node* lef;  // subarbol izquierdo
  Node* rig;  // subarbol derecho
};

// implementacion del AVL
// permite elementos repetidos (como el 'multiset' de la stl)
struct AVL {
  
  // nodo raiz del arbol
  Node* root;
  
  // retorna la altura de 'nod'
  inline int height(Node* nod) {
    return nod == NULL ? 0 : nod->hei;
  }
  
  // balance del arbol con raiz en 'nod':
  // sera negativo si el subarbol derecho es mas alto que el izquierdo
  // positivo si el subarbol izquierdo es el mas alto
  // o cero si son iguales
  inline int balance(Node* nod) {
    return nod == NULL ? 0 : height(nod->lef) - height(nod->rig);
  }
  
  // actualiza la altura de 'nod'
  inline void update_height(Node* nod) {
    int hl = height(nod->lef);
    int hr = height(nod->rig);
    nod->hei = (hl > hr ? hl : hr) + 1;
  }
  
  // rotacion derecha sobre 'x'
  inline Node* rotate_right(Node* x) {
    Node* y = x->lef;
    Node* t2 = y->rig;
    y->rig = x;
    x->lef = t2;
    update_height(x);
    update_height(y);
    return y;
  }
  
  // rotacion izquierda sobre 'x'
  inline Node* rotate_left(Node* x) {
    Node* y = x->rig;
    Node* t2 = y->lef;
    y->lef = x;
    x->rig = t2;
    update_height(x);
    update_height(y);
    return y;
  }
  
  // rotacion doble a la derecha
  inline Node* double_rotate_right(Node* nod) {
    nod->lef = rotate_left(nod->lef);
    return rotate_right(nod);
  }
  
  // rotacion doble a la izquierda
  inline Node* double_rotate_left(Node* nod) {
    nod->rig = rotate_right(nod->rig);
    return rotate_left(nod);
  }
  
  // rotaciones: equilibran el arbol despues de una insercion o borrado
  // al acabar, es necesario recalcular las alturas de los nodos tratados
  // complejidad: O(1)
  inline Node* update_balance(Node* nod) {
    
    // si el arbol ha quedado desequilibrado hay que hacer algun tipo de rotacion
    int bal = balance(nod);
    if (bal > 1) {
      // el subarbol izquierdo es mas alto que el derecho
      // hay que mirar su balance
      int bal_lef = balance(nod->lef);
      if (bal_lef >= 0) {
	// el sub-subarbol izquierdo es tan o mas alto: rotacion simple derecha
	return rotate_right(nod);
      } else {
	// el hijo derecho del subarbol izquierdo de 'nod' es mas alto
	// rotacion doble derecha
	return double_rotate_right(nod);
      }
    } else if (bal < -1) {
      // el subarbol derecho es mas alto que el izquierdo
      // hay que mirar su balance
      int bal_rig = balance(nod->rig);
      if (bal_rig <= 0) {
	// el sub-subarbol derecho es tan o mas alto: rotacion simple izquierda
	return rotate_left(nod);
      } else {
	// el hijo izquierdo del subarbol derecho de 'nod' es mas alto
	// rotacion doble izquierda
	return double_rotate_left(nod);
      }
    }
    
    // no hubo rotacion, devolver el nodo tal cual
    return nod;
  }
  
  // insercion: anadir 'val' en el (sub)arbol 'nod'
  // el re-balanceado se produce como mucho 1 vez
  // complejidad: O(log n)
  Node* insert(Node* nod, int val) {
    
    // nodo vacio, podemos insertar el valor aqui
    if (nod == NULL) return new Node({ val, 1, NULL, NULL });
    
    // si el valor a insertar es menor que el de 'nod'
    // insertar en el subarbol izquierdo
    // si es mayor o igual, ir al subarbol derecho
    // al acabar, recalcular la altura de 'nod'
    if (val < nod->val) nod->lef = insert(nod->lef, val);
    else nod->rig = insert(nod->rig, val);
    update_height(nod);
    
    // equilibrar el arbol si es necesario
    return update_balance(nod);
  }
  void insert(int val) {
    root = insert(root, val);
  }
  
  // devuelve el nodo de mayor valor del arbol con raiz en 'nod'
  inline Node* max_node(Node* nod) {
    while (nod->rig != NULL) {
      nod = nod->rig;
    }
    return nod;
  }
  
  // borrado: eliminar 'val' del (sub)arbol 'nod'
  // el re-balanceado puede producirse en todos los padres del nodo borrado
  // complejidad: O(log n)
  Node* erase(Node* nod, int val) {
    
    // si el nodo esta vacio, no hay nada que borrar
    if (nod == NULL) return NULL;
    
    // si el valor a borrar es menor que el de 'nod' buscar en el subarbol izquierdo
    // si es mayor, ir al subarbol derecho
    if (val < nod->val) nod->lef = erase(nod->lef, val);
    else if (val > nod->val) nod->rig = erase(nod->rig, val);
    else {
      // el valor del nodo es el que queremos borrar
      // hay 3 posibilidades: 
      if (nod->lef == NULL && nod->rig == NULL) {
	// 'nod' es una hoja -ponerlo a NULL, liberar memoria y retornar
	Node* leaf = nod;
	nod = NULL;
	delete leaf;
	return NULL;
      } else if (nod->lef == NULL || nod->rig == NULL) {
	// 'nod' es el padre de una hoja
	// sustituir 'nod' por su hijo, eliminar hoja y retornar
	Node* leaf = nod->lef == NULL ? nod->rig : nod->lef;
	*nod = *leaf;
	delete leaf;
	return nod;
      } else {
	// sustituir 'nod' por el nodo de mayor valor de su subarbol izquierdo
	// y eliminar ese nodo de mayor valor
	Node* leaf = max_node(nod->lef);
	nod->val = leaf->val;
	nod->lef = erase(nod->lef, leaf->val);
      }
    }
    
    // actualizar altura, y a lo mejor equilibrar el arbol
    update_height(nod);
    return update_balance(nod);
  }
  void erase(int val) {
    root = erase(root, val);
  }
  
  // consulta: esta 'val' presente en el (sub)arbol 'nod'?
  // complejidad: O(log n)
  Node* find(Node* nod, int val) {
    // nodo vacio, el valor buscado no esta en el arbol
    if (nod == NULL) return NULL;
    
    // si el valor buscado es menor que el de 'nod'
    // buscar en el subarbol izquierdo
    // si es mayor, ir al subarbol derecho
    // si es el mismo, lo hemos encontrado :)
    if (val < nod->val) return find(nod->lef, val);
    else if (val > nod->val) return find(nod->rig, val);
    return nod;
  }
  bool find(int val) {
    return find(root, val) == NULL ? 0 : 1;
  }
  
  // imprimir los valores del arbol, en pre-orden
  // complejidad: O(n)
  void print_preorder(Node* nod) {
    if (nod != NULL) {
      cout << nod->val << ' ';
      print_preorder(nod->lef);
      print_preorder(nod->rig);
    } else {
      cout << "* ";
    }
  }
  void print_preorder() {
    print_preorder(root);
    cout << endl;
  }
  
  // imprimir los valores del arbol, en in-orden
  // complejidad: O(n)
  void print_inorder(Node* nod) {
    if (nod != NULL) {
      print_inorder(nod->lef);
      cout << nod->val << ' ';
      print_inorder(nod->rig);
    } else {
      cout << "* ";
    }
  }
  void print_inorder() {
    print_inorder(root);
    cout << endl;
  }
  
  // imprimir los valores del arbol, en post-orden
  // complejidad: O(n)
  void print_postorder(Node* nod) {
    if (nod != NULL) {
      print_postorder(nod->lef);
      print_postorder(nod->rig);
      cout << nod->val << ' ';
    } else {
      cout << "* ";
    }
  }
  void print_postorder() {
    print_postorder(root);
    cout << endl;
  }
  
  Node* build_preorder(const IV &vals, int start, int end) {
    if (start > end) return NULL;
    int i = start + 1;
    while (i <= end && vals[i] < vals[start]) ++i;
    Node* nod = new Node({ vals[start], 1, NULL, NULL });
    nod->lef = build_preorder(vals, start + 1, i - 1);
    nod->rig = build_preorder(vals, i, end);
    update_height(nod);
    return nod;
  }
  
  Node* build_preorder2(const IV &vals, Node* &nod, int mx, int &pos, int len) {
    nod = new Node({ vals[pos], 0, NULL, NULL });
    ++pos;
    if (pos < len && vals[pos] < nod->val) {
      build_preorder2(vals, nod->lef, nod->val, pos, len);
    }
    if (pos < len && (mx == -1 || mx > vals[pos])) {
      build_preorder2(vals, nod->rig, mx, pos, len);
    }
    update_height(nod);
    return nod;
  }
  
  Node* build_sorted(const IV &vals, int start, int end) {
    if (start > end) return NULL;
    int m = (start + end) / 2;
    Node* nod = new Node({ vals[m], 1, NULL, NULL });
    nod->lef = build_sorted(vals, start, m - 1);
    nod->rig = build_sorted(vals, m + 1, end);
    update_height(nod);
    return nod;
  }
  
  // call this on 
  void init(const IV &preorder) {
    int n = preorder.size();
    //root = build_preorder(preorder, 0, n - 1);
    int i = 0;
    build_preorder2(preorder, root, -1, i, n);
    //root = build_sorted(preorder, 0, n - 1);
  }
  
};

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  AVL avl;
  // probando los insert
  /*
  avl.insert(50);
  avl.insert(17);
  avl.insert(12);
  avl.insert(9);
  avl.insert(14);
  avl.insert(23);
  avl.insert(19);
  avl.insert(72);
  avl.insert(54);  // doble rotacion izquierda
  avl.insert(7);
  avl.insert(8);  // doble rotacion derecha
  avl.insert(67);
  avl.insert(76);
  avl.insert(15);
  avl.insert(51);
  avl.insert(52);
  // probando los erase
  avl.erase(14);
  avl.erase(7);
  avl.erase(15); // doble rotacion derecha, doble izquierda
  // a ver las consultas?
  cout << avl.find(8) << endl;
  cout << avl.find(15) << endl;
  cout << avl.find(10000) << endl;
  cout << avl.find(76) << endl;
  // prints
  cout << "preorder: ";
  avl.print_preorder();
  //cout << "inorder: ";
  //avl.print_inorder();
  //cout << "postorder: ";
  //avl.print_postorder();
  */
  
  //IV vals{ 2, 1, 7, 3, 8 };
  //IV vals{ 23, 17, 9, 8, 12, 19, 54, 51, 50, 52, 72, 67, 76 };
  //IV vals{ 1, 2, 3, 7, 8 };
  IV vals{ 3, 1, 2, 7, 8 };
  avl.init(vals);
  cout << "preorder: ";
  avl.print_preorder();
  
  return 0;
}

