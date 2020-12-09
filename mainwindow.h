#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int k;
    void new_table();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void slotAction();

private:
    Ui::MainWindow *ui;
};

class MainWindow2 : public QDialog, public QPainter
{
    Q_OBJECT
    QVector<QLabel *> L_percents, L_faults, L_percs, L_sum_percs;
    QScrollArea *S_graph;
    QGroupBox *G_graph;
    double shift;

public:
    MainWindow2(QWidget *parent = 0);
    ~MainWindow2();
protected:
    void paintEvent(QPaintEvent *);
};

class MainWindow3 : public QDialog
{
    Q_OBJECT
    QTableWidget *Tab_legend;

public:
    MainWindow3(QWidget *parent = 0);
    ~MainWindow3();
};

#endif // MAINWINDOW_H
