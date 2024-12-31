#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 8

typedef struct {
    float x, y;
} Point;

typedef struct {
    float x, y;
    float halfWidth;
    float halfHeight;
} Boundary;

//a node in the quadtree
typedef struct QuadTree {
    Boundary boundary;
    Point points[MAX_POINTS];
    int pointCount;

    struct QuadTree* northwest;
    struct QuadTree* northeast;
    struct QuadTree* southwest;
    struct QuadTree* southeast;

} QuadTree;

//create the structs via functions. C is cool =)
Boundary createBoundary(float x, float y, float halfWidth, float halfHeight) {
    Boundary boundary = {x, y, halfWidth, halfHeight};
    return boundary;
}
QuadTree* createQuadTree(Boundary boundary) {
    QuadTree* qt = (QuadTree*)malloc(sizeof(QuadTree));
    qt->boundary = boundary;
    qt->pointCount = 0;
    qt->northwest = qt->northeast = qt->southwest = qt->southeast = NULL;
    return qt;
}

int containsPoint(Boundary boundary, Point point) {
    return (point.x >= boundary.x - boundary.halfWidth &&
            point.x <= boundary.x + boundary.halfWidth &&
            point.y >= boundary.y - boundary.halfHeight &&
            point.y <= boundary.y + boundary.halfHeight);
}

void subdivide(QuadTree* qt) {
    float x = qt->boundary.x;
    float y = qt->boundary.y;
    float hw = qt->boundary.halfWidth / 2;
    float hh = qt->boundary.halfHeight / 2;

    qt->northwest = createQuadTree(createBoundary(x - hw, y - hh, hw, hh));
    qt->northeast = createQuadTree(createBoundary(x + hw, y - hh, hw, hh));
    qt->southwest = createQuadTree(createBoundary(x - hw, y + hh, hw, hh));
    qt->southeast = createQuadTree(createBoundary(x + hw, y + hh, hw, hh));
}

int insert(QuadTree* qt, Point point) {
    if (!containsPoint(qt->boundary, point)) {
        return 0;
    }
    if (qt->pointCount < MAX_POINTS) {
        qt->points[qt->pointCount++] = point;
        return 1;
    }
    if (qt->northwest == NULL) {
        subdivide(qt);
    }

    if (insert(qt->northwest, point)) return 1;
    if (insert(qt->northeast, point)) return 1;
    if (insert(qt->southwest, point)) return 1;
    if (insert(qt->southeast, point)) return 1;

    return 0;
}

void quadTreeQueryRange(QuadTree* qt, Boundary range, Point* foundPoints, int* foundCount) {
    if (!(range.x - range.halfWidth > qt->boundary.x + qt->boundary.halfWidth ||
          range.x + range.halfWidth < qt->boundary.x - qt->boundary.halfWidth ||
          range.y - range.halfHeight > qt->boundary.y + qt->boundary.halfHeight ||
          range.y + range.halfHeight < qt->boundary.y - qt->boundary.halfHeight)) {

        for (int i = 0; i < qt->pointCount; i++) {
            if (containsPoint(range, qt->points[i])) {
                foundPoints[(*foundCount)++] = qt->points[i];
            }
        }

        if (qt->northwest != NULL) {
            quadTreeQueryRange(qt->northwest, range, foundPoints, foundCount);
            quadTreeQueryRange(qt->northeast, range, foundPoints, foundCount);
            quadTreeQueryRange(qt->southwest, range, foundPoints, foundCount);
            quadTreeQueryRange(qt->southeast, range, foundPoints, foundCount);
        }
    }
}

//cool C kids always free their memory =)
void freeQuadTree(QuadTree* qt) {
    if (qt == NULL) return;

    freeQuadTree(qt->northwest);
    freeQuadTree(qt->northeast);
    freeQuadTree(qt->southwest);
    freeQuadTree(qt->southeast);

    free(qt);
}
