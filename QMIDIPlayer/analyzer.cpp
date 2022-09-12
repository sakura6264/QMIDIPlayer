#include "analyzer.h"
#include "ui_analyzer.h"
/*
 * The window to show the informations of the midi file.
 * Doesn't have too much functions.
 */
Analyzer::Analyzer(QWidget *parent) : QDialog(parent), ui(new Ui::Analyzer) {
  ui->setupUi(this);
}

Analyzer::~Analyzer() { delete ui; }

void Analyzer::setString(const QString &str) { ui->label->setText(str); }

void Analyzer::on_pushButtonCopy_clicked() {
  QApplication::clipboard()->setText(ui->label->text());
  QMessageBox::about(this, QString("Copy"), QString("Result Has Been Copied."));
}
