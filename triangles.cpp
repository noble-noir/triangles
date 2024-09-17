#include <iostream>
#include <cmath>

namespace
{
struct Point
{
    double x, y, z;
    Point(double a, double b, double c) : x(a), y(b), z(c){}
    void Point_read()
    {
        std::cin >> x >> y >> z;
    }
};
struct Triangle
{
    Point tr[3];
    Triangle() : tr[0]{0, 0, 0}, tr[1]{0, 0, 0}, tr[2]{0, 0, 0} {};
    void Triangle_read()
    {
        tr[0].Point_read();
        tr[1].Point_read();
        tr[2].Point_read();
    }
};


struct Plane
{
    double a, b, c, d;
    Plane(Point a1, Point a2, Point a3)
    {
        a = (a2.y-a1.y)*(a3.z-a1.z)-(a2.z-a1.z)*(a3.y-a1.y);
        b = -(a2.x-a1.x)*(a3.z-a1.z)+(a2.z-a1.z)*(a3.x-a1.x);
        c = -(a2.y-a1.y)*(a3.x-a1.x)+(a2.x-a1.x)*(a3.y-a1.y);
        d = -a1.x*a+a1.y*b-a1.z*c;
    }
    Plane(Triangle tr) : Plane(tr.tr[0], tr.tr[1], tr.tr[2]) {}
    int pl_charact(Point p)
    {
        return a*p.x+b*p.y+c*p.z+d;
    }
    void Plane_print()
    {
        std::cout << "plane " << std::endl << a << std::endl << b << std::endl << c << std::endl << d << std::endl;
    }
};


// // struct dir_segm
// // {
// //     double x, y, z;
//     double len()
//     {
//         return std::hypot(x, y, z);
//     }
// //     dir_segm(double a, double b, double c) : x(a), y(b), z(c){}
// //     dir_segm(Point a, Point b) : x(b.x-a.x), y(b.y-a.y), z(b.z-a.z){}
//     dir_segm operator-(const dir_segm &a)
//     {
//         dir_segm res{x-a.x, y-a.y, z-a.z};
//         return res;
//     }
// //     dir_segm operator*(const double a)
// //     {
// //         dir_segm res{x*a, y*a, z*a};
// //         return res;
// //     }
// // };

// Pooint::Point operator+(Point A, dir_segm v)
// {
//     Point res{A.x+v.x, A.y+v.y, A.z+v.z};
//     return res;
// }
struct dir_segm
{
    Point init, term;
    dir_segm(Point a, Point b) : init(a), term(b){}
    double len()
    {
        return std::hypot(term.x-init.x, term.y-init.y, term.z-init.z);
    }
    // dir_segm operator-(const dir_segm &a)
    // {
    //     dir_segm res{init-a.init, term-a.term};
    //     return res;
    // }
    dir_segm operator*(const double a)
    {
        Point result{init.x+(term.x-init.x)*a, init.y+(term.y-init.y)*a, init.z+(term.z-init.z)*a};
        dir_segm res{init, result};
        return res;
    }
    double operator*(dir_segm b)
    {
            return (this->term.x-this->init.x)*(b.term.x-b.init.x)+(this->term.y-this->init.y)*(b.term.y-b.init.y)+(this->term.z-this->init.z)*(b.term.z-b.init.z);
    }
};

int Charact(Plane pl, dir_segm vec)
{
    return pl.pl_charact(vec.init)*pl.pl_charact(vec.term);
}
// double sc_mul(dir_segm a, dir_segm b)
// {
//     return (a.term.x-a.init.x)*(b.term.x-b.init.x)+(a.term.y-a.init.y)*(b.term.y-b.init.y)+(a.term.z-a.init.z)*(b.term.z-b.init.z);
// }
double cos_dir_segm(dir_segm a, dir_segm b)
{
    return a*b/(a.len()*b.len());
}
int dir_segn_intersect(dir_segm AB, dir_segm CD)
{
    dir_segm AC{AB.init, CD.init};
    dir_segm AD{AB.init, CD.term};
    double k = cos_dir_segm(AB, CD)*AC.len()/AB.len();
    dir_segm AH{AB*k};
    dir_segm HC{AH.term, AC.term};

    dir_segm CA{AC.term, AC.init};
    dir_segm CB{CD.init, AB.term};
    double l = cos_dir_segm(CA, CD)*CA.len()/CD.len();
    dir_segm CK{CD*l};
    dir_segm KA{CK.term, AB.init};

    if(((HC*AC)*(HC*AD) <= 0) && ((KA*CA)*(KA*CB) <= 0))
        return 1;
    return 0;
}

int triangle_vec_intersect_2d(Triangle tr, dir_segm vec)
{
    dir_segm v23 {tr.tr[1], tr.tr[2]};
    if (dir_segn_intersect(v23, vec) != 0)
        return 1;
    dir_segm v13 {tr.tr[0], tr.tr[2]};
    if (dir_segn_intersect(v13, vec) != 0)
        return 1;
    dir_segm v12 {tr.tr[0], tr.tr[1]};
    if (dir_segn_intersect(v12, vec) != 0)
        return 1;
    return 0;
}

int triangle_vec_intersect_3d(Triangle tr, dir_segm vec)
{
    Plane pl{tr};
    if (Charact(pl, vec) > 0)
        return 0;
    Plane first(vec.init, vec.term, tr.tr[0]);
    dir_segm v23 {tr.tr[1], tr.tr[2]};
    if (Charact(first, v23) > 0)
        return 0;
    Plane second(vec.init, vec.term, tr.tr[1]);
    dir_segm v13 {tr.tr[0], tr.tr[2]};
    if (Charact(second, v13) > 0)
        return 0;
    Plane third(vec.init, vec.term, tr.tr[2]);
    dir_segm v12 {tr.tr[0], tr.tr[1]};
    if (Charact(third, v12) > 0)
        return 0;
    return 1;
}
int triangles_intersect(Triangle tr_f, Triangle tr_s)
{
    Plane pl_f{tr_f};
    Plane pl_s{tr_s};
    dir_segm v23 {tr_s.tr[1], tr_s.tr[2]};
    dir_segm v13 {tr_s.tr[0], tr_s.tr[2]};
    dir_segm v12 {tr_s.tr[0], tr_s.tr[1]};
    double k = pl_f.a/pl_s.a;
    if((pl_f.b/pl_s.b == k) && (pl_f.c/pl_s.c == k))
    {
        if(pl_f.d/pl_s.d != k)//paralel
            return 0;
        else//one plane
        {
            return triangle_vec_intersect_2d(tr_f, v23)+triangle_vec_intersect_2d(tr_f, v13)+triangle_vec_intersect_2d(tr_f, v12);
        }
    }
    //not paralel
    return triangle_vec_intersect_3d(tr_f, v23)+triangle_vec_intersect_3d(tr_f, v13)+triangle_vec_intersect_3d(tr_f, v12);
}
}


int main()
{
    Triangle tr_f, tr_s;
    tr_f.Triangle_read();
    tr_s.Triangle_read();
    //Plane pl_f{tr_f};
    //pl_f.Plane_print();
    //std::cout << triangles_intersect(tr_f, tr_s) << std::endl;
    if (triangles_intersect(tr_f, tr_s) != 0)
        std::cout << "yes" << std::endl;
    else
        std::cout << "no" << std::endl;
    return 0;
}