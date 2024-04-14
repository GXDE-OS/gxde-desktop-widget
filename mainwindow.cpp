#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

// 用于钟表绘制
#include <QPainter>
#include <QDateTime>

// 引用数学库
#define _USE_MATH_DEFINES
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //qDebug() << ui->clockDrawing->height();
    //ui->clockDrawing->setMaximumWidth(ui->clockDrawing->sizeHint().height());
    qDebug() << GetCoordinateOnCircularArc(DegreeToRadian(30), 5);
    QTimer *dataUpdate = new QTimer();
    dataUpdate->setInterval(10);
    connect(dataUpdate, &QTimer::timeout, this, &MainWindow::UpdateInformation);
    dataUpdate->start();

    ui->clockDrawing->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    qDebug() << ui->clockDrawing << event->type();
    if (watched == ui->clockDrawing && event->type() == QEvent::Resize) {
        DrawingClock();
    }
    if (watched == ui->clockDrawing && event->type() == QEvent::Paint) {
        DrawingClock();
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::UpdateInformation()
{
    ui->clockDrawing->repaint();
    //DrawingClock();
}

void MainWindow::DrawingClock()
{
    // 设置控件尺寸
    ui->clockDrawing->setMaximumWidth(ui->clockDrawing->height());
    QPainter painter(ui->clockDrawing);
    //painter.setPen(QPen(Qt::red, 10));
    int width = ui->clockDrawing->width();
    int height = ui->clockDrawing->height();
    // 计算中点坐标
    int centerPoint_X = width / 2;
    int centerPoint_Y = height / 2;
    // 绘制表盘
    int r = centerPoint_X * 0.9;   // 表盘半径
    for(int i = 0; i < 12; i++) {
        // 绘制 12 个点
        QList<int> point_in = GetCoordinateOnCircularArc(DegreeToRadian(30 * i), r * 0.9);
        QList<int> point_out = GetCoordinateOnCircularArc(DegreeToRadian(30 * i), r);
        painter.drawLine(QPoint(centerPoint_X + point_in.at(0), centerPoint_Y + point_in.at(1)),
                         QPoint(centerPoint_X + point_out.at(0), centerPoint_Y + point_out.at(1))
                         );
    }
    // 获取当前时间
    QDateTime nowTime = QDateTime::currentDateTime();
    int second = nowTime.time().second();
    int minute = nowTime.time().minute();
    int hour = nowTime.time().hour();
    // 绘制秒针
    QList<int> secondPoint = GetCoordinateOnCircularArc(DegreeToRadian(second * 360.0 / 60), r * 0.9);
    painter.drawLine(QPoint(centerPoint_X, centerPoint_Y),
                     QPoint(centerPoint_X + secondPoint.at(0), centerPoint_Y + secondPoint.at(1))
                     );
    // 绘制分针
    QList<int> minutePoint = GetCoordinateOnCircularArc(DegreeToRadian(minute * 360.0 / 60), r * 0.7);
    painter.drawLine(QPoint(centerPoint_X, centerPoint_Y),
                     QPoint(centerPoint_X + minutePoint.at(0), centerPoint_Y + minutePoint.at(1))
                     );
    // 绘制时针
    QList<int> hourPoint = GetCoordinateOnCircularArc(DegreeToRadian(hour * 360.0 / 12), r * 0.5);
    painter.drawLine(QPoint(centerPoint_X, centerPoint_Y),
                     QPoint(centerPoint_X + hourPoint.at(0), centerPoint_Y + hourPoint.at(1))
                     );
}

/*
 * 角度制转弧度制
 * α=β*π/180
 */
double MainWindow::DegreeToRadian(double degree)
{
    return degree * M_PI / 180.0;
}

/*
 * 求圆上一点的坐标
 * 以圆心为圆点构建平面直角坐标系，以 r 为半径，以 12 点钟方向为始边，以顺时针方向旋转 α° 求出终边与圆上相交点 F 的坐标(x,y)
 * α∈[0, 2π]
 * r 使用弧度制
 * 推导过程可见源码目录
 */
QList<int> MainWindow::GetCoordinateOnCircularArc(double angle, double r)
{
    int x = sin(angle) * r;  // X 坐标
    int y = -cos(angle) * r;  // Y 坐标
    QList<int> coordinate;
    coordinate.append(x);
    coordinate.append(y);
    return coordinate;
}

MainWindow::~MainWindow()
{
    delete ui;
}
