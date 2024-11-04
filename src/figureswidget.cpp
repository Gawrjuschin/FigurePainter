#include "figureswidget.h"
#include "figureparams.h"
#include "figureshistory.h"
#include "figuresmodel.h"
#include "figuresrenderer.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QGroupBox>
#include <QLabel>

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>

#include <QPushButton>

#include <QFileDialog>
#include <QTableView>

#include <QFile>

#include <limits>

enum CBoxIndexes { CIRCLE, TRIANGLE, SQUARE, NANGLE };

constexpr long MIN_EDGES = 3;
constexpr long MAX_EDGES = 32;

constexpr double MIN_ANGLE = 0;
constexpr double MAX_ANGLE = 360;

constexpr double MIN_COORD = 0;
constexpr double MAX_COORD = 1000;

constexpr double MIN_DIAMETER = 2;
constexpr double MAX_DIAMETER = MAX_COORD / 2;

constexpr long MIN_COLOR = 0;
constexpr long MAX_COLOR = std::numeric_limits<long>::max();

constexpr long MIN_WIDTH = 1;
constexpr long MAX_WIDTH = 32;

QWidget *FiguresWidget::MakeMenu()
{
  auto *menu_wdg = new QGroupBox(tr("Область меню"));
  auto *menu_lo = new QVBoxLayout(menu_wdg);

  menu_lo->addStretch(1);

  auto *grid_lo = new QGridLayout;
  menu_lo->addLayout(grid_lo);

  // Настройка комбобокса с фигурами
  {
    figure_->addItem(tr("Круг"));
    figure_->addItem(tr("Треугольник"));
    figure_->addItem(tr("Квадрат"));
    figure_->addItem(tr("Многоугольник"));

    auto *figure_lbl = new QLabel(tr("Список фигур:"));
    figure_lbl->setBuddy(figure_);

    grid_lo->addWidget(figure_lbl, 0, 0);
    grid_lo->addWidget(figure_, 1, 0);

    connect(figure_,
	    static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	    this,
	    &FiguresWidget::onCBoxSwitch);
  }

  // Настройка ввода количества углов
  {
    edges_->setRange(MIN_EDGES, MAX_EDGES);

    auto *edges_lbl = new QLabel(tr("Количество углов:"));
    edges_lbl->setBuddy(edges_);

    grid_lo->addWidget(edges_lbl, 0, 1);
    grid_lo->addWidget(edges_, 1, 1);
  }

  // Настройки ввода координат фигуры
  {
    coord_x_->setRange(MIN_COORD, MAX_COORD);
    coord_y_->setRange(MIN_COORD, MAX_COORD);

    auto *coords_lbl = new QLabel(tr("Координаты центра:"));
    coords_lbl->setBuddy(coord_x_);

    grid_lo->addWidget(coords_lbl, 2, 0, 1, 2);
    grid_lo->addWidget(coord_x_, 3, 0);
    grid_lo->addWidget(coord_y_, 3, 1);
  }

  // Настройка ввода угла поворота
  {
    grad_angle_->setRange(MIN_ANGLE, MAX_ANGLE);

    auto *angle_lbl = new QLabel(tr("Угол вращения:"));
    angle_lbl->setBuddy(grad_angle_);

    grid_lo->addWidget(angle_lbl, 4, 0);
    grid_lo->addWidget(grad_angle_, 5, 0);
  }

  // Настройка ввода размера фигуры
  {
    diameter_->setRange(MIN_DIAMETER, MAX_DIAMETER);

    auto *size_lbl = new QLabel(tr("Размер:"));
    size_lbl->setBuddy(diameter_);
    grid_lo->addWidget(size_lbl, 4, 1);
    grid_lo->addWidget(diameter_, 5, 1);
  }
  // Настройки пера
  {
    color_->setInputMask("hhhhhh");
    color_->setText("00000000");

    auto *color_lbl = new QLabel(tr("Цвет (RGB):"));
    color_lbl->setBuddy(color_);

    grid_lo->addWidget(color_lbl, 6, 0);
    grid_lo->addWidget(color_, 7, 0);

    width_->setRange(MIN_WIDTH, MAX_WIDTH);

    auto *width_lbl = new QLabel(tr("Ширина:"));
    width_lbl->setBuddy(width_);

    grid_lo->addWidget(width_lbl, 6, 1);
    grid_lo->addWidget(width_, 7, 1);
  }
  // Начальное состояние
  onCBoxSwitch(CBoxIndexes::CIRCLE);

  // Настройка кнопок
  {
    auto *clear_btn = new QPushButton(tr("Очистить"));
    grid_lo->addWidget(clear_btn, 8, 0);
    connect(clear_btn, &QPushButton::clicked, renderer_, &FiguresRenderer::onClear);

    auto *redraw_btn = new QPushButton(tr("Перерисовать"));
    grid_lo->addWidget(redraw_btn, 8, 1);
    connect(redraw_btn, &QPushButton::clicked, history_, &FiguresHistory::onRedraw);

    auto *history_btn = new QPushButton(tr("История"));
    history_->setModal(true);
    grid_lo->addWidget(history_btn, 9, 0);
    connect(history_btn, &QPushButton::clicked, history_, &FiguresHistory::show);

    auto *add_btn = new QPushButton(tr("Добавить"));
    grid_lo->addWidget(add_btn, 9, 1);
    connect(add_btn, &QPushButton::clicked, this, &FiguresWidget::onAddClicked);

    auto *load_btn = new QPushButton(tr("Загрузить"));
    grid_lo->addWidget(load_btn, 10, 0);
    connect(load_btn, &QPushButton::clicked, this, &FiguresWidget::onLoadClicked);

    auto *save_btn = new QPushButton(tr("Сохранить"));
    grid_lo->addWidget(save_btn, 10, 1);
    connect(save_btn, &QPushButton::clicked, this, &FiguresWidget::onSaveClicked);
  }

  menu_lo->addStretch(1);

  return menu_wdg;
}

