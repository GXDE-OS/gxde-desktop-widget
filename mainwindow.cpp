#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QScreen>

// 用于一言信息获取
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

// 用于获取系统硬件占用率
#include "infoutils.h"
#include <QNetworkInterface>

// 用于钟表绘制
#include <QPainter>
#include <QDateTime>
#include <QThread>

// 用于获取天气
#include "weatherinformation.h"
#include <QMessageBox>

// 引用数学库
#define _USE_MATH_DEFINES
#include <cmath>

#define WindowWidthProportion 4
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
    this->setMaximumWidth(window_width);
    this->move(window_x, window_y);

    // 资源占用率更新
    systemResourceUpdater = new QTimer();
    systemResourceUpdater->setInterval(2000);
    connect(systemResourceUpdater, &QTimer::timeout, this, &MainWindow::UpdateSystemDeviceResouce);
    systemResourceUpdater->start();

    // 钟表与时间绘制
    dataUpdate = new QTimer();
    dataUpdate->setInterval(200);  // 钟表刷新次数
    connect(dataUpdate, &QTimer::timeout, this, &MainWindow::UpdateInformation);
    dataUpdate->start();

    // 一言绘制
    sentence = new QTimer();
    sentence->setInterval(60 * 1000);
    connect(sentence, &QTimer::timeout, this, &MainWindow::UpdateSentence);
    sentence->start();

    ui->clockDrawing->installEventFilter(this);

    // 读取离线词库
    QFile sentence(":/Poem/poem.json");
    sentence.open(QFile::ReadOnly);
    offLineSentence = QJsonDocument::fromJson(sentence.readAll()).array();
    offLineSentence_count = offLineSentence.count(); // 提前计算数据以减少损耗
    sentence.close();

    UpdateSystemDeviceResouce();
    UpdateSentence();

    // 天气显示
    information = new WeatherInformation();
    connect(information, &WeatherInformation::loadFinished, this, [this](){
         ui->m_weatherShower->setText("<img src='" + this->information->current_weatherIconUrl() + "'>");
    });
    weatherUpdater = new QTimer();
    weatherUpdater->setInterval(60 * 60 * 1000);
    connect(weatherUpdater, &QTimer::timeout, this, [this](){
        this->information->LoadInformation();
    });
    weatherUpdater->start();
    information->LoadInformation();

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

