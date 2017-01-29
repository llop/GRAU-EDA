#include <iostream>
#include <bitset>
#include <vector>
#include <queue>
#include <map>
using namespace std;

typedef vector<char> CV;
typedef vector<double> DV;
typedef map<char, CV> CVM;


// arbol para representar el codigo de Huffman
struct HuffmanTree {
  // variables
  char symbol;       // el simbolo (o letra) -solo es necesario para las hojas
  double prob;       // probabilidad (o peso)
  HuffmanTree *lef;  // puntero al subarbol izquierdo
  HuffmanTree *rig;  // puntero al subarbol derecho
  // constructora
  HuffmanTree(char symbol, double prob, HuffmanTree *lef, HuffmanTree *rig) :
    symbol(symbol), prob(prob), lef(lef), rig(rig) {}
  // destructora
  ~HuffmanTree() {
    del(lef);
    del(rig);
  }
  void del(HuffmanTree *t) {
    if (t != NULL) {
      del(t->lef);
      del(t->rig);
      delete t;
    }
  }
};
// comparador necesario para la priority_queue
struct HuffmanTreeComparator {
  bool operator()(const HuffmanTree* a, const HuffmanTree* b) {
    if (a->prob == b->prob) return a->symbol > b->symbol;
    return a->prob > b->prob;
  }
};
typedef priority_queue<HuffmanTree*, vector<HuffmanTree*>, HuffmanTreeComparator> HTQ;


// dados los simbolos del lenguaje, y sus probabilidades,
// devuelve el arbol del codigo Huffman correspondiente
HuffmanTree* build_huffman_tree(const CV &symbols, const DV &probs) {
  // crear la cola y anadir los caracteres con sus probabilidades
  HTQ que;
  for (int i = 0; i < (int)symbols.size(); ++i) {
    if (probs[i] > 0.0) {  // descartar simbolos con probabilidad 0 (o menor)
      HuffmanTree* leaf = new HuffmanTree(symbols[i], probs[i], NULL, NULL);
      que.push(leaf);
    }
  }
  if (que.empty()) return NULL;
  // construir el arbol: mientras quede mas de un arbol en la cola, 
  // unir los 2 primeros arboles en uno nuevo, y encolarlo
  while ((int)que.size() > 1) {
    HuffmanTree* rig = que.top();
    que.pop();
    HuffmanTree* lef = que.top();
    que.pop();
    HuffmanTree* node = new HuffmanTree(-1, lef->prob + rig->prob, lef, rig);
    que.push(node);
  }
  // el arbol que queda es la raiz
  return que.top();
}


// construye el map que relaciona los simbolos con su codigo huffman
void build_code_map(HuffmanTree* t, CV &code, CVM &code_map) {
  if (t->lef == NULL && t->rig == NULL) {
    // hoja: asignar el codigo
    code_map[t->symbol] = code;
  } else {
    // nodo interno: seguir construyendo el codigo
    if (t->lef != NULL) {
      code.push_back('0');
      build_code_map(t->lef, code, code_map);
      code.pop_back();
    }
    if (t->rig != NULL) {
      code.push_back('1');
      build_code_map(t->rig, code, code_map);
      code.pop_back();
    }
  }
}


// imprimir l'arbre en preordre
void print_huffman_tree(HuffmanTree *t) {
  if (t->symbol == -1) cout << "-";
  else cout << t->symbol;
  cout << " (" << t->prob << "), ";
  if (t->lef != NULL) print_huffman_tree(t->lef);
  else cout << "L, ";
  if (t->rig != NULL) print_huffman_tree(t->rig);
  else cout << "R, ";
}
// imprimir tabla de codigos
void print_code_map(const CVM &code_map) {
  for (auto &e : code_map) {
    cout << e.first << " -> ";
    for (char c : e.second) cout << c;
    cout << endl;
  }
}

int main() {
  cout << (1.0/2.0 + 2.0/4.0 + 3.0/8.0 + 8.0/16.0) << endl;
  
  // esta funcion acelera el cin/cout
  ios_base::sync_with_stdio(0);
  cin.tie(NULL);
  
  // parametros
  CV symbols = { 'a', 'b', 'c', 'd', 'e' };                      // { 'a', 'b', 'c', 'd' };
  DV probs = { 1.0/2.0, 1.0/4.0, 1.0/8.0, 1.0/16.0, 1.0/16.0 };  // { 70, 10, 15, 5 };
  CVM code_map;
  // crear arbol e imprimirlo
  HuffmanTree* root = build_huffman_tree(symbols, probs);
  print_huffman_tree(root);
  cout << endl;
  // construir tabla de codigos e imprimirla
  CV code;
  build_code_map(root, code, code_map);
  print_code_map(code_map);
  
  return 0;
}

