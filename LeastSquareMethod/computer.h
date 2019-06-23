#ifndef COMPUTER_H
#define COMPUTER_H

#include "polynom.h"
#include "matrix.h"

#include <QObject>
#include <QVector>

class Computer : public QObject
{
    Q_OBJECT
public:
    Polynom p1;
    Polynom p2;

    int a;
    int b;
    int n;
    int m;

    double E;

    double error1;
    double error2;
public:
    Computer(int a, int b, int n, int m);

    double func(double x);
    double funcWithError(double x);
    QVector<double> funcValues(QVector<double> pts);
    QVector<double> funcValuesWithError(QVector<double> pts);

    QVector<double> points();

    double getAlpha(QVector<double> pts, Polynom q);
    double getBeta(QVector<double> pts, Polynom current, Polynom prev);

    void calculateNormalPolynom();
    void calculateOrtogPolynom();

public slots:
    void setLeftBorder(int a);
    void setRightBorder(int b);
    void setPointNumber(int n);
    void setEquationNumber(int m);

    void calculate();

signals:
    void finished();
};

#endif // COMPUTER_H
