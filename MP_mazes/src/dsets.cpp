#include <vector>
#include "dsets.h"


void DisjointSets::addelements(int num) {
        for(int i = 0; i < num; i++) {
            s.push_back(-1);
        }
    }

   int DisjointSets::find(int elem) {
   
        if (s[elem] < 0) {
            return elem; 
        } else {
            s[elem] = find(s[elem]);
            return s[elem];
        }
    }

void DisjointSets::setunion(int a, int b) {
        int root1 = find(a);
        int root2 = find(b);

        if(root1 != root2) { 
          
        if(s[root1] <= s[root2]) { 
                s[root1] += s[root2];
                s[root2] = root1;
        } else { // root2 
                s[root2] += s[root1];
                s[root1] = root2;
        }
        }
    }

    int DisjointSets::size(int elem) {
        int f = find(elem);
        return -s[f];
    }


