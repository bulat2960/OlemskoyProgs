#ifndef COMPUTER_H
#define COMPUTER_H

#include "matrix.h"
#include "polynom.h"

#include <QVector>
#include <QObject>

class Computer : public QObject
{
    Q_OBJECT

public:
    QVector<Polynom> p10;
    QVector<Polynom> p21;
    QVector<Polynom> p32;

    int a;
    int b;
    int n;
public:
    Computer(int a, int b, int n);

    double func(double x);
    QVector<double> funcValues(QVector<double> pts);

    QVector<double> points();

    void calculateSplineCoefs10();
    void calculateSplineCoefs21();
    void calculateSplineCoefs32();

public slots:
    void setLeftBorder(int a);
    void setRightBorder(int b);
    void setPointNumber(int n);

    void calculate();

signals:
    void finished();
};

#endif // COMPUTER_H
