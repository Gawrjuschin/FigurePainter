#include "figureshistory.h"

#include "figureparams.h"
#include "figuresmodel.h"
#include "qpushbutton.h"

#include <QHeaderView>
#include <QTableView>

#include <QVBoxLayout>

FiguresHistory::FiguresHistory(QWidget *parent)
    : QDialog{parent}
    , view_(new QTableView)
    , model_(new FiguresModel)
{
  auto *main_lo = new QVBoxLayout(this);

  view_->setModel(model_);
  view_->setSelectionBehavior(QAbstractItemView::SelectRows);
  view_->verticalHeader()->hide();

  main_lo->addWidget(view_);

  auto *btn_lo = new QHBoxLayout;
  main_lo->addLayout(btn_lo);

  auto *close_btn = new QPushButton(tr("Закрыть"));
  auto *add_btn = new QPushButton(tr("Добавить"));
  auto *clear_btn = new QPushButton(tr("Очистить"));

  btn_lo->addWidget(close_btn);
  btn_lo->addWidget(add_btn);
  btn_lo->addWidget(clear_btn);

  connect(add_btn, &QPushButton::clicked, this, &FiguresHistory::onAddClicked);
  connect(close_btn, &QPushButton::clicked, this, &QDialog::accept);
  connect(clear_btn, &QPushButton::clicked, model_, &FiguresModel::clear);
}

FiguresHistory::~FiguresHistory() = default;

void FiguresHistory::onAddFigures(QByteArray bytes)
{
  model_->append(bytes);
}
void FiguresHistory::onClear()
{
  model_->clear();
}

void FiguresHistory::onRedraw()
{
  emit sigAddFigures(toBytes());
}

void FiguresHistory::onAddClicked()
{
  auto *sel_model = view_->selectionModel();
  if (!sel_model->hasSelection()) {
    return;
  }

  QByteArray bytes;
  const auto &raw_data = model_->rawData();
  const auto selected_rows = sel_model->selectedRows();
  for (const auto index : selected_rows) {
    const auto &fig = raw_data[index.row()];
    bytes.append(reinterpret_cast<const char *>(&fig), sizeof(FigureParams));
  }

  emit sigAddFigures(bytes);
}

QByteArray FiguresHistory::toBytes() const
{
  QByteArray bytes;
  const auto &raw_data = model_->rawData();
  for (const auto &fig : raw_data) {
    bytes.append(reinterpret_cast<const char *>(&fig), sizeof(FigureParams));
  }
  return bytes;
}
