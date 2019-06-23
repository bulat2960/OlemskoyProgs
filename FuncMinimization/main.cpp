#include "vector.h"
#include "matrix.h"

#include <QDebug>

const int DIMENSION = 3;
const double EPSILON = 0.000001;

enum class Type
{
    GRAD,
    COORD
};

typedef Vector<double> Vec;
typedef Matrix<double> Mat;

// ------------------------------------------ //

Vec grad(Mat A, Vec x, Vec b)
{
    return A * x + b;
}

Vec coord(int elem)
{
    Vec result(DIMENSION);
    result[elem % 3] = 1;
    return result;
}

double calculateStep(Vec q, Mat A, Vec x, Vec b)
{
    double num = q * (A * x + b);
    double den = q * (A * q);
    return -num / den;
}

Vec nextApprox(Vec x, Vec grad, double step)
{
    return x + grad * step;
}

double norm(Vec v)
{
    double error = 0.0;
    for (int i = 0; i < DIMENSION; i++)
    {
        error += pow(v[i], 2);
    }
    return sqrt(error);
}

double f(Vec v)
{
    double x = v[0];
    double y = v[1];
    double z = v[2];
    return 2 * x * x + 3.2 * y * y + 4.2 * z * z + x * y - y * z + x * z + x - 2 * y + 3 * z + 2;
}

void optim(Mat A, Vec x, Vec b, Type type)
{
    int iterations = 0;

    qDebug() << (type == Type::GRAD ? "ГРАДИЕНТНЫЙ" : "ПОКООРДИНАТНЫЙ") << "СПУСК";
    qDebug() << "Исходный вектор:" << x;

    while (true)
    {
        qDebug () << "Итерация" << (iterations + 1);
        Vec q;
        if (type == Type::GRAD)  q = grad(A, x, b);
        if (type == Type::COORD) q = coord(iterations);

        double step = calculateStep(q, A, x, b);

        x = nextApprox(x, q, step);
        qDebug() << "Приближение:" << x;
        iterations++;

        double vecNormApprox = norm(A * x + b) / A.delta();
        qDebug() << "Оценка нормы сверху:" << vecNormApprox;

        if (vecNormApprox < EPSILON)
        {
            break;
        }
    }
    qDebug() << "--------------------------";
    qDebug() << "РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ";
    qDebug() << "Минимум достигнут в:" << x;
    qDebug() << "Значение функции:" << f(x);
    qDebug() << "Итераций: " << iterations;
    qDebug() << "\n\n";
}

int main()
{
    Vec s1 = {4.0,  1.0,  1.0};
    Vec s2 = {1.0,  6.4, -1.0};
    Vec s3 = {1.0, -1.0,  8.4};

    Mat A = {s1, s2, s3};
    Vec x = {1.0,  1.0, 1.0};
    Vec b = {1.0, -2.0, 3.0};

    optim(A, x, b, Type::GRAD);
    optim(A, x, b, Type::COORD);

    return 0;
}
