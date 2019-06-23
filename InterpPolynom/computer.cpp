#include "computer.h"

#include <QDebug>

Computer::Computer(int a, int b, int n)
{
    this->a = a;
    this->b = b;
    this->n = n;
}

double Computer::func(double x)
{
    return pow(x, 3) - exp(x) + 1;
}

void Computer::getNormalPoints()
{
    QVector<double> result;

    double step = (b - a) * 1.0 / n;
    for (int i = 0; i < (n + 1); i++)
    {
        result.push_back(a + i * step);
    }

    normalPts = result;
}

void Computer::getOptimalPoints()
{
    QVector<double> result;

    for (int i = 0; i < (n + 1); i++)
    {
        double param = (2 * i + 1) * 1.0 / (2 * (n + 1));
        double cosI = cos(param * 3.1415926);
        double x = 0.5 * ((b - a) * cosI + (b + a));
        result.push_back(x);
    }

    optimalPts = result;
}

Polynom Computer::lagrMultiplier(int index, QVector<double> pts)
{
    Polynom numerator = {1};
    double denominator = 1.0;

    for (int i = 0; i < pts.size(); i++)
    {
        Polynom num = {1, -pts[i]};
        double den = pts[index - 1] - pts[i];

        if (den == 0.0)
        {
            continue;
        }
        numerator = numerator * num;
        denominator *= den;
    }

    Polynom result = numerator / denominator;

    return result;
}

QVector<Polynom> Computer::lagrMultArray(QVector<double> pts)
{
    QVector<Polynom> result;
    for (int i = 0; i < pts.size(); i++)
    {
        Polynom mult = lagrMultiplier(i + 1, pts);
        result.push_back(mult);
    }
    return result;
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

Polynom Computer::calculateLagrPolynom(QVector<Polynom> mults, QVector<double> funcValues)
{
    Polynom p(funcValues.size() - 1);

    for (int i = 0; i < funcValues.size(); i++)
    {
        p = p + mults[i] * funcValues[i];
    }

    return p;
}

double Computer::separatedDifference(int degree, QVector<double> pts)
{
    if (degree == 1)
    {
        return 0;
    }

    // Иначе вычисляем разделенную разность порядка degree - 1

    double result = 0.0;
    for (int i = 0; i < degree; i++)
    {
        double g = 1;
        for (int j = 0; j < degree; j++)
        {
            double denominator = pts[i] - pts[j];

            if (denominator == 0.0)
            {
                continue;
            }

            g /= denominator;
        }
        result += g * func(pts[i]);
    }

    return result;
}

QVector<double> Computer::separatedDiffArray(QVector<double> pts)
{
    QVector<double> result;
    for (int i = 0; i < pts.size(); i++)
    {
        double separatedDiff = separatedDifference(i + 1, pts); // разделенная разность i-ой степени
        result.push_back(separatedDiff);
    }

    return result;
}

Polynom Computer::calculateNewtonPolynom(QVector<double> separatedDiffArray, QVector<double> pts)
{
    Polynom p(separatedDiffArray.size());

    for (int i = separatedDiffArray.size() - 1; i >= 0; i--)
    {
        Polynom temp = {1};
        for (int j = 0; j < i; j++)
        {
            Polynom piece = {1, -pts[j]};
            temp = temp * piece;
        }

        if (i == 0)
        {
            p = p + temp * func(pts[0]);
        }
        else
        {
            p = p + temp * separatedDiffArray[i];
        }
    }

    return p;
}

void Computer::calculate()
{
    getNormalPoints();
    getOptimalPoints();

    QVector<Polynom> normalLagrMultArray = lagrMultArray(normalPts);
    QVector<Polynom> optimalLagrMultArray = lagrMultArray(optimalPts);

    QVector<double> normalSeparatedDiffArray = separatedDiffArray(normalPts);
    QVector<double> optimalSeparatedDiffArray = separatedDiffArray(optimalPts);

    QVector<double> normalFuncValues = funcValues(normalPts);
    QVector<double> optimalFuncValues = funcValues(optimalPts);

    normalLagr = calculateLagrPolynom(normalLagrMultArray, normalFuncValues);
    optimalLagr = calculateLagrPolynom(optimalLagrMultArray, optimalFuncValues);

    normalNewton = calculateNewtonPolynom(normalSeparatedDiffArray, normalPts);
    optimalNewton = calculateNewtonPolynom(optimalSeparatedDiffArray, optimalPts);

    emit finished();
}

Polynom Computer::getNormal() const
{
    return normalLagr;
}

Polynom Computer::getOptimal() const
{
    return optimalLagr;
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

Computer::~Computer()
{

}
