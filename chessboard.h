#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <vector>
#include "chess.h"
#include "ai.h"
#include "instructiondialog.h"
#include "windialog.h"

class ChessBoard : public QWidget
{
	Q_OBJECT

protected:
	using ChessStepVector = std::vector<ChessStep>;
	ChessType chess[15][15]{};	//15*15棋盘
	int chessRadius = 15;	//棋子半径
	ChessType side = black; //当前出棋的一方
	ChessStepVector chessStepVector;

	int model;	//0代表人人对战，1代表人机对战
	bool difficulty = true;

	void PlaceChess(const ChessPoint &chessPoint);
	bool IsWin(const ChessPoint &chessPoint);
	bool IsDraw();

	virtual void paintEvent(QPaintEvent *) override;//绘制棋盘
	void DrawGrid(QPainter &painter); // 绘制网格
	void DrawChess(QPainter &painter); // 绘制棋子

	bool HumanPlaceChess(QMouseEvent *e);
	bool AIPlaceChess(const bool difficulty);

	QPoint ChessPoint2QPoint(const ChessPoint &chessPoint);
	ChessPoint QPoint2ChessPoint(const QPoint &point);

signals:
	void SideChanged(ChessType);

public:
	ChessBoard(QWidget *parent = 0, int model = 0);
	~ChessBoard() = default;

	ChessType GetSide();
	void SetSide(const ChessType s);

	int GetModel();
	void SetModel(const int m);

	void SetDifficulty(const bool d);
	bool GetDifficulty();

	void Restart();	//清空棋子
	void Back();	//悔棋
	void WinInformation();
	void DrawInformation();

	virtual void mousePressEvent(QMouseEvent *e) override; // 鼠标点击棋盘时候触发
	virtual void enterEvent(QEvent *) override;
};

#endif // CHESSBOARD_H
