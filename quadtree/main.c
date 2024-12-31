#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "quadtree.c"  // Include the QuadTree implementation from the previous code

//thanks to Daniel Vasallo (https://stackoverflow.com/questions/2150291/how-do-i-measure-a-time-interval-in-c)
double getElapsedTime(clock_t start, clock_t end) {
    return ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
}

void generateRandomPoints(Point* points, int count, float maxRange) {
    if(!points) return;

    for (int i = 0; i < count; i++) {
        points[i].x = (float)(rand() % (int)(maxRange * 2)) - maxRange;
        points[i].y = (float)(rand() % (int)(maxRange * 2)) - maxRange;
    }
}

int isPointInBoundary(Point p, Boundary b) {
    return fabsf(p.x - b.x) <= b.halfWidth && fabsf(p.y - b.y) <= b.halfHeight;
}

int arrayCollisionDetection(Point* points, int count, Boundary queryBox, Point* result) {
    if(!points || !result) return 0;

    int foundCount = 0;
    for (int i = 0; i < count; i++) {
        if (isPointInBoundary(points[i], queryBox)) {
            result[foundCount++] = points[i];
        }
    }
    return foundCount;
}

void test(int numPoints, float maxRange, float queryRange) {
    Point* points = malloc(sizeof(Point) * numPoints);
    generateRandomPoints(points, numPoints, maxRange);

    Boundary qtBoundary = createBoundary(0, 0, maxRange, maxRange);
    QuadTree* qt = createQuadTree(qtBoundary);

    clock_t start = clock();
    for (int i = 0; i < numPoints; i++) {
        insert(qt, points[i]);
    }
    clock_t end = clock();
    double qtInsertTime = getElapsedTime(start, end);

    Point* qtResult = malloc(sizeof(Point) * numPoints);
    int qtFound = 0;
    Boundary queryBox = createBoundary(0, 0, queryRange, queryRange);
    start = clock();
    quadTreeQueryRange(qt, queryBox, qtResult, &qtFound);
    end = clock();
    double qtQueryTime = getElapsedTime(start, end);

    Point* arrayResult = malloc(sizeof(Point) * numPoints);
    start = clock();
    arrayCollisionDetection(points, numPoints, queryBox, arrayResult);
    end = clock();
    double arrayQueryTime = getElapsedTime(start, end);

    printf("Iterations: %d\n", numPoints);
    printf("QuadTree Insert: %.6f ms\n", qtInsertTime);
    printf("QuadTree Query: %.6f ms\n", qtQueryTime);
    printf("Array Query: %.6f ms\n\n", arrayQueryTime);

    free(points);
    free(qtResult);
    free(arrayResult);
    freeQuadTree(qt);
}

int main() {
    srand((unsigned)time(NULL));
    long pL = 100000;
    long pH = 10000000;

    printf("Light Load Test:\n");
    printf("--- Test with <%ld> points ---\n", pL);
    test(pL, 100.0f, 10.0f);

    printf("Heavy Load Test:\n");
    printf("--- Test with <%ld> points ---\n", pH);
    test(pH, 1000.0f, 100.0f);   //increased max range to improve insert performance of such a big quadTree

    return 0;
}