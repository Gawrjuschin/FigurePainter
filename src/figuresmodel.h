#ifndef FIGURESMODEL_H
#define FIGURESMODEL_H

#include <QAbstractListModel>

struct FigureParams;

class FiguresModel : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit FiguresModel(QObject *parent = nullptr);
  ~FiguresModel();

public:
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  QVariant headerData(int section,
		      Qt::Orientation orrientation,
		      int role = Qt::DisplayRole) const override;

  int rowCount(const QModelIndex &parent = {}) const override;
  int columnCount(const QModelIndex &parent = {}) const override;

  void append(QByteArray);

  void append(const FigureParams &);
  void append(const QList<FigureParams> &);

  void clear();

  const QList<FigureParams> &rawData() const;

private:
  QList<FigureParams> model_data_;
};

#endif // FIGURESMODEL_H
