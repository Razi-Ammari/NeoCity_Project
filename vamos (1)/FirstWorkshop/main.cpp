#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Modern fusion style
    a.setStyle(QStyleFactory::create("Fusion"));

    // Dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,          QColor(30,  30,  40));
    darkPalette.setColor(QPalette::WindowText,      QColor(220, 220, 230));
    darkPalette.setColor(QPalette::Base,            QColor(20,  20,  30));
    darkPalette.setColor(QPalette::AlternateBase,   QColor(40,  40,  55));
    darkPalette.setColor(QPalette::ToolTipBase,     Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,     Qt::white);
    darkPalette.setColor(QPalette::Text,            QColor(220, 220, 230));
    darkPalette.setColor(QPalette::Button,          QColor(45,  45,  60));
    darkPalette.setColor(QPalette::ButtonText,      QColor(220, 220, 230));
    darkPalette.setColor(QPalette::BrightText,      Qt::red);
    darkPalette.setColor(QPalette::Link,            QColor(0,   180, 255));
    darkPalette.setColor(QPalette::Highlight,       QColor(0,   180, 255));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    a.setPalette(darkPalette);

    MainWindow w;
    w.show();
    return a.exec();
}
