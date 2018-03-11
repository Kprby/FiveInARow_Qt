#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include "chessboard.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void on_hvhButton_clicked();
	void on_hvaButton_clicked();
	void on_back_triggered();
	void on_restart_triggered();
	void on_hvh_triggered();
	void on_hva_triggered();
	void on_chessBoard_SideChanged(ChessType chessType);
	void on_easy_triggered();
	void on_difficult_triggered();
	void on_instruction_triggered();

private:
	QMenu *menuOption;
	QMenu *menuModel;
	QMenu *menuDifficulty;
	QAction *back;
	QAction *restart;
	QAction *instruction;
	QAction *hvh;
	QAction *hva;
	QAction *easy;
	QAction *difficult;

	QStatusBar *statusbar;

	QWidget *startWidget;
	QGridLayout *layout;
	QPushButton *hvhButton, *hvaButton;
	ChessBoard *chessBoard;
};

#endif // MAINWINDOW_H
