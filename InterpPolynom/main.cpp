#include <QApplication>
#include <QDebug>

#include <QtWidgets>

#include "computer.h"
#include "image.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Computer* computer = new Computer(0, 0, 1);

    QWidget* widget = new QWidget;

    QCheckBox* funcCheckbox  = new QCheckBox("График функции");
    QCheckBox* lagrCheckbox1 = new QCheckBox("График полинома Лагранжа");
    QCheckBox* lagrCheckbox2 = new QCheckBox("График оптимального полинома Лагранжа");
    QCheckBox* newtonCheckbox1 = new QCheckBox("График полинома Ньютона");
    QCheckBox* newtonCheckbox2 = new QCheckBox("График оптимального полинома Ньютона");

    funcCheckbox->setStyleSheet("QCheckBox { color: red }");
    lagrCheckbox1->setStyleSheet("QCheckBox { color: black }");
    lagrCheckbox2->setStyleSheet("QCheckBox { color: blue }");
    newtonCheckbox1->setStyleSheet("QCheckBox { color: green }");
    newtonCheckbox2->setStyleSheet("QCheckBox { color: magenta }");

    QHBoxLayout* checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(funcCheckbox);
    checkBoxLayout->addWidget(lagrCheckbox1);
    checkBoxLayout->addWidget(lagrCheckbox2);
    checkBoxLayout->addWidget(newtonCheckbox1);
    checkBoxLayout->addWidget(newtonCheckbox2);

    QTextBrowser* polyBrowser = new QTextBrowser;
    polyBrowser->setMinimumHeight(100);
    polyBrowser->setMaximumHeight(200);

    QTextBrowser* dataBrowser = new QTextBrowser;
    dataBrowser->setMaximumSize(300, 200);

    QHBoxLayout* dataLayout = new QHBoxLayout;
    dataLayout->addWidget(polyBrowser);
    dataLayout->addWidget(dataBrowser);

    QTableWidget* table = new QTableWidget(0, 6);
    table->setMinimumHeight(100);
    table->setMaximumHeight(300);

    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setColumnWidth(i, 1850 / table->columnCount());
    }

    QVBoxLayout* browserLayout = new QVBoxLayout;
    browserLayout->addLayout(dataLayout);
    browserLayout->addWidget(table);

    Image* image = new Image(funcCheckbox, lagrCheckbox1, lagrCheckbox2, newtonCheckbox1, newtonCheckbox2, polyBrowser, dataBrowser, table, computer);
    image->setMinimumSize(100, 100);

    // 1
    QSlider* firstBorderSlider = new QSlider(Qt::Horizontal);
    firstBorderSlider->setRange(-50, 0);
    firstBorderSlider->setValue(0);
    QObject::connect(firstBorderSlider, SIGNAL(valueChanged(int)), computer, SLOT(setLeftBorder(int)));
    QObject::connect(firstBorderSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QSlider* secondBorderSlider = new QSlider(Qt::Horizontal);
    secondBorderSlider->setRange(0, 50);
    secondBorderSlider->setValue(0);
    QObject::connect(secondBorderSlider, SIGNAL(valueChanged(int)), computer, SLOT(setRightBorder(int)));
    QObject::connect(secondBorderSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QHBoxLayout* borderSliderLayout = new QHBoxLayout;
    borderSliderLayout->addWidget(firstBorderSlider);
    borderSliderLayout->addWidget(secondBorderSlider);

    // 2
    QSlider* pointNumberSlider = new QSlider(Qt::Horizontal);
    pointNumberSlider->setRange(1, 100);
    QObject::connect(pointNumberSlider, SIGNAL(valueChanged(int)), computer, SLOT(setPointNumber(int)));
    QObject::connect(pointNumberSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    // 3
    QSlider* zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(1, 50);
    QObject::connect(zoomSlider, SIGNAL(valueChanged(int)), image, SLOT(changeZoom(int)));

    // 4
    QSlider* precisionSlider = new QSlider(Qt::Horizontal);
    precisionSlider->setRange(100, 1000);
    QObject::connect(precisionSlider, SIGNAL(valueChanged(int)), image, SLOT(changePrec(int)));
    QObject::connect(precisionSlider, SIGNAL(sliderReleased()), image, SLOT(fillErrorTable()));

    QHBoxLayout* actionSliderLayout = new QHBoxLayout;
    actionSliderLayout->addWidget(pointNumberSlider);
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
