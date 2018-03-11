#include "instructiondialog.h"
#include "ui_instructiondialog.h"

InstructionDialog::InstructionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InstructionDialog)
{
	ui->setupUi(this);
}

InstructionDialog::~InstructionDialog()
{
	delete ui;
}

void InstructionDialog::on_pushButton_clicked()
{
	close();
}
