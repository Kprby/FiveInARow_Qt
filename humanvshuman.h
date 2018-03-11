#ifndef HUMANVSHUMAN_H
#define HUMANVSHUMAN_H

#include "chessboard.h"

class HumanVsHuman : public ChessBoard
{
	Q_OBJECT

public:
	HumanVsHuman(QWidget *parent = 0);
	~HumanVsHuman() = default;

	void WinInformation();
	void DrawInformation();
	void Back();	//悔棋

	virtual void mousePressEvent(QMouseEvent *e) override; // 鼠标点击棋盘时候触发
	virtual void enterEvent(QEvent *) override;
};

#endif // HUMANVSHUMAN_H
