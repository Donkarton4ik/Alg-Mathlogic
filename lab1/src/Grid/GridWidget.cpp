#include "GridWidget.h"

#include <QPainter>
#include <QMouseEvent>


void GridWidget::set_grid(const std::vector<std::vector<int>>& g){
    grid = g;
    update();
}

void GridWidget::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    if (grid.empty()) return;

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    //ширину и высоту одной ячейки под текущий размер виджета
    double cell_w = width() / static_cast<double>(cols);
    double cell_h = height() / static_cast<double>(rows);

    //светло-серое перо для границы ячеек
    painter.setPen(Qt::lightGray);
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            //вычисляем координаты прямоугольника текущей ячейки
            QRectF cell_rect(c * cell_w, r * cell_h, cell_w, cell_h);
            //отрисовка рамки
            painter.drawRect(cell_rect);
            //заливка ячейки
            painter.fillRect(cell_rect, grid[r][c] ? Qt::black : Qt::white);
        }
    }
}

void GridWidget::mousePressEvent(QMouseEvent* event){
    if (grid.empty()) return;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    double cell_w = width() / static_cast<double>(cols);
    double cell_h = height() / static_cast<double>(rows);
    
    //перевод корд в клика в индексы ячейки
    int col = static_cast<int>(event->position().x() / cell_w);
    int row = static_cast<int>(event->position().y() / cell_h);
    if (row >= 0 && row < rows && col >= 0 && col < cols){
        emit cell_clicked(row, col);
    }
}
