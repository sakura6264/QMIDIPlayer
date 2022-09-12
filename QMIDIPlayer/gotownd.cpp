#include "gotownd.h"
#include "ui_gotownd.h"
/* The window to allow user to input the time they want to jump.
 * At the same time, prevent them to input a wrong time.
 */

GotoWnd::GotoWnd(QWidget *parent) : QDialog(parent), ui(new Ui::GotoWnd) {
  ui->setupUi(this);
}

void GotoWnd::init(int max) {
  this->max = max;
  ui->spinBoxM->setMaximum(this->max / 3000);
  ui->spinBoxS->setMaximum(this->max < 3000 ? this->max / 50 : 60);
}

int GotoWnd::getresult() { return value; }

GotoWnd::~GotoWnd() { delete ui; }

void GotoWnd::on_buttonBox_accepted() {
  this->value = ui->spinBoxM->value() * 60 * 50 + ui->spinBoxS->value() * 50;
}

void GotoWnd::on_buttonBox_rejected() { this->value = -1; }