QWidget *FiguresWidget::MakeDisplay()
{
  auto *display_wdg = new QGroupBox(tr("Область отображения"));
  auto *display_lo = new QVBoxLayout(display_wdg);

  display_lo->addWidget(renderer_, 1);

  return display_wdg;
}

FiguresWidget::FiguresWidget(QWidget *parent)
    : QWidget(parent)
    , figure_(new QComboBox)
    , edges_(new QSpinBox)
    , coord_x_(new QSpinBox)
    , coord_y_(new QSpinBox)
    , grad_angle_(new QSpinBox)
    , diameter_(new QSpinBox)
    , color_(new QLineEdit)
    , width_(new QSpinBox)
    , renderer_(new FiguresRenderer)
    , history_(new FiguresHistory)
{
  history_->setWindowTitle(tr("История"));

  auto *main_lo = new QHBoxLayout(this);

  main_lo->addWidget(MakeMenu());
  main_lo->addWidget(MakeDisplay(), 1);

  connect(this, &FiguresWidget::sigAddFigures, history_, &FiguresHistory::onAddFigures);
  connect(this, &FiguresWidget::sigAddFigures, renderer_, &FiguresRenderer::onAddFigures);
  connect(history_, &FiguresHistory::sigAddFigures, renderer_, &FiguresRenderer::onAddFigures);
}

FiguresWidget::~FiguresWidget()
{
  delete history_;
}

void FiguresWidget::onCBoxSwitch(int index)
{
  Q_ASSERT(index >= 0);
  Q_ASSERT(index <= CBoxIndexes::NANGLE);
  switch (index) {
    case CBoxIndexes::CIRCLE:
      edges_->setMinimum(-1);
      edges_->setValue(-1);
      edges_->clear();
      edges_->setEnabled(false);
      grad_angle_->setEnabled(false);
      break;
    case CBoxIndexes::TRIANGLE:
      edges_->setMinimum(3);
      edges_->setValue(3);
      edges_->setEnabled(false);
      grad_angle_->setEnabled(true);
      break;
    case CBoxIndexes::SQUARE:
      edges_->setMinimum(4);
      edges_->setValue(4);
      edges_->setEnabled(false);
      grad_angle_->setEnabled(true);
      break;
    case CBoxIndexes::NANGLE:
      edges_->setMinimum(5);
      edges_->setValue(5);
      edges_->setEnabled(true);
      grad_angle_->setEnabled(true);
      break;
  }
}

void FiguresWidget::onAddClicked()
{
  FigureParams fig{.coordinates = {double(coord_x_->value()), double(coord_y_->value())},
		   .radius = (double(diameter_->value()) / 2),
		   .edges = edges_->value(),
		   .grad_angle = grad_angle_->value(),
		   .width = width_->value(),
		   // За проверку ввода цвета отвечает маска
		   .color = color_->text().toLong(nullptr, 16)};
  emit sigAddFigures(fig.toBytes());
}

void FiguresWidget::onLoadClicked()
{
  auto file_path = QFileDialog::getOpenFileName(this,
						tr("Открыть файл"),
						QDir::homePath(),
						tr("Файл фигур (*.fig)"));

  if (file_path.isEmpty()) {
    return;
  }

  QFile figures_file(file_path);
  if (!figures_file.open(QIODevice::ReadOnly)) {
    return;
  }

  auto bytes = figures_file.readAll();
  if (bytes.isEmpty()) {
    return;
  }

  history_->onClear();
  emit sigAddFigures(bytes);
}

void FiguresWidget::onSaveClicked()
{
  auto file_path = QFileDialog::getSaveFileName(this,
						tr("Сохранить файл"),
						QDir::homePath() + QDir::separator() + "file.fig",
						tr("Файл фигур (*.fig)"));

  if (file_path.isEmpty()) {
    return;
  }

  QFile figures_file(file_path);
  if (!figures_file.open(QIODevice::WriteOnly)) {
    return;
  }

  auto bytes = history_->toBytes();
  figures_file.write(bytes);
}
