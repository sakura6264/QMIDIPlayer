#ifndef GOTOWND_H
#define GOTOWND_H

#include <QDialog>

namespace Ui {
class GotoWnd;
}

class GotoWnd : public QDialog {
  Q_OBJECT

public:
  explicit GotoWnd(QWidget *parent = nullptr);
  void init(int max);
  int getresult();
  ~GotoWnd();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::GotoWnd *ui;
  int max;
  int value;
};

#endif // GOTOWND_H
