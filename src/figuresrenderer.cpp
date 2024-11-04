#include "figuresrenderer.h"
#include "figureparams.h"

#include <QPaintEvent>

#include <QPainter>
#include <QPainterPath>

#include <QDebug>

#include <cmath>

struct FiguresRenderer::PathPair
{
  QPainterPath path;
  QPen pen;
};

FiguresRenderer::FiguresRenderer(QWidget *parent)
    : QWidget{parent}
    , pathes_{}
{}

FiguresRenderer::~FiguresRenderer() = default;

void FiguresRenderer::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.translate(rect().bottomLeft());
  for (const auto &[path, pen] : pathes_) {
    painter.setPen(pen);
    painter.drawPath(path);
  }
}

static inline double ToRad(long angle)
{
  return (angle * std::numbers::pi / 180);
}

static inline QPointF Rotate(const QPointF &point, long grad_angle)
{
  double angle = ToRad(grad_angle);
  return {point.x() * std::sin(angle) + point.y() * std::cos(angle),
	  -point.x() * std::cos(angle) + point.y() * std::sin(angle)};
}

// Для N-угольника радиуса r возвращаю corner_num вершину
static inline QPointF MakeCorner(std::uint32_t N, double r, std::uint32_t corner_num)
{
  Q_ASSERT(N > 2);
  Q_ASSERT(corner_num >= 0);
  Q_ASSERT(corner_num < N);
  const double angle = std::numbers::pi / (2 * N);
  const double period = 2 * std::numbers::pi / N;
  return {r * std::sin(angle + corner_num * period), r * std::cos(angle + corner_num * period)};
}

// Переход к правым координатам
static inline QPointF InvertY(QPointF point)
{
  point.ry() = -point.y();
  return point;
}

static inline QPainterPath MakePainterPath(const FigureParams &fig)
{
  Q_ASSERT(fig);
  Q_ASSERT(fig.edges == -1 || fig.edges >= 3);

  QPainterPath path;
  if (-1 == fig.edges) {
    path.addEllipse(InvertY(fig.coordinates), fig.radius, fig.radius);
    return path;
  }

  // Начальное положение пера
  const auto corner = MakeCorner(fig.edges, fig.radius, fig.edges - 1);
  path.moveTo(InvertY(fig.coordinates + Rotate(corner, fig.grad_angle)));

  for (std::uint32_t corner_num = 0; corner_num < fig.edges; ++corner_num) {
    const auto corner = MakeCorner(fig.edges, fig.radius, corner_num);
    path.lineTo(InvertY(fig.coordinates + Rotate(corner, fig.grad_angle)));
  }

  return path;
}

static inline QPen MakePen(const FigureParams &fig)
{
  QPen pen;
  pen.setColor(QColor::fromRgb(QRgb(fig.color)));
  pen.setWidth(fig.width);
  return pen;
}

void FiguresRenderer::onAddFigures(QByteArray bytes)
{
  Q_ASSERT(bytes.count() % sizeof(FigureParams) == 0);
  const auto figures_count = bytes.count() / sizeof(FigureParams);
  Q_ASSERT(figures_count >= 0);

  for (int i = 0; i < figures_count; ++i) {
    const FigureParams &fig = *reinterpret_cast<const FigureParams *>(bytes.data()
								      + i * sizeof(FigureParams));
    pathes_.append({MakePainterPath(fig), MakePen(fig)});
  }

  update();
}

void FiguresRenderer::onClear()
{
  pathes_.clear();
  update();
}
