#ifndef CHESS_H
#define CHESS_H

#include <QPoint>

enum ChessType {blank, black, white};

struct Direction
{
	int dx;
	int dy;
};

struct ChessPoint
{
	int x;
	int y;

	bool operator !=(const ChessPoint &chessPoint) const
	{
		if (x != chessPoint.x || y != chessPoint.y)
			return true;
		else
			return false;
	}

	ChessPoint operator +(const Direction dir) const
	{
		ChessPoint chessPoint{0, 0};
		chessPoint.x = this->x + dir.dx;
		chessPoint.y = this->y + dir.dy;
		return chessPoint;
	}

	ChessPoint operator -(const Direction dir) const
	{
		ChessPoint chessPoint{0, 0};
		chessPoint.x = this->x - dir.dx;
		chessPoint.y = this->y - dir.dy;
		return chessPoint;
	}

	bool operator ==(const ChessPoint &chessPoint) const
	{
		return x == chessPoint.x && y == chessPoint.y;
	}

	bool IsInBounds() const
	{
		if (x >= 0 && x <= 14 && y >= 0 && y <= 14)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct ChessStep
{
	ChessPoint chessPoint;
	ChessType side;
};

struct LineType;
inline int ValueOfLineType(const LineType &l);

struct LineType	//代表在一条阳线或阴线上连续相邻的5个点的棋型
{
	int type;	//-1代表死，4代表连活，3代表跳活，2代表大跳活，1代表冲或眠，0代表没有棋型
	int count;	//若(type!=-1&&type!=0)则代表同色棋子的数量，
				//若type==0则代表分数

	bool operator <(const LineType &l) const
	{
		return ValueOfLineType(*this) < ValueOfLineType(l);
	}

	int Value() const
	{
		if (type == -1)
			return -7;
		else if (type == 0)
			return count;

		switch (count)
		{
		case 5:
			return 1 * 10e7;
		case 4:
			if (type == 4)
				return 1 * 10e6;
			else
				return 3600;
		case 3:
			switch (type)
			{
			case 4: return 3500;
			case 3: return 2250;
			case 1: return 358;
			}
		case 2:
			switch (type)
			{
			case 4: return 750;
			case 3: return 500;
			case 2: return 179;
			case 1: return 60;
			}
		}

		//防止警告
		return 0;
	}
};

inline int ValueOfLineType(const LineType &l)
{
	if (l.type == -1)
		return -7;
	else if (l.type == 0)
		return l.count;

	switch (l.count)
	{
	case 5:
		return 1 * 10e7;
	case 4:
		if (l.type == 4)
			return 1 * 10e6;
		else
			return 3600;
	case 3:
		switch (l.type)
		{
		case 4: return 3500;
		case 3: return 2250;
		case 1: return 358;
		}
	case 2:
		switch (l.type)
		{
		case 4: return 750;
		case 3: return 500;
		case 2: return 179;
		case 1: return 60;
		}
	}

	//防止警告
	return 0;
}

#endif // CHESS_H
