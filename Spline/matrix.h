#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <QVector>

#include <QDebug>

class Matrix
{
private:
    QVector<QVector<double>> m_data;
    QVector<QVector<double>> m_inv;
public:
    Matrix() {}
    Matrix(int size)
    {
        for (int i = 0; i < size; i++)
        {
            m_data.push_back(QVector<double>(size));
            m_inv.push_back(QVector<double>(size));
            m_inv[i][i] = 1;
        }
    }

    Matrix(QVector<QVector<double>> data)
    {
        m_data = data;
        for (int i = 0; i < m_data.size(); i++)
        {
            m_inv.push_back(QVector<double>(size()));
            m_inv[i][i] = 1;
        }
    }

    Matrix(const Matrix& matrix)
    {
        m_data = matrix.m_data;
    }

    void init(int size)
    {
        m_data.clear();
        m_inv.clear();
        for (int i = 0; i < size; i++)
        {
            m_data.push_back(QVector<double>(size));
            m_inv.push_back(QVector<double>(size));
            m_inv[i][i] = 1;
        }
    }

    QVector<double> operator *(const QVector<double>& vector)
    {
        QVector<double> result;

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

    Matrix operator *(const Matrix& matrix)
    {
        Matrix result(size());

        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                double res = 0;
                for (int k = 0; k < matrix.size(); k++)
                {
                    res += m_data[i][k] * matrix.m_data[k][j];
                }
                result[i][j] = res;
            }
        }

        return result;
    }

    void operator /=(const double value)
    {
        for (int i = 0; i < m_data.size(); i++)
        {
            for (int j = 0; j < m_data.size(); j++)
            {
                m_data[i][j] /= value;
            }
        }
    }

    QVector<double>& operator [](int index)
    {
        return m_data[index];
    }

    int size() const
    {
        return m_data.size();
    }

    void print()
    {
        qDebug() << "Left";
        for (int i = 0; i < m_data.size(); i++)
        {
            QString s;
            for (int j = 0; j < m_data[i].size(); j++)
            {
                s += QString::number(m_data[i][j], 'd', 3) + " ";
            }
            qDebug().noquote() << s;
        }

        qDebug() << "Right";
        for (int i = 0; i < m_inv.size(); i++)
        {
            QString s;
            for (int j = 0; j < m_inv[i].size(); j++)
            {
                s += QString::number(m_inv[i][j], 'd', 3) + " ";
            }
            qDebug().noquote() << s;
        }
    }

    int findCorrectString(int i)
    {
        for (int j = i + 1; j < size(); j++)
        {
            if (m_data[j][i] != 0.0)
            {
                return j;
            }
        }
        return -1;
    }

    void transpose()
    {
        for (int i = 0; i < size(); i++)
        {
            for (int j = i + 1; j < size(); j++)
            {
                std::swap(m_data[i][j], m_data[j][i]);
            }
        }

        for (int i = 0; i < size(); i++)
        {
            for (int j = i + 1; j < size(); j++)
            {
                std::swap(m_inv[i][j], m_inv[j][i]);
            }
        }
    }

    void zeroBottomTriangle()
    {
        for (int i = 0; i < size(); i++)
        {
            double coef = 0.0;
            for (int j = i + 1; j < size(); j++)
            {
                if (m_data[i][i] == 0.0)
                {
                    int x = findCorrectString(i);
                    if (x == -1)
                    {
                        break;
                    }

                    for (int k = 0; k < size(); k++)
                    {
                        std::swap(m_data[i][k], m_data[x][k]);
                        std::swap(m_inv[i][k], m_inv[x][k]);
                    }
                }
                coef = m_data[j][i] / m_data[i][i];
                for (int k = 0; k < size(); k++)
                {
                    m_data[j][k] -= m_data[i][k] * coef;
                    m_inv[j][k] -= m_inv[i][k] * coef;
                }
            }
        }
    }

    void zeroTopTriangle()
    {
        for (int i = size() - 1; i >= 0; i--)
        {
            double coef = 0.0;
            for (int j = i - 1; j >= 0; j--)
            {
                coef = m_data[j][i] / m_data[i][i];
                for (int k = 0; k < size(); k++)
                {
                    m_data[j][k] -= m_data[i][k] * coef;
                    m_inv[j][k] -= m_inv[i][k] * coef;
                }
            }
        }
    }

    void divDiag()
    {
        for (int i = 0; i < size(); i++)
        {
            double keyElem = m_data[i][i];
            for (int j = 0; j < size(); j++)
            {
                m_inv[i][j] /= keyElem;
                m_data[i][j] /= keyElem;
            }
        }
    }

    void getInv()
    {
        zeroBottomTriangle();
        qDebug() << "AFTER ZERO BOT:";
        print();
        zeroTopTriangle();
        qDebug() << "AFTER ZERO TOP:";
        print();
        divDiag();
        m_data = m_inv;
    }
};


#endif // MATRIX_H
