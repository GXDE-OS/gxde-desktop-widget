#include "aboutprogram.h"
#include "ui_aboutprogram.h"

AboutProgram::AboutProgram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutProgram)
{
    ui->setupUi(this);
}

AboutProgram::~AboutProgram()
{
    delete ui;
}
