 
//Author: Richard Escalona
import java.util.*;
//thread that will merge first and second half of A
class mergeThread implements Runnable {

    private double[] out;
    private double[] first;
    private double[] second;
    private int endIndex;

    public mergeThread(double[] firsthalf, double[] secondhalf, double[] C, int arrSize) {
        out = C;
        first = firsthalf;
        second = secondhalf;
        endIndex = arrSize;

    }

    public void run() {
        int i, j;
        for (i = 0; i < endIndex / 2; i++) {
            out[i] = first[i];
        }
        for (i = 0, j = endIndex / 2; j < endIndex && i < endIndex / 2; i++, j++) {
            out[j] = second[i];
        }
    }
}

//thread that will sort
class threadSort implements Runnable {

    private double[] arrSort;
    private int arrSize, start;

    public threadSort(double[] myarr, int startIndex, int endIndex) {
        arrSort = myarr;
        arrSize = endIndex;
        start = startIndex;
    }

    // @Override
    public void run() {
        int i, j;
        double temp;

        for (i = start; i < arrSize; i++) {
            temp = arrSort[i];
            j = i - 1;
            while (j >= 0 && arrSort[j] > temp) {
                arrSort[j + 1] = arrSort[j];
                j = j - 1;
            }
            arrSort[j + 1] = temp;
        }
    }
}

public class part2 {
    public static void main(String[] arg) {

        int arrSize = Integer.parseInt(arg[0]);
        int arrHalf = arrSize / 2;
        int i;

        double[] A = new double[arrSize];
        double[] B = new double[arrSize];
        double[] C = new double[arrSize];

        double[] Afirsthalf = new double[arrSize / 2];
        double[] Asecondhalf = new double[arrSize / 2];

        Random randomDoubles = new Random();
        double start_t, start_t1, end_t, end_t1;

        // generating random doubles for A
        for (i = 0; i < arrSize; i++) {

            A[i] = randomDoubles.nextDouble() * 1000.0;
        }
        // copy A to B
        for (i = 0; i < arrSize; i++) {
            B[i] = A[i];
        }
        // copy first half of A to Afirsthalf
        for (i = 0; i < arrHalf; i++) {
            Afirsthalf[i] = A[i];
        }
        // copy first half of A to Afirsthalf
        for (i = arrHalf; i < arrSize; i++) {
            Asecondhalf[i - arrHalf] = A[i];
        }
        // single threading creation
        threadSort r1 = new threadSort(B, 0, arrSize);
        Thread THB = new Thread(r1);
        start_t = System.nanoTime();
        THB.start(); // START CLOCK FOR SINGLE THREAD
        try {
            THB.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        end_t = System.nanoTime() - start_t; // STOP CLOCK FOR SINGLE THREAD

        System.out.println("Sorting is done in " + end_t / 1000000.0 + "ms when one thread is used");

        // initializing the threads that I will be using for the multi-threading/merge part
        threadSort r2 = new threadSort(Afirsthalf, 0, arrHalf);
        threadSort r3 = new threadSort(Asecondhalf, 0, arrHalf);
        mergeThread r4 = new mergeThread(Afirsthalf, Asecondhalf, C, arrSize);

        Thread MRG = new Thread(r4);
        Thread THA1 = new Thread(r2);
        Thread THA2 = new Thread(r3);

        start_t1 = System.nanoTime(); //START MULTITHREAD CLOCK
        THA1.start();
        THA2.start();
        // wait for threads THA's
        try {
            THA1.join();
            THA2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        // Start the merging thread Afirsthalf and Asecondhalf ( will be saved to array C )
        
        MRG.start();

        try {
            MRG.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        end_t1 = System.nanoTime() - start_t1; // END MULTITHREAD CLOCK

        System.out.println("Sorting is done in " + end_t1 / 1000000.0 + "ms when two threads are used");
    }
}