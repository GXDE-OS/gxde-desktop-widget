#include "aboutprogram.h"
#include "ui_aboutprogram.h"

#include <QDateTime>

AboutProgram::AboutProgram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutProgram)
{
    ui->setupUi(this);
    ui->m_aboutInfo->setText(ui->m_aboutInfo->text().replace("{TIME}", QDateTime::currentDateTime().toString("yyyy")));
}

AboutProgram::~AboutProgram()
{
    delete ui;
}

void AboutProgram::on_m_OK_clicked()
{
    this->close();
}

