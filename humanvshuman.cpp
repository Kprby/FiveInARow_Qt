#include "humanvshuman.h"
#include <QMouseEvent>
#include <QDebug>
#include "windialog.h"

HumanVsHuman::HumanVsHuman(QWidget *parent)
	: ChessBoard(parent)
{

}

void HumanVsHuman::WinInformation()
{
	if (side == black)
	{
		WinDialog winDialog(tr("黑方胜"));
		winDialog.exec();
		if (winDialog.choice == 0)
		{
			Restart();
		}
		else
		{
			Back();
		}
	}
	else if (side == white)
	{
		WinDialog winDialog(tr("白方胜"));
		winDialog.exec();
		if (winDialog.choice == 0)
		{
			Restart();
		}
		else
		{
			Back();
		}
	}
}

void HumanVsHuman::DrawInformation()
{
	WinDialog winDialog(tr("和棋"));
	winDialog.exec();
	if (winDialog.choice == 0)
	{
		Restart();
	}
	else
	{
		Back();
	}
}

void HumanVsHuman::Back()
{
	if (!chessStepVector.empty())
	{
		ChessStep &chessStep = chessStepVector.back();
		chessStepVector.pop_back();
		chess[chessStep.chessPoint.x][chessStep.chessPoint.y] = blank;
		SetSide(chessStep.side);
		update();
	}
}

void HumanVsHuman::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		const ChessPoint chessPoint(QPoint2ChessPoint(e->pos()));
		if ((chessPoint != ChessPoint{-1, -1}) && chess[chessPoint.x][chessPoint.y] == blank)
		{
			PlaceChess(chessPoint);
			if (IsWin(chessPoint))
			{
				WinInformation();
				return;
			}
			if (IsDraw())
			{
				DrawInformation();
				return;
			}
			SetSide((side == black) ? white : black);
			update();
		}
	}
	else
	{
		SetSide((side == black) ? white : black);
	}
}

void HumanVsHuman::enterEvent(QEvent *)
{
	emit SideChanged(side);
}

