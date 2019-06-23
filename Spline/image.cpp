#include "image.h"

#include <QDebug>

Image::Image(QCheckBox* sp10button, QCheckBox* sp21button, QCheckBox* sp32button, QTextBrowser* polyBrowser, QTextBrowser* dataBrowser,
      QTableWidget* table, Computer* computer)
{
    this->sp10button = sp10button;
    this->sp21button = sp21button;
    this->sp32button = sp32button;
    this->polyBrowser = polyBrowser;
    this->dataBrowser = dataBrowser;
    this->table = table;
    this->computer = computer;

    connect(sp10button, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(sp21button, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(sp32button, SIGNAL(clicked()), this, SLOT(repaint()));

    connect(computer, SIGNAL(finished()), this, SLOT(doWork()));

    clearErrorTable();
}

void Image::fillErrorTable()
{
    QTableWidgetItem* data1 = new QTableWidgetItem(QString::number(computer->n));
    QTableWidgetItem* data2 = new QTableWidgetItem(QString::number(prec));
    QTableWidgetItem* data3 = new QTableWidgetItem(QString::number(sp10max));
    QTableWidgetItem* data4 = new QTableWidgetItem(QString::number(sp21max));
    QTableWidgetItem* data5 = new QTableWidgetItem(QString::number(sp32max));

    QVector<QTableWidgetItem*> dataItems = {data1, data2, data3, data4, data5};

    table->setRowCount(table->rowCount() + 1);

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setItem(table->rowCount() - 1, i, dataItems[i]);
    }

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setColumnWidth(i, 1450 / table->columnCount());
    }
}

void Image::clearErrorTable()
{
    table->clear();
    table->setRowCount(0);

    QStringList headerLabels;
    headerLabels << "Узлов";
    headerLabels << "Проверочных точек";
    headerLabels << "Rmax линейного сплайна";
    headerLabels << "Rmax квадратичного сплайна";
    headerLabels << "Rmax кубического сплайна";

    table->setHorizontalHeaderLabels(headerLabels);
}

void Image::doWork()
{
    QVector<Polynom> p10 = computer->p10;
    QVector<Polynom> p21 = computer->p21;
    QVector<Polynom> p32 = computer->p32;

    sp10data.clear();
    sp21data.clear();
    sp32data.clear();

    funcData.clear();

    sp10max = -1;
    sp21max = -1;
    sp32max = -1;

    QVector<double> pts = computer->points();

    for (int i = 0; i < prec; i++)
    {
        double step = (computer->b - computer->a) * 1.0 / prec;
        double currentPoint = computer->a + i * step;
        funcData.push_back(QPointF(currentPoint, computer->func(currentPoint)));
    }

    for (int i = 0; i < computer->n; i++)
    {
        Polynom p1 = p10[i];
        Polynom p2 = p21[i];
        Polynom p3 = p32[i];

        QVector<QPointF> temp1;
        QVector<QPointF> temp2;
        QVector<QPointF> temp3;

        double step = (pts[i + 1] - pts[i]) * 1.0 / prec;

        for (int j = 0; j < prec; j++)
        {
            double currentPoint = pts[i] + j * step;

            temp1.push_back(QPointF(currentPoint, p1.value(currentPoint)));
            temp2.push_back(QPointF(currentPoint, p2.value(currentPoint)));
            temp3.push_back(QPointF(currentPoint, p3.value(currentPoint)));

            if (sp10max < fabs(computer->func(currentPoint) - p1.value(currentPoint)))
            {
                sp10max = fabs(computer->func(currentPoint) - p1.value(currentPoint));
            }
            if (sp21max < fabs(computer->func(currentPoint) - p2.value(currentPoint)))
            {
                sp21max = fabs(computer->func(currentPoint) - p2.value(currentPoint));
            }
            if (sp32max < fabs(computer->func(currentPoint) - p3.value(currentPoint)))
            {
                sp32max = fabs(computer->func(currentPoint) - p3.value(currentPoint));
            }
        }

        sp10data.push_back(temp1);
        sp21data.push_back(temp2);
        sp32data.push_back(temp3);
    }

    repaint();
    updateBrowserData();
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

    for (int i = 0; i < prec; i++)
    {
        if (funcData.size() > 0)
        {
            pen.setColor(Qt::black);
            painter.setPen(pen);
            painter.drawPoint(QPointF(funcData[i].x(), -funcData[i].y()) * zoom);
        }

        for (int j = 0; j < sp10data.size(); j++)
        {
            if (sp10button->isChecked())
            {
                pen.setColor(Qt::red);
                painter.setPen(pen);
                painter.drawPoint(QPointF(sp10data[j][i].x(), -sp10data[j][i].y()) * zoom);
            }

            if (sp21button->isChecked())
            {
                pen.setColor(Qt::green);
                painter.setPen(pen);
                if (sp21data[j][i].x() == sp21data[j][i].x() && -sp21data[j][i].y() == -sp21data[j][i].y())
                {
                    painter.drawPoint(QPointF(sp21data[j][i].x(), -sp21data[j][i].y()) * zoom);
                }
            }

            if (sp32button->isChecked())
            {
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                painter.drawPoint(QPointF(sp32data[j][i].x(), -sp32data[j][i].y()) * zoom);
            }
        }
    }
}

void Image::updateBrowserData()
{
    polyBrowser->clear();
    QString s;
    s += "Линейный сплайн\n";
    for (int i = 0; i < computer->p10.size(); i++)
    {
        s += computer->p10[i] + QString("\n");
    }
    s += "Квадратичный сплайн\n";
    for (int i = 0; i < computer->p21.size(); i++)
    {
        s += computer->p21[i] + QString("\n");
    }
    s += "Кубический сплайн\n";
    for (int i = 0; i < computer->p32.size(); i++)
    {
        s += computer->p32[i] + QString("\n");
    }

    polyBrowser->setText(s);
    dataBrowser->setText("Интервал: [" + QString::number(computer->a) + ", " + QString::number(computer->b) + "] \n" +
                         "Количество узлов: " + QString::number(computer->n) + "\n" +
                         "Зум: " + QString::number(zoom) + "\n" + "Точность:" + QString::number(prec));
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

