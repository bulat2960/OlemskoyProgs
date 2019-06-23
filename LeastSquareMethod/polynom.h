#ifndef POLYNOM_H
#define POLYNOM_H

#include <QVector>
#include <initializer_list>

#include <QDebug>

class Polynom
{
    QVector<double> m_data;
public:
    Polynom();
    Polynom(int size);
    Polynom(const Polynom& other);

    Polynom(std::initializer_list<double> list);
    Polynom(QVector<double> v);

    void operator =(const Polynom& other);

    Polynom operator +(const Polynom& other);
    Polynom operator *(const Polynom& other);
    Polynom operator *(double value);
    Polynom operator /(double value);

    int size() const;

    QVector<double> data() const;

    double value(double value);

    friend inline QDebug operator <<(QDebug d, const Polynom& p)
    {
        for (int i = 0; i < p.data().size(); i++)
        {
            int xDigit = p.data().size() - i - 1;
            d.nospace() << p.data()[i];
            if (xDigit > 0) d.nospace() << "x^" << xDigit;

            if (i < p.data().size() - 1)
            {
                d.nospace() << " + ";
            }
        }

        return d;
    }

    operator QString() const
    {
        QString result;

        for (int i = 0; i < data().size(); i++)
        {
            int xDigit = data().size() - i - 1;
            result += QString::number(data()[i]);
            if (xDigit > 0) result += "x^" + QString::number(xDigit);

            if (i < data().size() - 1)
            {
                result += " + ";
            }
        }

        return result;
    }
};

#endif // POLYNOM_H
