#ifndef FIGURESRENDERER_H
#define FIGURESRENDERER_H

#include <QList>
#include <QWidget>

class QPainterPath;
class QPen;

class FiguresRenderer : public QWidget
{
  Q_OBJECT
public:
  explicit FiguresRenderer(QWidget *parent = nullptr);
  ~FiguresRenderer();

public slots:
  void onAddFigures(QByteArray);
  void onClear();

protected:
  void paintEvent(QPaintEvent *) override;

private:
  struct PathPair;
  QList<PathPair> pathes_;
};

#endif // FIGURESRENDERER_H
