#include <iostream>
#include <vector>
#include <list>
using namespace std;

typedef pair<int, int> IP;
typedef list<IP> IPL;
typedef IPL::iterator IPLI;
typedef vector<IPL> IPM;

// Separate Chaining Hash Table
// Implementacion de una tabla de hash de encadenamiento separado
// Viene a ser lo mismo que el unordered_map<int, int> de la STL
struct HashTable {
  
  int cnt;  // numero de elementos de la tabla
  IPM tbl;  // tabla de entradas
  
  // Capacidad inicial de 8
  // Factor de carga: 2
  HashTable() {
    cnt = 0;
    tbl = IPM(8);
  }
  
  // Simple funcion de hash: el propio valor de 'key' modulo 'mod'
  inline int hash(int key, int mod) {
    return key % mod;
  }
  
  // Redimensiona la tabla de entradas creando una nueva tabla el doble de grande,
  // e insertando ahi todas las entradas existentes
  // Complejidad: O(n)
  void resize() {
    IPM tbl_tmp = IPM(tbl.size() * 2);
    for (int i = 0; i < (int)tbl.size(); ++i) {
      for (IPLI it = tbl[i].begin(); it != tbl[i].end(); ++it) {
	const IP &key_val = *it;
	int pos = hash(key_val.first, tbl_tmp.size());
	tbl_tmp[pos].insert(tbl_tmp[pos].end(), key_val);
      }
    }
    tbl = tbl_tmp;
  }
  
  // Inserta una entrada <clave, valor> en la tabla
  // Coste amortizado: O(1)
  // En el peor de los casos, todas las entradas irian a la misma lista: O(n)
  void insert(int key, int val) {
    // Encontrar la posicion
    int pos = hash(key, tbl.size());
    // Sustituir el valor si existe una entrada con esa 'key'
    for (IPLI it = tbl[pos].begin(); it != tbl[pos].end(); ++it) {  
      if (it->first == key) {
	it->second = val;
	return;
      }
    }
    // Si no la 'key' es nueva, insertarla en la siguiente posicion libre
    tbl[pos].insert(tbl[pos].end(), IP(key, val));
    ++cnt;
    // Si se ha superado el factor de carga, redimensionar
    if (cnt >= 2 * (int)tbl.size()) resize();
  }
  
  // Eliminar la entrada cuya clave es 'key'
  // Coste amortizado: O(1)
  // Aunque si todas las entradas estan en la misma posicion, O(n)
  void erase(int key) {
    // Encontrar la posicion en la que se encontraria la entrada
    int pos = hash(key, tbl.size());
    for (IPLI it = tbl[pos].begin(); it != tbl[pos].end(); ++it) {
      if (it->first == key) {
	// clave encontrada -eliminarla
	tbl[pos].erase(it);
	return;
      }
    }
  }
  
  // Retorna el valor asociado a la clave 'key'
  // Si ningun valor ha sido asociado a esa clave, devuelve -(2^31)
  // Misma complejidad que 'insert' y 'erase'
  int get_value(int key) {
    int pos = hash(key, tbl.size());
    for (IPLI it = tbl[pos].begin(); it != tbl[pos].end(); ++it) {  
      if (it->first == key) {
	return it->second;
      }
    }
    return 1 << 31;
  }
  
};

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  HashTable hash_table;
  cout << hash_table.get_value(0) << endl;
  hash_table.insert(0, 1);
  hash_table.insert(8, 2);
  hash_table.insert(16, 3);
  hash_table.insert(24, 4);
  hash_table.insert(32, 5);
  cout << hash_table.get_value(0) << endl;
  hash_table.insert(0, 6);
  cout << hash_table.get_value(0) << endl;
  hash_table.insert(66, 7);
  hash_table.insert(77, 8);
  hash_table.insert(88, 9);
  hash_table.insert(99, 10);
  return 0;
}

