#include <iostream>
#include <vector>
using namespace std;

typedef pair<int, int> IP;
typedef vector<IP*> IPV;

// Open Addressing Hash Table
// Implementacion de una tabla de hash de direccionamiento abierto
// Viene a ser lo mismo que el unordered_map<int, int> de la STL
struct HashTable {
  
  int cnt;  // numero de elementos
  IPV tbl;  // tabla de entradas
  
  // Crea la hash table, con capacidad inicial de 8
  // Factor de carga: 0.8
  HashTable() {
    cnt = 0;
    tbl = IPV(8, NULL);
  }
  
  // Simple funcion de hash: el propio valor de 'key' modulo 'mod'
  inline int hash(int key, int mod) {
    return key % mod;
  }
  
  // Encuentra la posicion de la tabla en la que se encuentra 'key'
  // O si la 'key' no esta en la tabla, encuentra la posicion (vacia)
  // en la que es posible insertar 'key'
  // En el caso peor habra que patearse la tabla entera O(n)
  // Igual el coste amortizado es O(1)
  inline int slot(int key, const IPV &table) {
    int pos = hash(key, table.size());
    while (table[pos] != NULL) {
      if (table[pos]->first == key) return pos;
      pos = (pos + 1) % (int)table.size();
    }
    return pos;
  }
  
  // Redimensiona la tabla
  // Crea una nueva tabla el doble de grande,
  // y anade los elementos de la tabla original uno a uno
  // Complejidad: O(n)
  void resize() {
    IPV tbl_tmp(2 * tbl.size(), NULL);
    for (int i = 0; i < (int)tbl.size(); ++i) {
      if (tbl[i] != NULL) {
	int pos = slot(tbl[i]->first, tbl_tmp);
	tbl_tmp[pos] = tbl[i];
      }
    }
    tbl = tbl_tmp;
  }
  
  // Inserta un par <clave, valor> en la tabla
  // En el peor de los casos, todas las entradas irian a la misma posicion
  // de la tabla -complejidad O(n)
  // En el mejor caso, cada entrada se mapearia en una posicion distinta
  // en cuyo caso la complejidad seria O(1)
  // Coste amortizado: O(1)
  void insert(int key, int val) {
    // Encontrar la posicion para la nueva entrada
    int pos = slot(key, tbl);
    if (tbl[pos] == NULL) {
      // La posicion esta ocupada -> la 'key' ya tiene un valor asociado
      // hay que sobreescribirlo
      ++cnt;
      tbl[pos] = new IP(key, val);
      // Factor de carga rebasado: hay que redimensionar la tabla 
      if (cnt >= (int)(0.8 * tbl.size())) resize();
    } else {
      // Hay hueco -> insertar ahi la nueva entrada
      tbl[pos]->second = val;
    }
  }
  
  // Elimina la entrada cuya clave es 'key'
  // Al igual que la insercion, el coste puede ser
  // entre O(1) -caso mejor-, y O(n) -caso peor.
  // Coste amortizado: O(1)
  void erase(int key) {
    // Encontrar entrada y eliminarla de la tabla
    int pos = slot(key, tbl);
    if (tbl[pos] != NULL) delete tbl[pos];
    // no puede haber huecos entre la posicion natural de una entrada
    // y su posicion establecida -si no, la busqueda de la funcion 'slot' fallaria
    // asi que hay que reubicar las entradas que caerian en la posicion eliminada
    int nex_pos = pos;
    while (tbl[pos] != NULL) {
      // eliminar la entrada
      tbl[pos] = NULL;
      bool loop = true;
      while (loop) {
	// si la siguiente posicion esta vacia, hemos terminado
	nex_pos = (nex_pos + 1) % tbl.size();
	if (tbl[nex_pos] == NULL) return;
	// 'p' es la posicion natural por hash de la siguiente entrada en la tabla
	// si 'p' no esta dentro del intervalo ciclico [pos, next_pos],
	// hay que rellenar la posicion vacia con la entrada en nex_pos
	int p = hash(tbl[nex_pos]->first, tbl.size());
	loop = pos <= nex_pos ? pos < p && p <= nex_pos : pos < p || p <= nex_pos;
      }
      // hacer la reubicacion de la entrada conflictiva
      tbl[pos] = tbl[nex_pos];
      pos = nex_pos;
    }
  }
  
  // Retorna el valor asociado a la clave 'key'
  // Si ningun valor ha sido asociado a esa clave, devuelve -(2^31)
  // Misma complejidad que 'insert' y 'erase'
  // Coste amortizado: O(1)
  int get_value(int key) {
    int pos = slot(key, tbl);
    if (tbl[pos] != NULL) return tbl[pos]->second;
    return 1 << 31;
  }
  
};

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  HashTable hash_table;
  cout << hash_table.get_value(999) << endl;
  hash_table.insert(7, 2);
  hash_table.insert(15, 3);
  hash_table.insert(23, 4);
  cout << hash_table.get_value(7) << endl;
  hash_table.erase(7);
  cout << hash_table.tbl[7]->first << ' ' << hash_table.tbl[7]->second << endl;
  cout << hash_table.get_value(15) << endl;
  
  /*
  hash_table.insert(32, 5);
  cout << hash_table.get_value(999) << endl;
  hash_table.insert(999, 6);
  cout << hash_table.get_value(999) << endl;
  hash_table.insert(66, 7);
  hash_table.insert(77, 8);
  hash_table.insert(88, 9);
  hash_table.insert(99, 10);
  hash_table.erase(999);
  cout << hash_table.get_value(999) << endl;
  */
  
  return 0;
}

