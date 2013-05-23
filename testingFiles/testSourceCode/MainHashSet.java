import java.util.*;

public class MainHashSet {
    public static void main(String args[]) {
        
        int N = Integer.parseInt(args[0]);
        int tot = N / 10;

        HashSet<Integer> set = new HashSet<Integer>();
        for (int te = 0; te < 10; ++te) {
            for (int i = 1; i <= tot; ++i) set.add(i);
            for (int i = 1; i <= tot; ++i) set.remove(i);
        }

    }
}
