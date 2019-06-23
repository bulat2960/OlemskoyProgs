#ifndef IMAGE_H
#define IMAGE_H

#include <QtWidgets>

#include "computer.h"

class Image : public QWidget
{
    Q_OBJECT

private:
    int zoom = 1;
    int prec = 100;

private:
    QCheckBox* sp10button;
    QCheckBox* sp21button;
    QCheckBox* sp32button;

    QTextBrowser* polyBrowser;
    QTextBrowser* dataBrowser;
    QTableWidget* table;

    Computer* computer;

    QVector<QVector<QPointF>> sp10data;
    QVector<QVector<QPointF>> sp21data;
    QVector<QVector<QPointF>> sp32data;

    QVector<QPointF> funcData;

    double sp10max;
    double sp21max;
    double sp32max;

public:
    Image(QCheckBox* sp10button, QCheckBox* sp21button, QCheckBox* sp32button, QTextBrowser* polyBrowser, QTextBrowser* dataBrowser,
          QTableWidget* table, Computer* computer);

    void updateBrowserData();

public slots:
    void paintEvent(QPaintEvent* event);

    void fillErrorTable();
    void clearErrorTable();

    void doWork();

    void changeZoom(int value);
    void changePrec(int value);
};

#endif // IMAGE_H
