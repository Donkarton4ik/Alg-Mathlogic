#include "MainWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTimer>
#include <cstdlib>
#include <string>

MainWindow::MainWindow(){
    width_spin = new QSpinBox;
    width_spin->setRange(3, 100);
    width_spin->setValue(20);

    iters_spin = new QSpinBox;
    iters_spin->setRange(1, 100000);
    iters_spin->setValue(10);

    random_radio = new QRadioButton("Случайные");
    manual_radio = new QRadioButton("Вручную (клик по полю)");
    random_radio->setChecked(true);

    init_button = new QPushButton("Перегенерировать");
    step_button = new QPushButton("Шаг");
    run_button = new QPushButton("Старт");

    grid_widget = new GridWidget;
    grid_widget->setMinimumSize(400, 400);

    QVBoxLayout* left_panel = new QVBoxLayout;
    left_panel->addWidget(new QLabel("Ширина:"));
    left_panel->addWidget(width_spin);
    left_panel->addWidget(new QLabel("Число итераций:"));
    left_panel->addWidget(iters_spin);
    left_panel->addWidget(new QLabel("Начальные условия:"));
    left_panel->addWidget(random_radio);
    left_panel->addWidget(manual_radio);
    left_panel->addWidget(init_button);
    left_panel->addWidget(step_button);
    left_panel->addWidget(run_button);
    left_panel->addStretch();

    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->addLayout(left_panel, 0);
    main_layout->addWidget(grid_widget, 1);

    timer = new QTimer(this);
    timer->setInterval(150);

    connect(init_button, &QPushButton::clicked, this, &MainWindow::on_init);
    connect(step_button, &QPushButton::clicked, this, &MainWindow::make_step);
    connect(run_button, &QPushButton::clicked, this, &MainWindow::on_toggle_run);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_auto_tick);
    connect(grid_widget, &GridWidget::cell_clicked, this, &MainWindow::on_cell_clicked);

    on_init();
}

//слот на перегенерацию
void MainWindow::on_init(){
    field_size = width_spin->value();
    max_iterations = iters_spin->value();
    iteration = 0;

    cells.assign(field_size, std::vector<int>(field_size, 0));
    if (random_radio->isChecked()){
        for (int r = 0; r < field_size; ++r){
            for (int c = 0; c < field_size; ++c){
                cells[r][c] = std::rand() % 2;
            }
        }
    }
    grid_widget->set_grid(cells);
}

//поле имеет форму тора
int MainWindow::torus_index(int i){
    if (i < 0) return i + field_size;
    if (i >= field_size) return i - field_size;
    return i;
}

//вычисление next состояния клетки по окрестности фон-неймана
int MainWindow::next_state(int row, int col){
    //смещенные индексы соседей текущей клетки
    int up = torus_index(row - 1);
    int down = torus_index(row + 1);
    int left = torus_index(col - 1);
    int right = torus_index(col + 1);

    //порядок: верх, лево, центр, право, низ
    std::string key;
    key += std::to_string(cells[up][col]);
    key += std::to_string(cells[row][left]);
    key += std::to_string(cells[row][col]);
    key += std::to_string(cells[row][right]);
    key += std::to_string(cells[down][col]);

    return neumann.neumann.at(key);
}

//для каждой клетки вычисляем новое состояние и обновляем сетку
void MainWindow::make_step(){
    std::vector<std::vector<int>> next(field_size, std::vector<int>(field_size, 0));
    for (int r = 0; r < field_size; ++r){
        for (int c = 0; c < field_size; ++c){
            next[r][c] = next_state(r, c);
        }
    }

    cells = next;
    iteration = iteration + 1;
    grid_widget->set_grid(cells);
}

//по тику таймера делаем шаг
void MainWindow::on_auto_tick(){
    if (remaining_auto_steps <= 0){
        timer->stop();
        run_button->setText("Старт");
        return;
    }

    make_step();
    remaining_auto_steps = remaining_auto_steps - 1;
}

//нажатие кнопки старт
void MainWindow::on_toggle_run(){
    if (timer->isActive()){
        timer->stop();
        run_button->setText("Старт");
    } else {
        max_iterations = iters_spin->value();
        remaining_auto_steps = max_iterations;
        timer->start();
        run_button->setText("Стоп");
    }
}

//по клику на клетку инвертируем ее значение и обновляем сетку
void MainWindow::on_cell_clicked(int row, int col){
    //менять клетки можно только пока выбран режим вручную
    if (manual_radio->isChecked() == false) return;
    cells[row][col] = cells[row][col] ? 0 : 1;
    grid_widget->set_grid(cells);
}
