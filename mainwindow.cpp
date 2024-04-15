#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QScreen>

// 用于钟表绘制
#include <QPainter>
#include <QDateTime>
#include <QThread>

// 引用数学库
#define _USE_MATH_DEFINES
#include <cmath>

#define WindowWidthProportion 5
#define WindowHeightProportion 4

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 隐藏标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 获取屏幕分辨率以设置窗口大小
    QScreen *screen = QGuiApplication::primaryScreen();  // 读取主窗口的分辨率
    int screen_width = screen->geometry().width();
    int screen_height = screen->geometry().height();
    int window_width = screen_width / WindowWidthProportion * 0.96;
    int window_height = screen_height / WindowHeightProportion * 0.96;
    int window_x = screen_width * (WindowWidthProportion - 1) / WindowWidthProportion + screen_width / WindowWidthProportion * 0.02;
    int window_y = screen_height / WindowHeightProportion * 0.02;
    this->resize(window_width, window_height);
    this->move(window_x, window_y);


    QTimer *dataUpdate = new QTimer();
    dataUpdate->setInterval(200);  // 钟表刷新次数
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
    // 刷新钟表
    ui->clockDrawing->repaint();
    // 更新日期
    UpdateTime();
}

void MainWindow::UpdateTime()
{
    QDateTime nowTime = QDateTime::currentDateTime();
    ui->timeNumber->setText("<h1 align=center>" + nowTime.toString("hh:mm:ss") + "</h1><h3 align=center>" + nowTime.toString("yyyy/MM/dd dddd") + "</h3>");
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
    int minsecond = nowTime.time().msec();
    double second = nowTime.time().second() + minsecond / 1000.0;
    double minute = nowTime.time().minute() + second / 60.0;
    double hour = nowTime.time().hour() + minute / 60.0;
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

