#ifndef COMPUTER_H
#define COMPUTER_H

#include <QtMath>
#include <QVector>

#include "polynom.h"

class Computer : public QObject
{
    Q_OBJECT

public:
    int a;
    int b;
    int n;

    QVector<double> normalPts;
    QVector<double> optimalPts;

    Polynom normalLagr;
    Polynom optimalLagr;

    Polynom normalNewton;
    Polynom optimalNewton;

public:
    double func(double x);
    void getNormalPoints();
    void getOptimalPoints();

    Polynom lagrMultiplier(int index, QVector<double> pts);
    QVector<Polynom> lagrMultArray(QVector<double> pts);
    QVector<double> funcValues(QVector<double> pts);

    double separatedDifference(int degree, QVector<double> pts);
    QVector<double> separatedDiffArray(QVector<double> pts);

public:
    Computer(int a, int b, int n);

    Polynom calculateLagrPolynom(QVector<Polynom> mults, QVector<double> funcValues);
    Polynom calculateNewtonPolynom(QVector<double> separatedDiffArray, QVector<double> pts);

    Polynom getNormal() const;
    Polynom getOptimal() const;

    virtual ~Computer();
public slots:
    void setLeftBorder(int a);
    void setRightBorder(int b);
    void setPointNumber(int n);

    void calculate();

signals:
    void finished();
};

#endif // COMPUTER_H
