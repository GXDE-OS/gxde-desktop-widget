#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) ;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void UpdateInformation();
    void DrawingClock();
    void UpdateTime();
    // 数学计算
    QList<int> GetCoordinateOnCircularArc(double angle, double r);
    double DegreeToRadian(double degree);
};
#endif // MAINWINDOW_H
