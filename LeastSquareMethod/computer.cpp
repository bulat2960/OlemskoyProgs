#include "computer.h"

Computer::Computer(int a, int b, int n, int m)
{
    this->a = a;
    this->b = b;
    this->n = n;
    this->m = m;

    E = 2 * (double((rand() % 21) - 10)) / 100;
}

QVector<double> Computer::points()
{
    QVector<double> result;

    double step = (b - a) * 1.0 / m;
    for (int i = 0; i < m; i++)
    {
        result.push_back(a + i * step);
    }

    for (int i = 0; i < result.size(); i++)
    {
        if (fabs(result[i]) < 0.00000001)
        {
            result[i] = 0;
        }
    }

    return result;
}

double Computer::func(double x)
{
    //return sin(50 * x * 3.1415926 / 180);
    //return pow(x, 3) - exp(x) + 1;

    double f = pow(x, 3) - exp(x) + 1;
    return f;
}

double Computer::funcWithError(double x)
{
    double f = pow(x, 3) - exp(x) + 1;
    return f * (1 + E);
}


QVector<double> Computer::funcValues(QVector<double> pts)
{
    QVector<double> result;
    for (int i = 0; i < pts.size(); i++)
    {
        double value = func(pts[i]);
        result.push_back(value);
    }
    return result;
}

QVector<double> Computer::funcValuesWithError(QVector<double> pts)
{
    QVector<double> result;
    for (int i = 0; i < pts.size(); i++)
    {
        double value = funcWithError(pts[i]);
        result.push_back(value);
    }
    return result;
}

void Computer::calculateNormalPolynom()
{
    QVector<double> pts = points();
    QVector<double> funcs = funcValuesWithError(pts);

    Matrix mat(n + 1);
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            double sum = 0.0;
            for (int k = 0; k < m; k++)
            {
                sum += pow(pts[k], i + j);
            }
            mat[i][j] = sum;
        }
    }

    QVector<double> v(n + 1);
    for (int i = 0; i < n + 1; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < m; j++)
        {
            sum += funcs[j] * pow(pts[j], i);
        }
        v[i] = sum;
    }

    mat.getInv();

    QVector<double> result = mat * v;
    std::reverse(result.begin(), result.end());

    p1 = Polynom(result);

    error1 = 0;
    for (int i = 0; i < n + 1; i++)
    {
        error1 += pow(func(pts[i]) - p1.value(pts[i]), 2);
    }
}

double Computer::getAlpha(QVector<double> pts, Polynom q)
{
    double numerator = 0.0;
    double denominator = 0.0;

    for (int i = 0; i < m; i++)
    {
        numerator += pts[i] * pow(q.value(pts[i]), 2);
        denominator += pow(q.value(pts[i]), 2);
    }

    return numerator / denominator;
}

double Computer::getBeta(QVector<double> pts, Polynom current, Polynom prev)
{
    double numerator = 0.0;
    double denominator = 0.0;

    for (int i = 0; i < m; i++)
    {
        numerator += pts[i] * current.value(pts[i]) * prev.value(pts[i]);
        denominator += pow(prev.value(pts[i]), 2);
    }

    return numerator / denominator;
}

void Computer::calculateOrtogPolynom()
{
    QVector<double> pts = points();
    QVector<double> funcs = funcValues(pts);

    QVector<Polynom> q;

    q.push_back(Polynom({1}));
    q.push_back(Polynom({1, getAlpha(pts, q[0])}));

    for (int i = 2; i < n + 1; i++)
    {
        Polynom b1 = q[q.size() - 1];
        Polynom b2 = q[q.size() - 2];

        Polynom s1 = b1 * Polynom({1, 0});
        Polynom s2 = b1 * getAlpha(pts, b1);
        Polynom s3 = b2 * getBeta(pts, b1, b2);

        Polynom s = s1 + s2 + s3;

        q.push_back(s);
    }

    QVector<double> qk;

    for (int i = 0; i < n + 1; i++)
    {
        double numerator = 0.0;
        double denominator = 0.0;

        for (int j = 0; j < m; j++)
        {
            numerator += q[i].value(pts[j]) * funcs[j];
            denominator += pow(q[i].value(pts[j]), 2);
        }

        qk.push_back(numerator / denominator);
    }

    Polynom x({0});

    for (int i = 0; i < n + 1; i++)
    {
        x = x + q[i] * qk[i];
    }

    p2 = x;

    error2 = 0;
    for (int i = 0; i < n + 1; i++)
    {
        error2 += pow(func(pts[i]) - p2.value(pts[i]), 2);
    }
}

void Computer::calculate()
{
    calculateNormalPolynom();
    calculateOrtogPolynom();

    emit finished();
}

void Computer::setLeftBorder(int a)
{
    this->a = a;
    calculate();
}

void Computer::setRightBorder(int b)
{
    this->b = b;
    calculate();
}

void Computer::setPointNumber(int n)
{
    this->n = n;
    calculate();
}

void Computer::setEquationNumber(int m)
{
    this->m = m;
    calculate();
}
