#include "chessboard.h"
#include <QPainter>
#include <QPoint>
#include <cstring>
#include <QDebug>
#include <QMouseEvent>
#include <QString>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ChessBoard::ChessBoard(QWidget *parent, int model)
	: QWidget(parent), model(model)
{
	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	this->setMinimumSize(chessRadius * 15 * 2, chessRadius * 15 * 2); //设置最小窗口大小
	//绘制背景
	this->setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Window, QColor(205, 149, 12));
	this->setPalette(palette);
}

ChessType ChessBoard::GetSide()
{
	return side;
}

void ChessBoard::SetSide(const ChessType s)
{
	side = s;
	emit SideChanged(s);
}

int ChessBoard::GetModel()
{
	return model;
}

void ChessBoard::SetModel(const int m)
{
	model = m;
	Restart();
}

void ChessBoard::Restart()
{
	chessStepVector.clear();
	std::memset(chess, 0, sizeof(chess));
	SetSide(black);
	update();
}

void ChessBoard::Back()
{
	if (!chessStepVector.empty())
	{
		if (model == 0)
		{
			const ChessStep &chessStep = chessStepVector.back();
			chessStepVector.pop_back();
			chess[chessStep.chessPoint.x][chessStep.chessPoint.y] = blank;
			SetSide(chessStep.side);
		}
		else if (model == 1)
		{
			ChessStep chessStep;
			do
			{
				chessStep = chessStepVector.back();
				chessStepVector.pop_back();
				chess[chessStep.chessPoint.x][chessStep.chessPoint.y] = blank;
			}
			while (!chessStepVector.empty() && chessStep.side != black);
			SetSide(black);
		}
		update();
	}
}

void ChessBoard::WinInformation()
{
	QString title;
	if (side == black)
	{
		title = tr("黑方胜");
	}
	else if (side == white)
	{
		title = tr("白方胜");
	}
	WinDialog winDialog(title);
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

void ChessBoard::DrawInformation()
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

void ChessBoard::SetDifficulty(const bool d)
{
	difficulty = d;
}

bool ChessBoard::GetDifficulty()
{
	return difficulty;
}

void ChessBoard::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (HumanPlaceChess(e))
			return;
	}
	else if (e->button() == Qt::RightButton)
	{
		SetSide((side == black) ? white : black);
	}
	else if (e->button() == Qt::MiddleButton)
	{
		if (AIPlaceChess(true))
			return;
	}

	if (model == 1)
	{
		if (AIPlaceChess(difficulty))
			return;
	}
}

void ChessBoard::enterEvent(QEvent *)
{
	emit SideChanged(side);
}

bool ChessBoard::IsWin(const ChessPoint &chessPoint)
{
	//方向数组
	int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};

	int count;	//记录两条阴线、两条阳线上的连子数
	ChessType currentSide = chess[chessPoint.x][chessPoint.y];

	for (int i = 0; i < 4; ++i)
	{
		int direct = i;
		count = 1;
		int x = chessPoint.x + dx[direct];
		int y = chessPoint.y + dy[direct];
		while (x >= 0 && x <= 14 && y >= 0 && y <= 14 && chess[x][y] == currentSide)
		{
			++count;
			x += dx[direct];
			y += dy[direct];
		}

		direct += 4;
		x = chessPoint.x + dx[direct];
		y = chessPoint.y + dy[direct];
		while (x >= 0 && x <= 14 && y >= 0 && y <= 14 && chess[x][y] == currentSide)
		{
			++count;
			x += dx[direct];
			y += dy[direct];
		}
		if (count >= 5)
		{
			return true;
		}

	}//end for
	return false;
}

bool ChessBoard::IsDraw()
{
	AI ai(chess, side, difficulty);
	ChessPoint chessPoint = ai.FindBestStep();
	if (chessPoint == ChessPoint{-1, -1})
	{
		DrawInformation();
		return true;
	}
	else
		return false;
}

void ChessBoard::paintEvent(QPaintEvent *)
{
	const int h = height();
	const int w = width();
	if (h <= w)
	{
		resize(h, h);
		chessRadius = h / ( 15 * 2 );	//设置半径
	}
	else
	{
		resize(w, w);
		chessRadius = w / ( 15 * 2 );	//设置半径
	}
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	DrawGrid(painter);
	DrawChess(painter);
}

