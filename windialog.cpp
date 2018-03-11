#include "windialog.h"
#include "ui_windialog.h"

WinDialog::WinDialog(const QString &text, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::WinDialog)
{
	ui->setupUi(this);
	ui->label->setText(text);
}

WinDialog::~WinDialog()
{
	delete ui;
}

void WinDialog::on_RestartButton_clicked()
{
	choice = 0;
	close();
}

void WinDialog::on_BackButton_clicked()
{
	choice = 1;
	close();
}
