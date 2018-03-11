#include "mainwindow.h"
#include "instructiondialog.h"
#include <iostream>
#include <QMenuBar>
#include <QApplication>
#include <QDesktopWidget>
#include <QString>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),

	back(new QAction(tr("悔棋"), this)),
	restart(new QAction(tr("重来"), this)),
	instruction(new QAction(tr("使用说明"), this)),
	hvh(new QAction(tr("人人对战"), this)),
	hva(new QAction(tr("人机对战"), this)),
	easy(new QAction(tr("简单"), this)),
	difficult(new QAction(tr("困难"), this)),

	statusbar(this->statusBar()),

	startWidget(new QWidget),
	layout(new QGridLayout(startWidget)),
	hvhButton(new QPushButton(tr("人人对战"))), hvaButton(new QPushButton(tr("人机对战"))),
	chessBoard(nullptr)
{
	this->setWindowTitle(tr("五子棋——3113005897杨仲铭"));
	QDesktopWidget *desktop = QApplication::desktop();
	this->setGeometry((desktop->width() - 450) / 2, (desktop->height() - 495)/2, 450, 495);

	//设置菜单栏
	QMenuBar *menubar = this->menuBar();
	menuOption = menubar->addMenu(tr("选项"));
	menuOption->setEnabled(false);
	menuOption->addAction(back);
	menuOption->addAction(restart);
	menuOption->addAction(instruction);

	menuModel = menubar->addMenu(tr("模式"));
	hvh->setCheckable(true);
	hva->setCheckable(true);
	menuModel->setEnabled(false);
	menuModel->addAction(hvh);
	menuModel->addAction(hva);

	menuDifficulty = menubar->addMenu(tr("难度"));
	easy->setCheckable(true);
	difficult->setCheckable(true);
	easy->setChecked(false);
	easy->setEnabled(true);
	difficult->setChecked(true);
	difficult->setEnabled(false);
	menuDifficulty->setEnabled(false);
	menuDifficulty->addAction(easy);
	menuDifficulty->addAction(difficult);

	//设置主窗口
	this->setCentralWidget(startWidget);
	hvhButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	hvaButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	layout->addWidget(hvhButton, 0, 0);
	layout->addWidget(hvaButton, 1, 0);

	//连接信号槽
	connect(hvhButton, &QPushButton::clicked, this, &MainWindow::on_hvhButton_clicked);
	connect(hvaButton, &QPushButton::clicked, this, &MainWindow::on_hvaButton_clicked);
	connect(back, &QAction::triggered, this, &MainWindow::on_back_triggered);
	connect(restart, &QAction::triggered, this, &MainWindow::on_restart_triggered);
	connect(instruction, &QAction::triggered, this, &MainWindow::on_instruction_triggered);
	connect(hvh, &QAction::triggered, this, &MainWindow::on_hvh_triggered);
	connect(hva, &QAction::triggered, this, &MainWindow::on_hva_triggered);
	connect(easy, &QAction::triggered, this, &MainWindow::on_easy_triggered);
	connect(difficult, &QAction::triggered, this, &MainWindow::on_difficult_triggered);
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_hvhButton_clicked()
{
	chessBoard = new ChessBoard(0, 0);
	connect(chessBoard, &ChessBoard::SideChanged, this, &MainWindow::on_chessBoard_SideChanged);
	this->setCentralWidget(chessBoard);
	menuOption->setEnabled(true);
	menuModel->setEnabled(true);
	hvh->setChecked(true);
	hvh->setEnabled(false);
	setWindowTitle(tr("人人对战"));
}

void MainWindow::on_hvaButton_clicked()
{
	chessBoard = new ChessBoard(0, 1);
	connect(chessBoard, &ChessBoard::SideChanged, this, &MainWindow::on_chessBoard_SideChanged);
	this->setCentralWidget(chessBoard);
	menuOption->setEnabled(true);
	menuModel->setEnabled(true);
	menuDifficulty->setEnabled(true);
	hva->setChecked(true);
	hva->setEnabled(false);
	setWindowTitle(tr("人机对战(困难)"));
}

void MainWindow::on_back_triggered()
{
	chessBoard->Back();
}

void MainWindow::on_restart_triggered()
{
	chessBoard->Restart();
}

void MainWindow::on_hvh_triggered()
{
	hvh->setChecked(true);
	hvh->setEnabled(false);
	hva->setChecked(false);
	hva->setEnabled(true);
	menuDifficulty->setEnabled(false);
	chessBoard->SetModel(0);
	setWindowTitle(tr("人人对战"));
}

void MainWindow::on_hva_triggered()
{
	hvh->setChecked(false);
	hvh->setEnabled(true);
	hva->setChecked(true);
	hva->setEnabled(false);
	menuDifficulty->setEnabled(true);
	chessBoard->SetModel(1);
	QString title(tr("人机对战"));
	if (chessBoard->GetDifficulty())
	{
		title += tr("(困难)");
	}
	else
	{
		title += tr("(简单)");
	}
	setWindowTitle(title);
}

void MainWindow::on_chessBoard_SideChanged(ChessType chessType)
{
	QString str;
	if (chessBoard->GetModel() == 0)
	{
		if (chessType == black)
			str = tr("轮到黑方");
		else
			str = tr("轮到白方");
	}
	else if (chessBoard->GetModel() == 1)
	{
		if (chessType == black)
			str = tr("轮到黑方(你)");
		else
			str = tr("轮到白方(AI)");
	}
	statusbar->showMessage(str);
}

void MainWindow::on_easy_triggered()
{
	difficult->setChecked(false);
	difficult->setEnabled(true);
	easy->setChecked(true);
	easy->setEnabled(false);
	chessBoard->SetDifficulty(false);
	setWindowTitle(tr("人机对战(简单)"));
}

void MainWindow::on_difficult_triggered()
{
	easy->setChecked(false);
	easy->setEnabled(true);
	difficult->setChecked(true);
	difficult->setEnabled(false);
	chessBoard->SetDifficulty(true);
	setWindowTitle(tr("人机对战(困难)"));
}

void MainWindow::on_instruction_triggered()
{
	InstructionDialog i;
	i.exec();
}
