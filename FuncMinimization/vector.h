#ifndef VECTOR_H
#define VECTOR_H

#include <QVector>

template<typename T>
class Vector : public QVector<T>
{
public:
    Vector() : QVector<T>() {}
    Vector(int size) : QVector<T>(size) {}
    Vector(const QVector<T>& data) : QVector<T>(data) {}
    Vector(std::initializer_list<T> args) : QVector<T>(args) {}

    Vector<T> operator +(const Vector<T>& v)
    {
        Vector<T> result = *this;
        for (int i = 0; i < v.size(); i++)
        {
            result[i] += v[i];
        }
        return result;
    }

    Vector<T> operator -(const Vector<T>& v)
    {
        Vector<T> result = *this;
        for (int i = 0; i < v.size(); i++)
        {
            result[i] -= v[i];
        }
        return result;
    }

    void operator +=(const Vector<T>& v)
    {
        for (int i = 0; i < v.size(); i++)
        {
            (*this)[i] += v[i];
        }
    }

    void operator -=(const Vector<T>& v)
    {
        for (int i = 0; i < v.size(); i++)
        {
            (*this)[i] -= v[i];
        }
    }

    Vector<T> operator-()
    {
        Vector<T> copy = (*this);
        for (int i = 0; i < copy.size(); i++)
        {
            copy[i] *= -1;
        }
        return copy;
    }

    double operator *(const Vector<T>& v)
    {
        double sum = 0.0;
        for (int i = 0; i < v.size(); i++)
        {
            sum += (*this)[i] * v[i];
        }
        return sum;
    }

    Vector<T> operator *(double value)
    {
        Vector<T> copy = (*this);
        for (int i = 0; i < copy.size(); i++)
        {
            copy[i] *= value;
        }
        return copy;
    }
};

#endif // VECTOR_H
