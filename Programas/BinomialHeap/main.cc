#include <iostream>
#include <list>
using namespace std;

struct Node {
  int val;
  int ord;
  list<Node*> children;
};

typedef list<Node*> NL;
typedef NL::iterator NLI;

struct MaxHeap {
  
  NL trees;
  
  Node* merge_trees(Node* t1, Node* t2) {
    if (t1->val > t2->val) {
      t1->children.push_back(t2);
      ++t1->ord;
      return t1;
    } else {
      t2->children.push_back(t1);
      ++t2->ord;
      return t2;
    }
  }
  
  void merge(NL &nodes) {
    Node* car = NULL;
    NLI it1 = trees.begin();
    NLI it2 = nodes.begin();
    while (it1 != trees.end() && it2 != nodes.end()) {
      Node* t1 = *it1;
      Node* t2 = *it2;
      if (car != NULL) {
	
	if (car->ord == t1->ord && car->ord == t2->ord) {
	  //cout << "1 + 1 + carry" << endl;
	  // 1 + 1 + carry
	  car = merge_trees(t1, t2);
	  *it1 = car;
	  ++it2;
	} else if (car->ord == t1->ord) {
	  //cout << "1 + 0 + carry" << endl;
	  // 1 + 0 + carry
	  t1 = merge_trees(t1, car);
	  *it1 = t1;
	  car = NULL;
	} else if (car->ord == t2->ord) {
	  //cout << "0 + 1 + carry" << endl;
	  // 0 + 1 + carry
	  t2 = merge_trees(t2, car);
	  trees.insert(it1, t2);
	  ++it2;
	  car = NULL;
	} else {
	  //cout << "0 + 0 + carry" << endl;
	  // 0 + 0 + carry
	  trees.insert(it1, car);
	  car = NULL;
	}
	++it1;
	
      } else {
	
	if (t1->ord == t2->ord) {
	  //cout << "1 + 1 " << trees.size() << endl;
	  // 1 + 1
	  it1 = trees.erase(it1);
	  car = merge_trees(t1, t2);
	  ++it2;
	} else if (t1->ord > t2->ord) {
	  //cout << "1 + 0 " << trees.size() << endl;
	  trees.insert(it1, t2);
	  ++it2;
	}
	
      }
    }
    
    while (it1 != trees.end() && car != NULL) {
      Node* t1 = *it1;
      if (t1->ord == car->ord) {
	//cout << "insert from t1 carry" << endl;
	car = merge_trees(t1, car);
	it1 = trees.erase(it1);
      } else {
	trees.insert(it1, car);
	car = NULL;
      }
    }
    
    while (it2 != nodes.end()) {
      Node* t2 = *it2;
      if (car != NULL) {
	//cout << "insert from t2 carry" << endl;
	t2 = merge_trees(t2, car);
	trees.insert(trees.end(), t2);
	++it2;
	car = NULL;
      } else {
	//cout << "insert from t2" << endl;
	trees.insert(trees.end(), t2);
	++it2;
      }
    }
    
    if (car != NULL) {
      trees.insert(trees.end(), car);
    }
  }
  
  void merge(MaxHeap &heap) {
    merge(heap.trees);
  }
  
  void insert(int x) {
    NL nodes;
    Node* nod = new Node({ x, 0, NL() });
    nodes.insert(nodes.end(), nod);
    merge(nodes);
  }
  
  int top() {
    int mx = 1 << 31;
    for (NLI it = trees.begin(); it != trees.end(); ++it) {
      cout << "tree " << (*it)->ord << ' ' << (*it)->val << endl;
      if ((*it)->val > mx) {
	mx = (*it)->val;
      }
    }
    return mx;
  }
  
};

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  MaxHeap heap;
  heap.insert(1);
  heap.insert(2);
  heap.insert(3);
  heap.insert(4);
  heap.insert(5);
  heap.insert(6);
  heap.insert(7);
  heap.insert(8);
  heap.insert(9);
  //cout << heap.top() << endl;
  
  MaxHeap heap2;
  heap2.insert(3);
  heap2.merge(heap);
  cout << heap2.top() << endl;
  
  return 0;
}

