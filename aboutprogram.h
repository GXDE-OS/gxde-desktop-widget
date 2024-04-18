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

private:
    Ui::AboutProgram *ui;
};

#endif // ABOUTPROGRAM_H
