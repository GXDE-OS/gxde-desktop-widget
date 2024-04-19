#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QMouseEvent>
#include <QMenu>

#include "weatherinformation.h"
#include "infoutils.h"

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
    QTimer *dataUpdate;
    QTimer *sentence;
    QTimer *systemResourceUpdater;
    QTimer *weatherUpdater;
    WeatherInformation *information;
    long m_cpuAll;
    long m_cpuFree;
    long m_down;
    long m_upload;
    int m_DecimalsNum = 2;
    infoUtils::Sensitive m_Sensitive = infoUtils::Sensitive::Default;
    void UpdateInformation();
    void DrawingClock();
    void UpdateTime();
    void UpdateSystemDeviceResouce();
    void UpdateSentence();
    // 一言
    void ChangePoem();
    void GetSentenceOffLine();
    void ShowPoemText(QJsonValue hitokoto, QJsonValue from, QJsonValue from_who);
    QJsonArray offLineSentence;
    int offLineSentence_count;
    // 数学计算
    QList<int> GetCoordinateOnCircularArc(double angle, double r);
    double DegreeToRadian(double degree);
    // 右键菜单实现
    QMenu *m_mainMenu;
    void mousePressEvent(QMouseEvent *event);
    void ShowAboutWindow();
    void ShowSettingWindow();
};
#endif // MAINWINDOW_H
