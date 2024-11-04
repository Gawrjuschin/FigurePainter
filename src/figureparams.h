#ifndef FIGUREPARAMS_H
#define FIGUREPARAMS_H

#include <QByteArray>
#include <QPointF>
#include <QString>

// Протокол для передачи параметров
struct FigureParams
{
  static QString typeStr(FigureParams fig)
  {
    switch (fig.edges) {
      case -1:
	return "Круг";
      case 0:
	return "NONE";
      case 1:
      case 2:
	return "ERROR";
      case 3:
	return "Треугольник";
      case 4:
	return "Квадрат";
      default:
	return QString("%1-угольник").arg(fig.edges);
    }
  }

  QPointF coordinates{};
  double radius{};
  long edges{};
  long grad_angle{};
  long width{};
  long color{};

  operator bool() const noexcept { return edges != 0; }

  // Работаю с байтами, чтобы не регистрировать структуру как метатип
  QByteArray toBytes() const
  {
    return QByteArray(reinterpret_cast<const char *>(this), sizeof(FigureParams));
  }

  static FigureParams fromBytes(QByteArray bytes)
  {
    if (bytes.size() != sizeof(FigureParams)) {
      return {};
    }

    const FigureParams *fig = reinterpret_cast<const FigureParams *>(bytes.data());
    return *fig;
  }
};

#endif // FIGUREPARAMS_H
