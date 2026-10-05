#pragma once
#include <QWidget>
#include <vector>

class GridWidget : public QWidget {
    Q_OBJECT
public:
    GridWidget(){};
    void set_grid(const std::vector<std::vector<int>>& g);

signals:
    void cell_clicked(int row, int col);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<std::vector<int>> grid; 
};
