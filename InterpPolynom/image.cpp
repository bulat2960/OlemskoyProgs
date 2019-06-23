#include "image.h"

Image::Image(QCheckBox *fbox, QCheckBox* box1, QCheckBox* box2, QCheckBox* box3, QCheckBox* box4,
             QTextBrowser* browser, QTextBrowser* dataBrowser, QTableWidget* table, Computer* computer, QWidget *parent) : QWidget(parent)
{
    functCheckBox = fbox;
    lagrCheckBox1 = box1;
    lagrCheckBox2 = box2;
    newtonCheckBox1 = box3;
    newtonCheckBox2 = box4;

    connect(functCheckBox, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(lagrCheckBox1, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(lagrCheckBox2, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(newtonCheckBox1, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(newtonCheckBox2, SIGNAL(clicked()), this, SLOT(repaint()));

    this->browser = browser;
    this->dataBrowser = dataBrowser;
    this->table = table;

    this->computer = computer;

    connect(computer, SIGNAL(finished()), this, SLOT(doWork()));

    QStringList headerLabels;
    headerLabels << "Количество узлов";
    headerLabels << "Количество проверочных точек";
    headerLabels << "Rmax равноотстоящих узлов Лагранжа";
    headerLabels << "Rmax оптимальных узлов Лагранжа";
    headerLabels << "Rmax равноотстоящих узлов Ньютона";
    headerLabels << "Rmax оптимальных узлов Ньютона";

    table->setHorizontalHeaderLabels(headerLabels);
}

#include <QDebug>

void Image::doWork()
{
    Polynom p1 = computer->normalLagr;
    Polynom p2 = computer->optimalLagr;
    Polynom p3 = computer->normalNewton;
    Polynom p4 = computer->optimalNewton;

    double step = (computer->b - computer->a) * 1.0 / prec;

    fData.clear();
    lagrData1.clear();
    lagrData2.clear();
    newtonData1.clear();
    newtonData2.clear();

    lagrNormalMax = -1;
    lagrOptimalMax = -1;
    newtonNormalMax = -1;
    newtonOptimalMax = -1;

    for (int i = 0; i < prec; i++)
    {
        double currentPoint = computer->a + i * step;
        fData.push_back(QPointF(currentPoint, computer->func(currentPoint)));
        lagrData1.push_back(QPointF(currentPoint, p1.value(currentPoint)));
        lagrData2.push_back(QPointF(currentPoint, p2.value(currentPoint)));
        newtonData1.push_back(QPointF(currentPoint, p3.value(currentPoint)));
        newtonData2.push_back(QPointF(currentPoint, p4.value(currentPoint)));

        double lagrNormalTemp = fabs(fData.back().y() - lagrData1.back().y());
        lagrNormalMax = (lagrNormalTemp > lagrNormalMax) ? lagrNormalTemp : lagrNormalMax;

        double lagrOptimalTemp = fabs(fData.back().y() - lagrData2.back().y());
        lagrOptimalMax = (lagrOptimalTemp > lagrOptimalMax) ? lagrOptimalTemp : lagrOptimalMax;

        double newtonNormalTemp = fabs(fData.back().y() - newtonData1.back().y());
        newtonNormalMax = (newtonNormalTemp > newtonNormalMax) ? newtonNormalTemp : newtonNormalMax;

        double newtonOptimalTemp = fabs(fData.back().y() - newtonData2.back().y());
        newtonOptimalMax = (newtonOptimalTemp > newtonOptimalMax) ? newtonOptimalTemp : newtonOptimalMax;
    }

    repaint();
    updateBrowserData();
}

void Image::fillErrorTable()
{
    QTableWidgetItem* data1 = new QTableWidgetItem(QString::number(computer->n));
    QTableWidgetItem* data2 = new QTableWidgetItem(QString::number(prec));
    QTableWidgetItem* data3 = new QTableWidgetItem(QString::number(lagrNormalMax));
    QTableWidgetItem* data4 = new QTableWidgetItem(QString::number(lagrOptimalMax));
    QTableWidgetItem* data5 = new QTableWidgetItem(QString::number(newtonNormalMax));
    QTableWidgetItem* data6 = new QTableWidgetItem(QString::number(newtonOptimalMax));

    QVector<QTableWidgetItem*> dataItems = {data1, data2, data3, data4, data5, data6};

    table->setRowCount(table->rowCount() + 1);

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setItem(table->rowCount() - 1, i, dataItems[i]);
    }

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setColumnWidth(i, 1850 / table->columnCount());
    }
}

void Image::clearErrorTable()
{
    table->clear();
    table->setRowCount(0);

    QStringList headerLabels;
    headerLabels << "Количество узлов";
    headerLabels << "Количество проверочных точек";
    headerLabels << "Rmax равноотстоящих узлов Лагранжа";
    headerLabels << "Rmax оптимальных узлов Лагранжа";
    headerLabels << "Rmax равноотстоящих узлов Ньютона";
    headerLabels << "Rmax оптимальных узлов Ньютона";

    table->setHorizontalHeaderLabels(headerLabels);
}

void Image::updateBrowserData()
{
    browser->setText("Лагранж: " + computer->normalLagr + "\n\n" + "Оптимальный Лагранж: " + computer->optimalLagr+ "\n\n" +
                     "Ньютон: " + computer->normalNewton + "\n\n" + "Оптимальный Ньютон: " + computer->optimalNewton);
    dataBrowser->setText("Рассматриваемый интервал: [" + QString::number(computer->a) + ", " + QString::number(computer->b) + "] \n\n" +
                         "Количество точек аппроксимации: " + QString::number(computer->n) + "\n\n" +
                         "Зум: " + QString::number(zoom) + "\n\n" + "Точек на одном графике функции:" + QString::number(prec));
}

void Image::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QRect geometry = this->geometry().adjusted(-13, -13, 0, 0);
    painter.fillRect(geometry, Qt::gray);

    painter.save();
    {
        QPen pen;
        pen.setWidth(1);
        pen.setColor(Qt::black);
        painter.setPen(pen);

        QPoint leftCenter = QPoint(0, geometry.height() / 2);
        QPoint rightCenter = QPoint(geometry.width(), geometry.height() / 2);

        QPoint topCenter = QPoint(geometry.width() / 2, 0);
        QPoint bottomCenter = QPoint(geometry.width() / 2, geometry.height());

        painter.drawLine(leftCenter, rightCenter);
        painter.drawLine(topCenter, bottomCenter);
    }
    painter.restore();

    painter.translate(geometry.center());

    QPen pen;
    pen.setWidth(3);

    for (int i = 0; i < fData.size(); i++)
    {
        if (functCheckBox->isChecked())
        {
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawPoint(QPointF(fData[i].x(), -fData[i].y()) * zoom);
        }

        if (lagrCheckBox1->isChecked())
        {
            pen.setColor(Qt::black);
            painter.setPen(pen);
            painter.drawPoint(QPointF(lagrData1[i].x(), -lagrData1[i].y()) * zoom);
        }

        if (lagrCheckBox2->isChecked())
        {
            pen.setColor(Qt::blue);
            painter.setPen(pen);
            painter.drawPoint(QPointF(lagrData2[i].x(), -lagrData2[i].y()) * zoom);
        }

        if (newtonCheckBox1->isChecked())
        {
            pen.setColor(Qt::green);
            painter.setPen(pen);
            painter.drawPoint(QPointF(newtonData1[i].x(), -newtonData1[i].y()) * zoom);
        }

        if (newtonCheckBox2->isChecked())
        {
            pen.setColor(Qt::magenta);
            painter.setPen(pen);
            painter.drawPoint(QPointF(newtonData2[i].x(), -newtonData2[i].y()) * zoom);
        }
    }
}

void Image::changeZoom(int value)
{
    zoom = value;
    repaint();
    updateBrowserData();
}

void Image::changePrec(int value)
{
    prec = value;
    doWork();
}

Image::~Image()
{

}
