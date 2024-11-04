#include "figureswidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  FiguresWidget w;
  w.setWindowTitle("Демонстрационный виджет");
  w.setMinimumSize(800, 600);
  w.show();
  return a.exec();
}
