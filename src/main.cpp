#include "figureswidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  FiguresWidget w;
  w.show();
  return a.exec();
}
