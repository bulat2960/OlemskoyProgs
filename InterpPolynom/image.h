#ifndef IMAGE_H
#define IMAGE_H

#include <QtWidgets>

#include "computer.h"

class Image : public QWidget
{
    Q_OBJECT

    int zoom = 1;
    int prec = 100;

    QCheckBox* functCheckBox;
    QCheckBox* lagrCheckBox1;
    QCheckBox* lagrCheckBox2;
    QCheckBox* newtonCheckBox1;
    QCheckBox* newtonCheckBox2;

    QTextBrowser* browser;
    QTextBrowser* dataBrowser;
    QTableWidget* table;

    Computer* computer;

    QVector<QPointF> fData;
    QVector<QPointF> lagrData1;
    QVector<QPointF> lagrData2;
    QVector<QPointF> newtonData1;
    QVector<QPointF> newtonData2;

    double lagrNormalMax;
    double lagrOptimalMax;
    double newtonNormalMax;
    double newtonOptimalMax;
public:
    Image(QCheckBox* fbox, QCheckBox* box1, QCheckBox* box2, QCheckBox* box3, QCheckBox* box4, QTextBrowser* browser,
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
