#ifndef LVECTOR_H
#define LVECTOR_H

#include "defines.h"
#include <vector>
#include <cmath>
#define PI (M_PI)
#define DEG_TO_RAD(angle) ((PI*angle)/180)
#define RAD_TO_DEG(angle) ((angle*180)/PI)

#ifndef MAIOR
#define MAIOR(a, b) (a>b?a:b)
#define MENOR(a, b) (a<b?a:b)
#endif

template <typename T>
class LVector
{
    public:
        LVector(T x_, T y_)
        : x(x_), y(y_)
        {}
        LVector()
        : x(0), y(0)
        {}
        LVector(const LVector& v)
        : x(v.x), y(v.y)
        {}
        virtual ~LVector()
        {}

        T x;
        T y;

        double magnitude()
        {
            return std::sqrt((x*x) + (y*y));
        }
        double magnitudeSquared()
        {
            return ((x*x) + (y*y));
        }

        // Vector in the format < cos, sin >
        LVector<T>& normalize()
        {
            return (*this) = getUnitVector();
        }

        LVector<T>& rotate(double angleDegree)
        {
            double theta = DEG_TO_RAD(angleDegree);
            double cs = cos(theta);
            double sn = sin(theta);
            T px = x * cs - y * sn;
               y = x * sn + y * cs;
            x = px;
            return *this;
        }

        // Vector in the format < cos, sin >
        LVector<double> getUnitVector()
        {
            if (x||y)
                return LVector<double>(*this)/magnitude();
            return LVector<double>(0, 0);
        }

        T dotProduct(const LVector<T>& v)
        {
            return (x*v.x) + (y*v.y);
        }

        void confineX(T minX, T maxX)
        {
            x = x>maxX?maxX:x;
            x = x<minX?minX:x;
        }
        void confineY(T minY, T maxY)
        {
            y = y>maxY?maxY:y;
            y = y<minY?minY:y;
        }

        T distanceSquared(const LVector<T>& v)
        {
            return ((x-v.x)*(x-v.x)) + ((y-v.y)*(y-v.y));
        }
        double distance(const LVector<T>& v)
        {
            return sqrt(distanceSquared(v));
        }

/*
        LVector<T> crossProduct(const LVector<T>& v)
        {
            T a = y * v.z - z * v.y;
            T b = z * v.x - x * v.z;
            T c = x * v.y - y * v.x;
            return LVector<T> (a, b, c);
        }
*/
        bool equals(T x_, T y_)
        {
            return (x==x_)&&(y==y_);
        }
        bool equals(const LVector<T>& v)
        {
            return (x==v.x)&&(y==v.y);
        }
        bool operator==(const LVector<T>& v)
        {
            return (x==v.x)&&(y==v.y);
        }
        bool operator!=(const LVector<T>& v)
        {
            return (x!=v.x)||(y!=v.y);
        }

        LVector<T> operator+(const LVector<T>& v)
        {
            return LVector<T>(x+v.x, y+v.y);
        }
        LVector<T> operator-(const LVector<T>& v)
        {
            return LVector<T>(x-v.x, y-v.y);
        }
        LVector<T> operator*(double factor)
        {
            return LVector<T>(T(x*factor), T(y*factor));
        }
        LVector<T> operator/(T factor)
        {
            return LVector<T>(x/factor, y/factor);
        }

        LVector<T>& operator+=(const LVector<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }
        LVector<T>& operator-=(const LVector<T>& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        LVector<T>& operator*=(double scalar)
        {
            x = T(x*scalar);
            y = T(y*scalar);
            return (*this);
        }
        LVector<T>& operator/=(T factor)
        {
            x /= factor;
            y /= factor;
            return *this;
        }

        template< typename C >
        operator LVector<C>()
        {
            return LVector<C>((C)x, (C)y);
        }

        #ifdef LSTRING_H
        operator const char*()
        {
            return LString(x) + ", " + y;
        }
        #endif

        template < typename C >
        size_t findClosest(std::vector< LVector<C> >& vectorList)
        {
            if (!vectorList.size()) return size_t(-1);
            size_t rt = 0;
            double distSqr = this->distanceSquared(vectorList.at(0));
            static double newDistSqr;
            range(i, 1, vectorList.size())
            {
                newDistSqr = this->distanceSquared(vectorList.at(i));
                if (newDistSqr < distSqr)
                {
                    distSqr = newDistSqr;
                    rt = i;
                }
            }
            return rt;
        }
};


typedef LVector<double> D_Vector;
typedef LVector<int> I_Vector;

#define ORIGIN (I_Vector())

#endif // LVECTOR_H
