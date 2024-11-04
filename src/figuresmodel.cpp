#include "figuresmodel.h"
#include "figureparams.h"

#include <QPointF>

constexpr int COLUMN_COUNT = 6;

enum Columns { FIGURE_TYPE, COORDINATES, ANGLE, SIZE, COLOR, WIDTH };

FiguresModel::FiguresModel(QObject *parent)
    : QAbstractListModel{parent}
{}

FiguresModel::~FiguresModel() = default;

QVariant FiguresModel::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DisplayRole) {
    return {};
  }

  const auto &fig_params = model_data_[index.row()];

  switch (index.column()) {
    case Columns::FIGURE_TYPE:
      return QVariant::fromValue(FigureParams::typeStr(fig_params));
    case Columns::COORDINATES:
      return QString("(%1, %2)").arg(fig_params.coordinates.x()).arg(fig_params.coordinates.x());
    case Columns::ANGLE:
      return QVariant::fromValue(fig_params.grad_angle);
    case Columns::SIZE:
      return QVariant::fromValue(2 * fig_params.radius);
    case Columns::COLOR:
      return QString("%1").arg(fig_params.color, 6, 16, QChar('0'));
    case Columns::WIDTH:
      return QVariant::fromValue(fig_params.width);

    default:
      return {};
  }
}

int FiguresModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid()) {
    return -1;
  }
  return model_data_.count();
}

int FiguresModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid()) {
    return -1;
  }
  return COLUMN_COUNT;
}

QVariant FiguresModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Orientation::Horizontal) {
    return {};
  }
  switch (section) {
    case Columns::FIGURE_TYPE:
      return tr("Фигура");
    case Columns::COORDINATES:
      return tr("Координаты центра");
    case Columns::ANGLE:
      return tr("Угол вращения");
    case Columns::SIZE:
      return tr("Размер");
    case Columns::COLOR:
      return tr("Цвет");
    case Columns::WIDTH:
      return tr("Ширина");

    default:
      return {};
  }
}

void FiguresModel::append(QByteArray bytes)
{
  Q_ASSERT(bytes.count() % sizeof(FigureParams) == 0);
  const auto figures_count = bytes.count() / sizeof(FigureParams);

  if (0 == figures_count) {
    return;
  }

  Q_ASSERT(figures_count > 0);

  const FigureParams &fig = *reinterpret_cast<const FigureParams *>(bytes.data());
  beginInsertRows({}, model_data_.count(), model_data_.count() + figures_count - 1);

  for (int i = 0; i < figures_count; ++i) {
    const FigureParams &fig = *reinterpret_cast<const FigureParams *>(bytes.data()
								      + i * sizeof(FigureParams));
    model_data_.append(fig);
  }

  endInsertRows();
}
void FiguresModel::append(const FigureParams &fig)
{
  beginInsertRows({}, model_data_.count(), model_data_.count());
  model_data_.append(fig);
  endInsertRows();
}

void FiguresModel::append(const QList<FigureParams> &fig_list)
{
  if (0 == fig_list.count()) {
    return;
  }
  beginInsertRows({}, model_data_.count(), model_data_.count() + fig_list.count() - 1);
  for (const auto &fig : fig_list) {
    model_data_.append(fig);
  }
  endInsertRows();
}

void FiguresModel::clear()
{
  beginResetModel();
  model_data_.clear();
  endResetModel();
}

const QList<FigureParams> &FiguresModel::rawData() const
{
  return model_data_;
}
