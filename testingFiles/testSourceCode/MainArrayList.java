import java.util.*;

public class MainArrayList {
    public static void main(String args[]) {
        
        int N = Integer.parseInt(args[0]);
        int tot = N / 10;

        ArrayList<Integer> arr = new ArrayList<Integer>();
        for (int te = 0; te < 10; ++te) {
            for (int i = 1; i <= tot; ++i) arr.add(i);
            for (int i = 0; i < arr.size(); ++i) arr.get(i);
            while (!arr.isEmpty()) arr.remove(arr.size() - 1);
        }

    }
}
