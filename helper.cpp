#include "helper.hpp"

using Shape::Point;

Shape::Helper::Helper()
{
}
Shape::Helper::~Helper()
{
}

double Shape::Helper::dist(double x1, double y1, double z1, double x2)
{
    // In the case of 2d: z1 means x2 and x2 means y2
    return hypot(z1 - x1, x2 - y1);
};

// https://en.wikipedia.org/wiki/Hypot
// Hypot is a mathematical function defined to calculate the length 
// of the hypotenuse of a right-angle triangle. 
// It was designed to avoid errors arising due to 
// limited-precision calculations performed on computers.
double Shape::Helper::hypot(double x, double y)
{
    double max = 0;
    double point[2];
    point[0] = x;
    point[1] = y;
    for (int i = 0; i < 2; i++)
    {
        double n = point[i];
        n = +n;
        n = abs(n);
        if (n > max)
            max = n;
        point[i] = n;
    }

    if (max == 0)
        max = 1;

    double sum = 0;
    double compensation = 0;

    for (int j = 0; j < 2; j++)
    {
        double m = point[j] / max;
        double summand = m * m - compensation;
        double preliminary = sum + summand;
        compensation = (preliminary - sum) - summand;
        sum = preliminary;
    }
    return sqrt(sum) * max;
}

// Rotates a point about a fixed point
Point *Shape::Helper::rotatePoint(Point fp, Point pt, double a)
{
    auto x = pt.x - fp.x;
    auto y = pt.y - fp.y;
    auto xRot = x * cos(a) + y * sin(a);
    auto yRot = y * cos(a) - x * sin(a);
    return new Point(fp.x + xRot, fp.y + yRot);
}

double Shape::Helper::acossafe(double x)
{
    if (x >= +1.0)
        return 0;
    if (x <= -1.0)
        return M_PI;
    return acos(x);
}

Point *Shape::Helper::lineLineIntersection(LineSegment l1, LineSegment l2)
{
/*     
    Point *p0 = new Point(l1.x1, l1.y1);
    Point *p1 = new Point(l1.x2, l1.y2);
    Point *p2 = new Point(l2.x1, l2.y1);
    Point *p3 = new Point(l2.x2, l2.y2); */
    unique_ptr<Point> p0 (new Point(l1.x1, l1.y1));
    unique_ptr<Point> p1 (new Point(l1.x1, l1.y1));
    unique_ptr<Point> p2 (new Point(l1.x1, l1.y1));
    unique_ptr<Point> p3 (new Point(l1.x1, l1.y1));
    
    Point s1, s2;
    s1.x = p1->x - p0->x;
    s1.y = p1->y - p0->y;
    s2.x = p3->x - p2->x;
    s2.y = p3->y - p2->y;

    double s10_x = p1->x - p0->x;
    double s10_y = p1->y - p0->y;
    double s32_x = p3->x - p2->x;
    double s32_y = p3->y - p2->y;

    double denom = s10_x * s32_y - s32_x * s10_y;

    if (denom == 0)
    {
/*         SafeDelete(p0);
        SafeDelete(p1);
        SafeDelete(p2);
        SafeDelete(p3); */
        return NULL;
    }

    bool denom_positive = denom > 0;

    double s02_x = p0->x - p2->x;
    double s02_y = p0->y - p2->y;

    double s_numer = s10_x * s02_y - s10_y * s02_x;

    if ((s_numer < 0) == denom_positive)
    {
/*         SafeDelete(p0);
        SafeDelete(p1);
        SafeDelete(p2);
        SafeDelete(p3); */
        return NULL;
    }

    double t_numer = s32_x * s02_y - s32_y * s02_x;

    if ((t_numer < 0) == denom_positive)
    {
/*         SafeDelete(p0);
        SafeDelete(p1);
        SafeDelete(p2);
        SafeDelete(p3); */
        return NULL;
    }

    if ((s_numer > denom) == denom_positive || (t_numer > denom) == denom_positive)
    {
/*         SafeDelete(p0);
        SafeDelete(p1);
        SafeDelete(p2);
        SafeDelete(p3); */
        return NULL;
    }

    double t = t_numer / denom;

    return new Point(p0->x + (t * s10_x), p0->y + (t * s10_y));
}

list<Point> *Shape::Helper::circleCircleIntersectionPoints(Circle c1, Circle c2)
{
    double r, R, d, cx, cy, Cx, Cy;
    if (c1.r < c2.r)
    {
        r = c1.r;
        R = c2.r;
        cx = c1.x;
        cy = c1.y;
        Cx = c2.x;
        Cy = c2.y;
    }
    else
    {
        r = c2.r;
        R = c1.r;
        Cx = c1.x;
        Cy = c1.y;
        cx = c2.x;
        cy = c2.y;
    }

    // Find the distance between two points.
    d = dist(cx, cy, Cx, Cy);

    // There are an infinite number of solutions
    // Seems appropriate to also return null
    if (d < EPS && abs(R - r) < EPS)
        return NULL;

    // No intersection (circles centered at the
    // same place with different size)
    else if (d < EPS)
        return NULL;

    double vx = cx - Cx,
           vy = cy - Cy;
    double x = (vx / d) * R + Cx,
           y = (vy / d) * R + Cy;
    Point *P = new Point(x, y);

    // Single intersection (kissing circles)
    if (abs((R + r) - d) < EPS || abs(R - (r + d)) < EPS)
        return NULL; //[P];

    // No intersection. Either the small circle contained within
    // big circle or circles are simply disjoint.
    if ((d + r) < R || (R + r < d))
        return NULL;

    Point *C = new Point(Cx, Cy);
    double angle = acossafe((r * r - d * d - R * R) / (-2.0 * d * R));
    Point *pt1 = rotatePoint(*C, *P, +angle);
    Point *pt2 = rotatePoint(*C, *P, -angle);
    //return [ pt1, pt2 ];
    return NULL; //TEMP!!!!!!!!!!!!!
};