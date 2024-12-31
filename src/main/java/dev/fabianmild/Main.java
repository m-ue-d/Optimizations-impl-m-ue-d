package dev.fabianmild;

public class Main {
    public static void main(String[] args) {
        System.out.println("Object Pooling: ");
        System.out.println("Light Load Test:");
        ObjectPoolingTest.testMain(1_000);
        System.out.println("\n\nHeavy Load Test:");
        ObjectPoolingTest.testMain(100_000);
    }
}ä