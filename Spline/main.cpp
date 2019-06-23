#include <QApplication>
#include <QDebug>
#include <QtWidgets>

#include "matrix.h"
#include "computer.h"
#include "image.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Computer* computer = new Computer(0, 0, 1);

    QWidget* mainWidget = new QWidget;

    QCheckBox* sp10button = new QCheckBox("График линейного сплайна");
    QCheckBox* sp21button = new QCheckBox("График квадратичного сплайна");
    QCheckBox* sp32button = new QCheckBox("График кубического сплайна");

    sp10button->setStyleSheet("QCheckBox { color: red }");
    sp21button->setStyleSheet("QCheckBox { color: green }");
    sp32button->setStyleSheet("QCheckBox { color: blue }");

    QHBoxLayout* checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(sp10button);
    checkBoxLayout->addWidget(sp21button);
    checkBoxLayout->addWidget(sp32button);

    QTextBrowser* polyBrowser = new QTextBrowser;
    polyBrowser->setMaximumWidth(400);
    polyBrowser->setMaximumHeight(200);

    QTextBrowser* dataBrowser = new QTextBrowser;
    dataBrowser->setMaximumWidth(400);
    dataBrowser->setMaximumHeight(100);

    QVBoxLayout* textBrowserLayout = new QVBoxLayout;
    textBrowserLayout->addWidget(polyBrowser, 1);
    textBrowserLayout->addWidget(dataBrowser, 2);

    QTableWidget* table = new QTableWidget(0, 5);
    table->setMaximumHeight(300);

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setColumnWidth(i, 1450 / table->columnCount());
    }

    QHBoxLayout* dataLayout = new QHBoxLayout;
    dataLayout->addLayout(textBrowserLayout);
    dataLayout->addWidget(table);

    Image* image = new Image(sp10button, sp21button, sp32button, polyBrowser, dataBrowser, table, computer);
    image->setMinimumSize(100, 100);

    QSlider* leftBorderSlider = new QSlider(Qt::Horizontal);
    leftBorderSlider->setRange(-10, 0);
    leftBorderSlider->setValue(0);
    QObject::connect(leftBorderSlider, SIGNAL(valueChanged(int)), computer, SLOT(setLeftBorder(int)));
    QObject::connect(leftBorderSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QSlider* rightBorderSlider = new QSlider(Qt::Horizontal);
    rightBorderSlider->setRange(0, 10);
    rightBorderSlider->setValue(0);
    QObject::connect(rightBorderSlider, SIGNAL(valueChanged(int)), computer, SLOT(setRightBorder(int)));
    QObject::connect(rightBorderSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QSlider* zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(1, 200);
    QObject::connect(zoomSlider, SIGNAL(valueChanged(int)), image, SLOT(changeZoom(int)));

    QSlider* pointNumberSlider = new QSlider(Qt::Horizontal);
    pointNumberSlider->setRange(1, 50);
    QObject::connect(pointNumberSlider, SIGNAL(valueChanged(int)), computer, SLOT(setPointNumber(int)));
    QObject::connect(pointNumberSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QSlider* precisionSlider = new QSlider(Qt::Horizontal);
    precisionSlider->setRange(100, 1000);
    QObject::connect(precisionSlider, SIGNAL(valueChanged(int)), image, SLOT(changePrec(int)));
    QObject::connect(precisionSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QHBoxLayout* sliderLayout1 = new QHBoxLayout;
    sliderLayout1->addWidget(leftBorderSlider);
    sliderLayout1->addWidget(rightBorderSlider);

    QHBoxLayout* sliderLayout2 = new QHBoxLayout;
    sliderLayout2->addWidget(zoomSlider);
    sliderLayout2->addWidget(precisionSlider);
    sliderLayout2->addWidget(pointNumberSlider);

    QVBoxLayout* sliderLayout = new QVBoxLayout;
    sliderLayout->addLayout(sliderLayout1);
    sliderLayout->addLayout(sliderLayout2);

    QVBoxLayout* finalLayout = new QVBoxLayout(mainWidget);
    finalLayout->addWidget(image);
    finalLayout->addLayout(checkBoxLayout);
    finalLayout->addLayout(sliderLayout);
    finalLayout->addLayout(dataLayout);

    QPushButton* exitButton = new QPushButton(mainWidget);
    exitButton->setGeometry(0, 0, QApplication::screens().at(0)->geometry().width() / 2, 50);
    exitButton->setText("Click to close");
    QObject::connect(exitButton, SIGNAL(clicked()), mainWidget, SLOT(close()));

    QPushButton* clearButton = new QPushButton(mainWidget);
    clearButton->setGeometry(QApplication::screens().at(0)->geometry().width() / 2, 0, QApplication::screens().at(0)->geometry().width() / 2, 50);
    clearButton->setText("Click to clear the table");
    QObject::connect(clearButton, SIGNAL(clicked()), image, SLOT(clearErrorTable()));

    mainWidget->showFullScreen();

    return a.exec();
}
