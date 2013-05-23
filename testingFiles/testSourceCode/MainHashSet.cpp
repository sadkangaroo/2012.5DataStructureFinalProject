#include "Utility.h"
#include "ArrayList.h"
#include "LinkedList.h"
#include "HashSet.h"
#include "HashMap.h"
#include "TreeSet.h"
#include "TreeMap.h"

#include<cstdlib>

class Hashint {
    public:
         static int hashcode(int obj) {
             return obj;
         }
};

int main(int argc, char **argv) {

    int N = atoi(argv[1]);
    int tot = N / 10;

    HashSet<int, Hashint> set;
    for (int te = 0; te < 10; ++te) {
        for (int i = 1; i <= tot; ++i) set.add(i);
        for (int i = 1; i <= tot; ++i) set.remove(i);
    }

    return 0;

}
