#ifndef FIGURESWIDGET_H
#define FIGURESWIDGET_H

#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QTimer;
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
  void onDemoClicked();
  void onDemoTimerTick();

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
  QTimer* demo_timer_;
  QPushButton* demo_button_;
};
#endif // FIGURESWIDGET_H
