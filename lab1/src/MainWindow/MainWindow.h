#pragma once
#include <QWidget>
#include <vector>

#include "Grid/GridWidget.h"
#include "Neumann/neumann_class.h"

class QSpinBox;
class QPushButton;
class QRadioButton;
class QTimer;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void on_init();
    void on_auto_tick();
    void on_toggle_run();
    void on_cell_clicked(int row, int col);

private:
    std::vector<std::vector<int>> cells;
    int field_size = 0;
    int iteration = 0;
    int max_iterations = 0;
    int remaining_auto_steps = 0;
    Neumann_class neumann;

    int torus_index(int i);
    int next_state(int row, int col);
    void make_step();

    QSpinBox* width_spin;
    QSpinBox* iters_spin;
    QRadioButton* random_radio;
    QRadioButton* manual_radio;
    QPushButton* init_button;
    QPushButton* step_button;
    QPushButton* run_button;
    GridWidget* grid_widget;
    QTimer* timer;

};
