package dev.fabianmild;

public class Main {
    public static void main(String[] args) {
        System.out.println("Light Load Test:");
        ObjectPoolingTest.test(1_000);
        System.out.println("\n\nHeavy Load Test:");
        ObjectPoolingTest.test(100_000);
    }
}