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

    ArrayList<int> arr;
    for (int te = 0; te < 10; ++te) {
        for (int i = 1; i <= tot; ++i) arr.add(i);
        for (int i = 0; i < arr.size(); ++i) arr.get(i);
        while (!arr.isEmpty()) arr.removeIndex(arr.size() - 1);
    }

    return 0;

}

