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

    LinkedList<int> lnk;
    for (int te = 0; te < 10; ++te) {
        for (int i = 1; i <= tot; ++i) lnk.add(i);
        while (!lnk.isEmpty()) lnk.removeLast();
    }

    return 0;

}
