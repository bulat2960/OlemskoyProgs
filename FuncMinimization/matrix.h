#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include "vector.h"

#include <QDebug>

template<typename T>
class Matrix
{
private:
    Vector<Vector<T>> m_data;
public:
    Matrix() {}
    Matrix(std::initializer_list<Vector<T>> args)
    {
        m_data = args;
    }

    Vector<T> operator *(const Vector<T>& vector)
    {
        Vector<double> result;

        for (int i = 0; i < m_data.size(); i++)
        {
            double sum = 0.0;
            for (int j = 0; j < m_data[i].size(); j++)
            {
                sum += m_data[i][j] * vector[j];
            }
            result.push_back(sum);
        }
        return result;
    }

    double delta()
    {
        double delta = 10000;
        for (int i = 0; i < m_data.size(); i++)
        {
            double temp = fabs(m_data[i][i]);
            for (int j = 0; j < m_data[i].size(); j++)
            {
                if (i != j)
                {
                    temp -= fabs(m_data[i][j]);
                }
            }
            if (delta > temp)
            {
                delta = temp;
            }
        }
        return delta;
    }

    Vector<T>& operator [](int index)
    {
        return m_data[index];
    }
};


#endif // MATRIX_H