void MainWindow::UpdateSystemDeviceResouce()
{
    // CPU
    long cpuAll = 0;
    long cpuFree = 0;
    infoUtils::cpuRate(cpuAll, cpuFree);
    ui->m_cpuProgressBar->setValue((((cpuAll - m_cpuAll) - (cpuFree - m_cpuFree)) * 100.0 / (cpuAll - m_cpuAll)));
    m_cpuAll = cpuAll;
    m_cpuFree = cpuFree;

    // 内存
    long memory = 0;
    long memoryAll = 0;
    long swap = 0;
    long swapAll = 0;
    infoUtils::memoryRate(memory, memoryAll, swap, swapAll);
    ui->m_memoryProgressBar->setValue(memory * 100.0 / memoryAll);

    // 网速
    long down = 0;
    long upload = 0;
    double downRate = 0;
    double uploadRate = 0;
        infoUtils::RateUnit unit = infoUtils::RateByte;

    infoUtils::netRate(down, upload);
    downRate = infoUtils::autoRateUnits((down - m_down) / (systemResourceUpdater->interval() / 1000), unit);
    QString downUnit = infoUtils::setRateUnitSensitive(unit, m_Sensitive);
    unit = infoUtils::RateByte;
    uploadRate = infoUtils::autoRateUnits((upload - m_upload) / (systemResourceUpdater->interval() / 1000), unit);
    QString uploadUnit = infoUtils::setRateUnitSensitive(unit, m_Sensitive);

    ui->m_netInfo->setText("Net: " +
                           QString::number(downRate) + downUnit +
                           " " + QString::number(uploadRate) + uploadUnit);

    m_down = down;
    m_upload = upload;
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
    QPen pen = painter.pen();
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
    pen.setWidth(1);
    painter.setPen(pen);
    QList<int> secondPoint = GetCoordinateOnCircularArc(DegreeToRadian(second * 360.0 / 60), r * 0.9);
    painter.drawLine(QPoint(centerPoint_X, centerPoint_Y),
                     QPoint(centerPoint_X + secondPoint.at(0), centerPoint_Y + secondPoint.at(1))
                     );
    // 绘制分针
    pen.setWidth(3);
    painter.setPen(pen);
    QList<int> minutePoint = GetCoordinateOnCircularArc(DegreeToRadian(minute * 360.0 / 60), r * 0.7);
    painter.drawLine(QPoint(centerPoint_X, centerPoint_Y),
                     QPoint(centerPoint_X + minutePoint.at(0), centerPoint_Y + minutePoint.at(1))
                     );
    // 绘制时针
    pen.setWidth(5);
    painter.setPen(pen);
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

void MainWindow::UpdateSentence()
{
    ChangePoem();
}

void MainWindow::ChangePoem()
{
    QUrl url("https://v1.hitokoto.cn/?c=d&c=i&c=k");
    QUrlQuery query;
    query.addQueryItem("type", "DESKDICT");
    query.addQueryItem("c", "d");
    query.addQueryItem("c", "i");
    query.addQueryItem("c", "k");
    query.addQueryItem("num", "4");
    query.addQueryItem("ver", "2.0");
    query.addQueryItem("le", "eng");
    query.addQueryItem("doctype", "json");
    url.setQuery(query.toString(QUrl::FullyEncoded));
    qDebug() << url;
    QNetworkRequest request(url);
    QNetworkAccessManager *m_http = new QNetworkAccessManager(this);
    QNetworkReply *reply = m_http->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, m_http](){
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            GetSentenceOffLine(); // 如果无法访问则使用离线数据库
            return;
        }
        QByteArray data = reply->readAll();
        qDebug() << data;
        // 解析数据
        // 数据例子：
        /*
        {
            "id": 6256,
            "uuid": "b2f3834f-5f52-4b62-bb3c-f9fb98901419",
            "hitokoto": "我们在努力扩大自己，以靠近，以触及我们自身以外的世界。",
            "type": "k",
            "from": "豪尔赫·路易斯·博尔赫斯",
            "from_who": "博尔赫斯谈话录",
            "creator": "Irony",
            "creator_uid": 4464,
            "reviewer": 1044,
            "commit_from": "web",
            "created_at": "1592126239",
            "length": 27
        }*/
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject object = document.object();
        QJsonValue hitokoto = object.value("hitokoto");
        QJsonValue from = object.value("from");
        QJsonValue from_who = object.value("from_who");
        this->ShowPoemText(hitokoto, from, from_who);
        delete m_http;
    });
}

void MainWindow::GetSentenceOffLine()
{
    // 通过随机数随机选择句子
    srand(time(0)); // 使用时间种子
    int id = rand() % (offLineSentence_count - 1) + 1;
    QJsonObject object = offLineSentence.at(id).toObject();
    QJsonValue hitokoto = object.value("hitokoto");
    QJsonValue from = object.value("from");
    QJsonValue from_who = object.value("from_who");
    this->ShowPoemText(hitokoto, from, from_who);
}

void MainWindow::ShowPoemText(QJsonValue hitokoto, QJsonValue from, QJsonValue from_who)
{
    // 显示文本
    QString showText = "<p>" + hitokoto.toString() + "</p><p align='right'>";
    QString who = "";
    if (!from.isNull()) {
        who += "《" + from.toString() + "》";
    }
    if (!from_who.isNull()) {
        who += from_who.toString();
    }
    if (who.length() > 0) {
        // 有内容，添加破折号
        who = "——" + who;
    }
    showText += who + "</p>";
    ui->sentenceText->setText(showText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

