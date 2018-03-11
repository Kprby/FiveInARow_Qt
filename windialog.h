#ifndef WINDIALOG_H
#define WINDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class WinDialog;
}

class WinDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WinDialog(const QString &text, QWidget *parent = 0);
	~WinDialog();
	int choice = 0;

private slots:
	void on_RestartButton_clicked();

	void on_BackButton_clicked();

private:
	Ui::WinDialog *ui;
};

#endif // WINDIALOG_H
