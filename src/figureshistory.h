#ifndef FIGURESHISTORY_H
#define FIGURESHISTORY_H

#include <QDialog>

class QTableView;
class FiguresModel;

class FiguresHistory : public QDialog
{
  Q_OBJECT
public:
  explicit FiguresHistory(QWidget *parent = nullptr);
  ~FiguresHistory();

  QByteArray toBytes() const;

public slots:
  void onAddClicked();
  void onAddFigures(QByteArray);
  void onRedraw();
  void onClear();

signals:
  // Посылается при выборе строк
  void sigAddFigures(QByteArray);

private:
  QTableView *view_;
  FiguresModel *model_;
};

#endif // FIGURESHISTORY_H
