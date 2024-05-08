#include <bits/stdc++.h>
using namespace std;

ifstream f("abce.in");
ofstream g("abce.out");

class Node {
  public:
    long long key;

    Node **forward;

    Node( long long key, short int level) {
      this->key = key;
      forward = new Node*[level+1];
      memset(forward, 0, sizeof(Node*)*(level+1));
    };
};

class SkipList {
  short int MAXLVL;
  float P;
  short int level;
  Node *header;

  public:
    SkipList(short int MAXLVL, float P) {
      this->MAXLVL = MAXLVL;
      this->P = P;
      level = 0;
      header = new Node(-1, MAXLVL);
    };

    int randomLevel() {
      float r = (float)rand() / RAND_MAX;
      short int lvl = 0;
      while(r < P && lvl < MAXLVL) {
        lvl++;
        r = (float)rand() / RAND_MAX;
      }
      return lvl;
    };

    Node* createNode( long long key, short int level) {
      Node *n = new Node(key, level);
      return n;
    };

    void insertElement( long long key) {
      Node *current = header;

      Node *update[MAXLVL+1];
      memset(update, 0, sizeof(Node*)*(MAXLVL+1));

      for(int i = level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->key < key) {
          current = current->forward[i];
        }
        update[i] = current;
      }
    
      current = current->forward[0];
    
      if (current == NULL || current->key != key) {
        short int rlevel = randomLevel();

        if(rlevel > level) {
          for(short int i = level + 1; i < rlevel + 1; i++) {
            update[i] = header;
          }
          level = rlevel;
        }

        Node* n = createNode(key, rlevel);

        for(short int i = 0; i <= rlevel; i++) {
          n->forward[i] = update[i]->forward[i];
          update[i]->forward[i] = n;
        }
        //cout<<"Successfully Inserted key "<<key<<"\n";
      }
    };

    void deleteElement( long long key) {
      Node *current = header;

      Node *update[MAXLVL+1];
      memset(update, 0, sizeof(Node*)*(MAXLVL+1));

      for(short int i = level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->key < key) {
          current = current->forward[i];
        }
        update[i] = current;
      }

      current = current->forward[0];

      if(current != NULL and current->key == key) {
        for(short int i = 0; i <= level; i++) {
          if(update[i]->forward[i] != current)
            break;

          update[i]->forward[i] = current->forward[i];
        }

        while(level > 0 && header->forward[level] == 0) {
          level--;
        }
        //cout<<"Successfully deleted key "<<key<<"\n";
      }
    };

    void searchElement( long long key) {
      Node *current = header;
    
      for(short int i = level; i >= 0; i--) {
        while(current->forward[i] && current->forward[i]->key < key) {
          current = current->forward[i];
        }
      }

      current = current->forward[0];

      if(current && current->key == key) {
        g << "1\n";
      } else {
        g << "0\n";
      }
    };

    void searchBiggestSmallerElement( long long key) {
      Node *current = header;
    
      for(short int i = level; i >= 0; i--) {
        while(current->forward[i] && current->forward[i]->key <= key) {
          current = current->forward[i];
        }
      } 

      g << current->key << '\n';
    };

    void searchSmallestBiggerElement( long long key) {
      Node *current = header;
    
      for(short int i = level; i >= 0; i--) {
        while(current->forward[i] && current->forward[i]->key < key) {
          current = current->forward[i];
        }
      }

      current = current->forward[0];

      if(current && current->key >= key)
        g << current->key << '\n';
    };

    void displayListBetweenXY( long long x,  long long y) {
      Node *current = header;

      for(short int i = level; i >= 0; i--) {
        while(current->forward[i] && current->forward[i]->key < x) {
          current = current->forward[i];
        }
      }
    
      current = current->forward[0];
      while(current && current->key <= y) {
        g << current->key << ' ';
        current = current->forward[0];
      }

      g << '\n';
    }

    void displayList() {
      g<<"\n*****Skip List*****"<<"\n";
      for(short int i = 0; i <= level; i++) {
        Node *node = header->forward[i];
        g<<"Level "<<i<<": ";
        while(node != NULL) {
            g<<node->key<<" ";
            node = node->forward[i];
        }
        g<<"\n";
      }
    };
};

int main() {
  srand((unsigned)time(0));

  short int t;
   long long q, x, y;
  f >> q;

  SkipList lst(32, 0.5);

  for( long long i = 0; i < q; ++i) {
    f >> t;
    switch(t) {
      case 1:
        f >> x;
        lst.insertElement(x);
        break;
      case 2:
        f >> x;
        lst.deleteElement(x);
        break;
      case 3:
        f >> x;
        lst.searchElement(x);
        break;
      case 4:
        f >> x;
        lst.searchBiggestSmallerElement(x);
        break;
      case 5:
        f >> x;
        lst.searchSmallestBiggerElement(x);
        break;
      case 6:
        //lst.displayList();
        f >> x >> y;
        lst.displayListBetweenXY(x, y);
        break;
      default:
        g << "wrong input\n";
    }
  }
}