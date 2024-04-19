#ifndef ABOUTPROGRAM_H
#define ABOUTPROGRAM_H

#include <QMainWindow>

namespace Ui {
class AboutProgram;
}

class AboutProgram : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutProgram(QWidget *parent = nullptr);
    ~AboutProgram();

private slots:
    void on_m_OK_clicked();

private:
    Ui::AboutProgram *ui;
};

#endif // ABOUTPROGRAM_H
