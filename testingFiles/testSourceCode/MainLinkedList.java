import java.util.*;

public class MainLinkedList {
    public static void main(String args[]) {
        
        int N = Integer.parseInt(args[0]);
        int tot = N / 10;

        LinkedList<Integer> lnk = new LinkedList<Integer>();
        for (int te = 0; te < 10; ++te) {
            for (int i = 1; i <= tot; ++i) lnk.add(i);
            while (!lnk.isEmpty()) lnk.removeLast();
        }
    }
}
