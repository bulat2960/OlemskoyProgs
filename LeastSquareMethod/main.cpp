#include <QApplication>

#include <ctime>

#include <QtWidgets>

#include "computer.h"
#include "image.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(static_cast<unsigned int>(time(nullptr)));

    Computer* computer = new Computer(0, 0, 1, 26);

    QWidget* widget = new QWidget;

    QCheckBox* funcCb = new QCheckBox("График функции");
    QCheckBox* cb1 = new QCheckBox("График нормального уравнения");
    QCheckBox* cb2 = new QCheckBox("График ортогонального полинома");

    funcCb->setStyleSheet("QCheckBox { color: blue }");
    cb1->setStyleSheet("QCheckBox { color: red }");
    cb2->setStyleSheet("QCheckBox { color: black }");

    QHBoxLayout* checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(funcCb);
    checkBoxLayout->addWidget(cb1);
    checkBoxLayout->addWidget(cb2);

    QTextBrowser* polyBrowser = new QTextBrowser;
    polyBrowser->setMinimumHeight(100);
    polyBrowser->setMaximumHeight(200);

    QTextBrowser* dataBrowser = new QTextBrowser;
    dataBrowser->setMaximumSize(300, 200);

    QHBoxLayout* dataLayout = new QHBoxLayout;
    dataLayout->addWidget(polyBrowser);
    dataLayout->addWidget(dataBrowser);

    QTableWidget* table = new QTableWidget(0, 4);
    table->setMinimumHeight(100);
    table->setMaximumHeight(200);

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setColumnWidth(i, 1850 / table->columnCount());
    }

    QVBoxLayout* browserLayout = new QVBoxLayout;
    browserLayout->addLayout(dataLayout);
    browserLayout->addWidget(table);

    Image* image = new Image(funcCb, cb1, cb2, polyBrowser, dataBrowser, table, computer);
    image->setMinimumSize(100, 100);

    // 1
    QSlider* firstBorderSlider = new QSlider(Qt::Horizontal);
    firstBorderSlider->setRange(-10, 0);
    firstBorderSlider->setValue(0);
    QObject::connect(firstBorderSlider, SIGNAL(valueChanged(int)), computer, SLOT(setLeftBorder(int)));
    QObject::connect(firstBorderSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QSlider* secondBorderSlider = new QSlider(Qt::Horizontal);
    secondBorderSlider->setRange(0, 10);
    secondBorderSlider->setValue(0);
    QObject::connect(secondBorderSlider, SIGNAL(valueChanged(int)), computer, SLOT(setRightBorder(int)));
    QObject::connect(secondBorderSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QHBoxLayout* borderSliderLayout = new QHBoxLayout;
    borderSliderLayout->addWidget(firstBorderSlider);
    borderSliderLayout->addWidget(secondBorderSlider);

    // 2
    QSlider* pointNumberSlider = new QSlider(Qt::Horizontal);
    pointNumberSlider->setRange(1, 5);
    QObject::connect(pointNumberSlider, SIGNAL(valueChanged(int)), computer, SLOT(setPointNumber(int)));
    QObject::connect(pointNumberSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QSlider* equationNumberSlider = new QSlider(Qt::Horizontal);
    equationNumberSlider->setRange(26, 100);
    QObject::connect(equationNumberSlider, SIGNAL(valueChanged(int)), computer, SLOT(setEquationNumber(int)));
    QObject::connect(equationNumberSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    // 3
    QSlider* zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(1, 50);
    QObject::connect(zoomSlider, SIGNAL(valueChanged(int)), image, SLOT(changeZoom(int)));

    // 4
    QSlider* precisionSlider = new QSlider(Qt::Horizontal);
    precisionSlider->setRange(100, 10000);
    QObject::connect(precisionSlider, SIGNAL(valueChanged(int)), image, SLOT(changePrec(int)));
    QObject::connect(precisionSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QHBoxLayout* actionSliderLayout = new QHBoxLayout;
    actionSliderLayout->addWidget(pointNumberSlider);
    actionSliderLayout->addWidget(equationNumberSlider);
    actionSliderLayout->addWidget(zoomSlider);
    actionSliderLayout->addWidget(precisionSlider);

    QVBoxLayout* sliderLayout = new QVBoxLayout;
    sliderLayout->addLayout(borderSliderLayout, 1);
    sliderLayout->addLayout(actionSliderLayout, 2);


    QVBoxLayout* grid = new QVBoxLayout(widget);
    grid->addWidget(image, 1);
    grid->addLayout(checkBoxLayout, 2);
    grid->addLayout(sliderLayout, 3);
    grid->addLayout(browserLayout, 4);

    QPushButton* exitButton = new QPushButton(widget);
    exitButton->setGeometry(0, 0, QApplication::screens().at(0)->geometry().width() / 2, 50);
    exitButton->setText("Click to close");
    QObject::connect(exitButton, SIGNAL(clicked()), widget, SLOT(close()));

    QPushButton* clearButton = new QPushButton(widget);
    clearButton->setGeometry(QApplication::screens().at(0)->geometry().width() / 2, 0, QApplication::screens().at(0)->geometry().width() / 2, 50);
    clearButton->setText("Click to clear the table");
    QObject::connect(clearButton, SIGNAL(clicked()), image, SLOT(clearErrorTable()));

    widget->showFullScreen();
    return a.exec();
}
