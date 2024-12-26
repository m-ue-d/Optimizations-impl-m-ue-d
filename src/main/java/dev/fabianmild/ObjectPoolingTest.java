package dev.fabianmild;

import java.util.ArrayList;
import java.util.List;

class Bullet {
    private boolean inAir;
    private double[] path;
    private int position;

    public Bullet() {
        this.inAir = false;
        this.position = 0;
        this.path = new double[1000];
    }

    public void shoot(int fromPosition) {
        this.inAir = true;
        this.position = fromPosition;
        for (int i = 0; i < path.length; i++) {
            path[i] = position + Math.random();
        }
    }

    public void hit() {
        this.inAir = false;
        //do some other logic here
    }

    public boolean isInAir() {
        return inAir;
    }
}


class BulletPool {
    private final List<Bullet> bullets;

    public BulletPool(int size) {
        bullets = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            bullets.add(new Bullet());
        }
    }

    public Bullet getBullet() {
        for (Bullet bullet : bullets) {
            if (!bullet.isInAir()) {
                return bullet;
            }
        }
        return null;
    }
}


public class ObjectPoolingTest {
    public static void test(int iterations) {
        for (int i = 0; i < 10; i++) {  //warm-up the jvm
            runTest(iterations, false);
        }

        System.out.printf("\n--- Test with <%d> iterations --- \n", iterations);
        runTest(iterations, true);
    }

    private static void runTest(int iterations, boolean out) {
        if(out) System.out.printf("Iterations: %d\n", iterations);

        BulletPool pool = new BulletPool(1000);
        long startTime = System.nanoTime();

        for (int i = 0; i < iterations; i++) {
            Bullet bullet = pool.getBullet();
            if (bullet != null) {
                bullet.shoot(0);
                bullet.hit();
            }
        }

        long endTime = System.nanoTime();
        if(out) System.out.printf("Object Pooling: %s ms\n", (endTime - startTime) / 1_000_000.0);

        startTime = System.nanoTime();

        for (int i = 0; i < iterations; i++) {
            Bullet bullet = new Bullet();
            bullet.shoot(0);
            bullet.hit();
        }

        endTime = System.nanoTime();
        if(out) System.out.printf("Dynamic Object Creation: %s ms\n", (endTime - startTime) / 1_000_000.0);
    }
}