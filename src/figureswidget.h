#ifndef FIGURESWIDGET_H
#define FIGURESWIDGET_H

#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QSpinBox;
class FiguresRenderer;
class FiguresModel;
class FiguresHistory;

class FiguresWidget : public QWidget
{
  Q_OBJECT

public:
  FiguresWidget(QWidget *parent = nullptr);
  ~FiguresWidget();

public slots:
  void onCBoxSwitch(int);
  void onAddClicked();
  void onLoadClicked();
  void onSaveClicked();

signals:
  void sigAddFigures(QByteArray);

private:
  QWidget *MakeMenu();
  QWidget *MakeDisplay();

private:
  QComboBox *figure_;
  QSpinBox *edges_;
  QSpinBox *coord_x_;
  QSpinBox *coord_y_;
  QSpinBox *grad_angle_;
  QSpinBox *diameter_;
  QLineEdit *color_;
  QSpinBox *width_;
  FiguresRenderer *renderer_;
  FiguresHistory *history_;
};
#endif // FIGURESWIDGET_H
