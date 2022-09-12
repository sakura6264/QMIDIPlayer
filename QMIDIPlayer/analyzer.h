#ifndef ANALYZER_H
#define ANALYZER_H

#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Analyzer;
}

class Analyzer : public QDialog {
  Q_OBJECT

public:
  explicit Analyzer(QWidget *parent = nullptr);
  ~Analyzer();
  void setString(const QString &str);

private slots:
  void on_pushButtonCopy_clicked();

private:
  Ui::Analyzer *ui;
};

#endif // ANALYZER_H
