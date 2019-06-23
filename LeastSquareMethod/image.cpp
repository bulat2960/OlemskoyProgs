#include "image.h"

Image::Image(QCheckBox *funcCb, QCheckBox* cb1, QCheckBox* cb2, QTextBrowser* browser, QTextBrowser* dataBrowser,
             QTableWidget* table, Computer* computer, QWidget *parent) : QWidget(parent)
{
    connect(funcCb, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(cb1, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(cb2, SIGNAL(clicked()), this, SLOT(repaint()));

    this->browser = browser;
    this->dataBrowser = dataBrowser;
    this->table = table;

    this->funcCb = funcCb;
    this->cb1 = cb1;
    this->cb2 = cb2;

    this->computer = computer;

    connect(computer, SIGNAL(finished()), this, SLOT(doWork()));

    QStringList headerLabels;
    headerLabels << "Количество узлов";
    headerLabels << "Количество проверочных точек";
    headerLabels << "Rmax нормального уравнения";
    headerLabels << "Rmax ортогонального полинома";

    table->setHorizontalHeaderLabels(headerLabels);
}

void Image::doWork()
{
    Polynom p1 = computer->p1;
    Polynom p2 = computer->p2;

    double step = (computer->b - computer->a) * 1.0 / prec;

    funcData.clear();
    p1data.clear();
    p2data.clear();

    p1max = -1;
    p2max = -1;

    for (int i = 0; i < prec; i++)
    {
        double currentPoint = computer->a + i * step;
        funcData.push_back(QPointF(currentPoint, computer->func(currentPoint)));
        p1data.push_back(QPointF(currentPoint, p1.value(currentPoint)));
        p2data.push_back(QPointF(currentPoint, p2.value(currentPoint)));
    }

    p1max = computer->error1;
    p2max = computer->error2;

    repaint();
    updateBrowserData();
}

void Image::fillErrorTable()
{
    QTableWidgetItem* data1 = new QTableWidgetItem(QString::number(computer->n));
    QTableWidgetItem* data2 = new QTableWidgetItem(QString::number(prec));
    QTableWidgetItem* data3 = new QTableWidgetItem(QString::number(p1max));
    QTableWidgetItem* data4 = new QTableWidgetItem(QString::number(p2max));

    QVector<QTableWidgetItem*> dataItems = {data1, data2, data3, data4};

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
    headerLabels << "Rmax нормального уравнения";
    headerLabels << "Rmax ортогонального полинома";

    table->setHorizontalHeaderLabels(headerLabels);
}

void Image::updateBrowserData()
{
    browser->setText("Нормальное уравнение: " + computer->p1 + "\n\n" + "Ортогональный полином" + computer->p2);
    dataBrowser->setText("Рассматриваемый интервал: [" + QString::number(computer->a) + ", " + QString::number(computer->b) + "] \n\n" +
                         "Количество узлов: " + QString::number(computer->n) + "\n\n" +
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

    for (int i = 0; i < p1data.size(); i++)
    {
        if (funcCb->isChecked())
        {
            pen.setColor(Qt::blue);
            painter.setPen(pen);
            painter.drawPoint(QPointF(funcData[i].x(), -funcData[i].y()) * zoom);
        }

        if (cb1->isChecked())
        {
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawPoint(QPointF(p1data[i].x(), -p1data[i].y()) * zoom);
        }

        if (cb2->isChecked())
        {
            pen.setColor(Qt::black);
            painter.setPen(pen);
            painter.drawPoint(QPointF(p2data[i].x(), -p2data[i].y()) * zoom);
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