void ChessBoard::DrawGrid(QPainter &painter)
{
	painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
	for (int i = 0; i < 15; ++i)
	{
		painter.drawLine(ChessPoint2QPoint({i, 0}), ChessPoint2QPoint({i, 14}));
		painter.drawLine(ChessPoint2QPoint({0, i}), ChessPoint2QPoint({14, i}));
	}
}

void ChessBoard::DrawChess(QPainter &painter)
{
	for (int x = 0; x < 15; ++x)
	{
		for (int y = 0; y < 15; ++y)
		{
			if(chess[x][y] == black)
			{//黑方
				painter.setBrush(Qt::black);
				painter.setPen(QPen(QBrush(Qt::transparent), 0));
				painter.drawEllipse(ChessPoint2QPoint({x, y}), chessRadius, chessRadius);
			}
			else if (chess[x][y] == white)
			{//白方
				painter.setBrush(Qt::white);
				painter.setPen(QPen(QBrush(Qt::transparent), 0));
				painter.drawEllipse(ChessPoint2QPoint({x, y}), chessRadius, chessRadius);
			}
		}
	}

	if (!chessStepVector.empty())
	{
		ChessPoint chessPoint = chessStepVector.back().chessPoint;
		QPoint point = ChessPoint2QPoint(chessPoint);
		painter.setBrush(Qt::transparent);
		painter.setPen(QPen(QBrush(Qt::red), 2));
		painter.drawRect(point.x() - chessRadius, point.y() - chessRadius, 2 * chessRadius, 2 * chessRadius);
	}
}

bool ChessBoard::HumanPlaceChess(QMouseEvent *e)
{
	ChessPoint chessPoint(QPoint2ChessPoint(e->pos()));
	if ((chessPoint != ChessPoint{-1, -1}) && chess[chessPoint.x][chessPoint.y] == blank)
	{
		PlaceChess(chessPoint);
		if (IsWin(chessPoint))
		{
			WinInformation();
			return true;
		}
		else if (IsDraw())
		{
			DrawInformation();
			return true;
		}
		SetSide((side == black) ? white : black);
		update();
		return false;
	}
	else
	{
		return true;
	}
}

bool ChessBoard::AIPlaceChess(const bool difficulty)
{
	AI ai(chess, side, difficulty);
	const ChessPoint chessPoint(ai.FindBestStep());
	if (chessPoint == ChessPoint{-1, -1})
	{
		DrawInformation();
		return true;
	}
	PlaceChess(chessPoint);
	if (IsWin(chessPoint))
	{
		WinInformation();
		return true;
	}
	SetSide((side == black) ? white : black);
	update();
	return false;
}

QPoint ChessBoard::ChessPoint2QPoint(const ChessPoint &chessPoint)
{
	QPoint point(chessRadius + 2 * chessRadius * chessPoint.x, chessRadius + 2 * chessRadius * chessPoint.y);
	return point;
}

ChessPoint ChessBoard::QPoint2ChessPoint(const QPoint &point)
{
	int x, y, rx, ry;

	x = (point.x() - chessRadius) / (2 * chessRadius);
	y = (point.y() - chessRadius) / (2 * chessRadius);
	rx = (point.x() - chessRadius) % (2 * chessRadius);
	ry = (point.y() - chessRadius) % (2 * chessRadius);

	if (rx > chessRadius) ++x;
	if (ry > chessRadius) ++y;

	if (x >= 0 && x <= 14 && y >= 0 && y <= 14)
	{
		QPoint p(ChessPoint2QPoint({x, y}));
		int dx, dy, distance;
		dx = p.x() - point.x();
		dy = p.y() - point.y();
		distance = dx * dx + dy * dy;
		if (distance < chessRadius * chessRadius)
		{
			return {x, y};
		}
	}

	return {-1, -1};
}

void ChessBoard::PlaceChess(const ChessPoint &chessPoint)
{
	chess[chessPoint.x][chessPoint.y] = side;
	chessStepVector.push_back({chessPoint, side});
}
