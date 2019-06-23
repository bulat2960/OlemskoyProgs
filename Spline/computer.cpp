#include "computer.h"

Computer::Computer(int a, int b, int n)
{
    this->a = a;
    this->b = b;
    this->n = n;
}

QVector<double> Computer::points()
{
    QVector<double> result;

    double step = (b - a) * 1.0 / n;
    for (int i = 0; i < (n + 1); i++)
    {
        result.push_back(a + i * step);
    }

    for (int i = 0; i < result.size(); i++)
    {
        if (fabs(result[i]) < 0.00001)
        {
            result[i] = 0;
        }
    }

    return result;
}

double Computer::func(double x)
{
    //return sin(50 * x * 3.1415926 / 180);
    //return 0.25 * x * x;
    return 1 / (1 + 100 * pow(x, 2));
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

void Computer::calculateSplineCoefs10()
{
    QVector<double> pts = points();
    QVector<double> fnc = funcValues(pts);

    p10.clear();

    for (int i = 0; i < n; i++)
    {
        Matrix splineMatrix(2);

        splineMatrix[0][0] = pts[i];
        splineMatrix[1][0] = pts[i + 1];
        splineMatrix[0][1] = 1;
        splineMatrix[1][1] = 1;

        splineMatrix.getInv();

        QVector<double> funcValues = {fnc[i], fnc[i + 1]};

        QVector<double> res = splineMatrix * funcValues;

        Polynom p({res[0], res[1]});

        p10.push_back(p);
    }
}

void Computer::calculateSplineCoefs21()
{
    QVector<double> pts = points();
    QVector<double> fnc = funcValues(pts);

    p21.clear();

    Matrix splineMatrix(n * 3);

    for (int i = 0; i < n; i++)
    {
        int mult = i * 3;

        splineMatrix[mult][mult] = pts[i] * pts[i];
        splineMatrix[mult][mult + 1] = pts[i];
        splineMatrix[mult][mult + 2] = 1;

        splineMatrix[mult + 1][mult] = pts[i + 1] * pts[i + 1];
        splineMatrix[mult + 1][mult + 1] = pts[i + 1];
        splineMatrix[mult + 1][mult + 2] = 1;

        splineMatrix[mult + 2][mult] = 2 * pts[i + 1];
        splineMatrix[mult + 2][mult + 1] = 1;
        if (i > 0)
        {
            splineMatrix[mult - 1][mult] = -2 * pts[i];
            splineMatrix[mult - 1][mult + 1] = -1;
        }
    }

    qDebug() << "Before:";
    splineMatrix.print();

    splineMatrix.getInv();

    qDebug() << "After:";
    splineMatrix.print();

    QVector<double> funcValues;
    for (int i = 0; i < n; i++)
    {
        funcValues.push_back(fnc[i]);
        funcValues.push_back(fnc[i + 1]);
        funcValues.push_back(0);
    }
    QVector<double> res = splineMatrix * funcValues;

    for (int i = 0; i < n; i++)
    {
        Polynom p({res[i * 3], res[i * 3 + 1], res[i * 3 + 2]});
        p21.push_back(p);
    }
}

void Computer::calculateSplineCoefs32()
{
    QVector<double> pts = points();
    QVector<double> fnc = funcValues(pts);

    p32.clear();

    QVector<double> hValues;
    for (int i = 0; i < pts.size() - 1; i++)
    {
        hValues.push_back(pts[i + 1] - pts[i]);
    }

    QVector<double> gammaValues;
    for (int i = 1; i < pts.size() - 1; i++)
    {
        gammaValues.push_back(6 * ((fnc[i + 1] - fnc[i]) / hValues[i] - (fnc[i] - fnc[i - 1]) / hValues[i - 1]));
    }

    Matrix H(hValues.size() - 1);
    for (int i = 0; i < hValues.size() - 1; i++)
    {
        H[i][i] = 2 * (hValues[i] + hValues[i + 1]);
        if (i < hValues.size() - 2)
        {
            H[i][i + 1] = hValues[i + 1];
            H[i + 1][i] = hValues[i + 1];
        }
    }
    H.getInv();

    QVector<double> yDeriv2 = H * gammaValues;
    yDeriv2.push_back(0);
    yDeriv2.push_front(0);

    QVector<double> yDeriv1;
    for (int i = 0; i < pts.size() - 1; i++)
    {
        yDeriv1.push_back((fnc[i + 1] - fnc[i]) / hValues[i] - yDeriv2[i + 1] * hValues[i] / 6 - yDeriv2[i] * hValues[i] / 3);
    }

    for (int i = 0; i < pts.size() - 1; i++)
    {
        Polynom deg3part({1, -3 * pts[i], 3 * pts[i] * pts[i], -pts[i] * pts[i] * pts[i]});
        deg3part = deg3part * ((yDeriv2[i + 1] - yDeriv2[i]) / (6 * hValues[i]));

        Polynom deg2part({1, -2 * pts[i], pts[i] * pts[i]});
        deg2part = deg2part * (yDeriv2[i] / 2);

        Polynom deg1part({1, -pts[i]});
        deg1part = deg1part * yDeriv1[i];

        Polynom deg0part({fnc[i]});

        Polynom final(4);
        final = final + deg3part + deg2part + deg1part + deg0part;

        p32.push_back(final);
    }
}

void Computer::calculate()
{
    calculateSplineCoefs10();
    calculateSplineCoefs21();
    calculateSplineCoefs32();
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
