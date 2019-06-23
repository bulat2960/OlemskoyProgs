#include "polynom.h"

Polynom::Polynom()
{

}

Polynom::Polynom(int size)
{
    m_data = QVector<double>(size);
}

Polynom::Polynom(const Polynom& other)
{
    m_data = other.m_data;
}

Polynom::Polynom(std::initializer_list<double> list)
{
    m_data = list;
}

int Polynom::size() const
{
    return m_data.size();
}

void Polynom::operator =(const Polynom& other)
{
    m_data = other.m_data;
}

#include <QDebug>

Polynom Polynom::operator +(const Polynom& other)
{
    int thisSize = this->size();
    int otherSize = other.size();

    int diff = thisSize - otherSize;

    Polynom result = (diff > 0) ? *this : other;
    Polynom plus = (diff > 0) ? other : *this;

    for (int i = 0; i < plus.size(); i++)
    {
        result.m_data[i + abs(diff)] += plus.m_data[i];
    }

    return result;
}

Polynom Polynom::operator *(double value)
{
    Polynom result = *this;

    for (int i = 0; i < result.size(); i++)
    {
        result.m_data[i] *= value;
    }

    return result;
}

Polynom Polynom::operator /(double value)
{
    Polynom result = *this;

    for (int i = 0; i < result.size(); i++)
    {
        result.m_data[i] /= value;
    }

    return result;
}

Polynom Polynom::operator *(const Polynom& other)
{
    Polynom result(this->size());

    for (int i = 0; i < other.size(); i++)
    {
        Polynom temp = *this * other.m_data[i];
        result = result + temp;

        if (i != other.size() - 1)
        {
            result.m_data.push_back(0);
        }
    }

    return result;
}

QVector<double> Polynom::data() const
{
    return m_data;
}

double Polynom::value(double value)
{
    double result = 0.0;

    for (int i = 0; i < m_data.size(); i++)
    {
        result += m_data[i] * pow(value, m_data.size() - i - 1);
    }

    return result;
}
