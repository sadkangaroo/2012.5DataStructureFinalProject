import java.util.*;

public class MainTreeSet {
    public static void main(String args[]) {
        
        int N = Integer.parseInt(args[0]);
        int tot = N / 10;

        Random random = new Random();
        TreeSet<Integer> tree = new TreeSet<Integer>();
        for (int te = 0; te < 10; ++te) {
            for (int i = 1; i <= tot; ++i) tree.add(random.nextInt());
        }

    }
}
