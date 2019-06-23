#ifndef IMAGE_H
#define IMAGE_H

#include <QtWidgets>

#include "computer.h"

class Image : public QWidget
{
    Q_OBJECT

    int zoom = 1;
    int prec = 100;

    QCheckBox* funcCb;
    QCheckBox* cb1;
    QCheckBox* cb2;

    QTextBrowser* browser;
    QTextBrowser* dataBrowser;
    QTableWidget* table;

    Computer* computer;

    QVector<QPointF> funcData;
    QVector<QPointF> p1data;
    QVector<QPointF> p2data;

    double p1max;
    double p2max;
public:
    Image(QCheckBox* funcCb, QCheckBox* cb1, QCheckBox* cb2, QTextBrowser* browser,
          QTextBrowser* dataBrowser, QTableWidget* table, Computer* computer, QWidget* parent = nullptr);

    void updateBrowserData();

    virtual ~Image();

public slots:
    void doWork();

    void fillErrorTable();
    void clearErrorTable();

    void changeZoom(int value);
    void changePrec(int value);

    void paintEvent(QPaintEvent* event);
};

#endif // IMAGE_H
