#include "humanvsai.h"
#include <QMouseEvent>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <QDebug>
#include "windialog.h"

HumanVsAI::HumanVsAI()
{

}

void HumanVsAI::WinInformation()
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

void HumanVsAI::DrawInformation()
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

void HumanVsAI::Back()
{
	if (!chessStepVector.empty())
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
		update();
	}
}

void HumanVsAI::SwitchDifficulty()
{
	difficulty = !difficulty;
}

int HumanVsAI::ValueOfStep(ChessStep step)
{
	std::vector<LineType> lineTypeVector;
	//计算假设我方下了这步棋的分值
	int our_value = 0;
	GetLineTypeVector(lineTypeVector, step);
	int i = 0;
	for (const LineType &l : lineTypeVector)
	{
		our_value += ValueOfLineType(l);
		if (difficulty && i < 2 && our_value >= 4500)	//我方必胜棋型判断
		{
			int temp = our_value;
			if (our_value <= 7000)	//棋型为【三三】
				temp = 250000;
			else if (our_value <= 9500)	//棋型为【四三】
				temp = 500000;
			else if (our_value <= 12000)	//棋型为【四四】
				temp = 1 * 10e6;
			our_value = temp;
			break;
		}
		++i;
	}

	//计算假设敌方下了这步棋的分值
	step.side = (step.side == black) ? white : black;
	int other_value = 0;
	GetLineTypeVector(lineTypeVector, step);
	i = 0;
	int j = 0;	//若我方棋型与敌方棋型相同时，应是我方分值略高。j用于从敌方棋型的分值中减去一点
	for (const LineType &l : lineTypeVector)
	{
		if (l.type != 0 && l.type != -1)	//不需要计算敌方的【活一】和【死棋】
		{
			other_value += ValueOfLineType(l);
			++j;
		}
		if (difficulty && i < 2 && other_value >= 4500)	//敌方必胜棋型判断
		{
			int temp = other_value;
			if (other_value <= 7000)	//棋型为【三三】
				temp = 250000;
			else if (other_value <= 9500)	//棋型为【四三】
				temp = 500000;
			else if (other_value <= 12000)	//棋型为【四四】
				temp = 1 * 10e6;
			other_value = temp;
			break;
		}
		++i;
	}

	if (our_value >= 250000)
	{
		if (our_value >= other_value)
			return our_value;
		else
			return other_value - j;
	}
	else
	{
		if (other_value >= 250000)
		{
			return other_value - j;
		}
	}

	return our_value + other_value - j;
}

int HumanVsAI::ValueOfLineType(const HumanVsAI::LineType &l)
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

bool HumanVsAI::GetLine(std::vector<int> &line, const ChessStep step, const Direction direction)
{
	//line: -1代表边界或对方的棋，0代表空，1代表我方的棋

	int j = 0, lenth = 0;
	line.clear();
	ChessPoint chessPoint = step.chessPoint;

	//chessPoint走到负方向的尽头
	do
	{
		chessPoint = chessPoint - direction;
		++j;
	} while (j < 5 && IsInBounds(chessPoint) && (step.side == chess[chessPoint.x][chessPoint.y] || blank == chess[chessPoint.x][chessPoint.y]));

	//负方向边界判定
	if (!IsInBounds(chessPoint))
	{
		line.push_back(-1);
	}
	else if (step.side == chess[chessPoint.x][chessPoint.y])
	{
		line.push_back(1);
	}
	else if (blank == chess[chessPoint.x][chessPoint.y])
	{
		line.push_back(0);
	}
	else
	{
		line.push_back(-1);
	}

	//chessPoint向正方向走回原点
	while (j > 1)
	{
		chessPoint = chessPoint + direction;
		if (step.side == chess[chessPoint.x][chessPoint.y])
		{
			line.push_back(1);
		}
		else if (blank == chess[chessPoint.x][chessPoint.y])
		{
			line.push_back(0);
		}
		else
		{
			std::cerr << "ValueOfStep error\n";
		}
		++lenth;
		--j;
	}

	//回到原点
	chessPoint = chessPoint + direction;
	line.push_back(1);
	++lenth;
	j = 0;

	//chessPoint走到正方向的尽头
	chessPoint = chessPoint + direction;
	while (j < 4 && IsInBounds(chessPoint) && (step.side == chess[chessPoint.x][chessPoint.y] || blank == chess[chessPoint.x][chessPoint.y]))
	{
		if (step.side == chess[chessPoint.x][chessPoint.y])
		{
			line.push_back(1);
		}
		else if (blank == chess[chessPoint.x][chessPoint.y])
		{
			line.push_back(0);
		}
		else
		{
			std::cerr << "ValueOfStep error\n";
		}
		++lenth;
		chessPoint = chessPoint + direction;
		++j;
	}

	//正方向边界判定
	if (!IsInBounds(chessPoint))
	{
		line.push_back(-1);
	}
	else if (step.side == chess[chessPoint.x][chessPoint.y])
	{
		line.push_back(1);
	}
	else if (blank == chess[chessPoint.x][chessPoint.y])
	{
		line.push_back(0);
	}
	else
	{
		line.push_back(-1);
	}
	//line已完成

	//是否死棋
	if (lenth >= 5)
		return true;
	else
		return false;
}

void HumanVsAI::GetLineTypeVector(std::vector<HumanVsAI::LineType> &lineTypeVector, const ChessStep step)
{
	lineTypeVector.clear();
	const Direction direction[4] =
	{
		{1, 0}, {1, 1}, {0, 1}, {1, -1}
	};

	for (int i = 0; i < 4; ++i)	//遍历4个方向
	{
		std::vector<int> line;	//-1代表边界或对方的棋，0代表空，1代表我方的棋
		bool live = GetLine(line, step, direction[i]);
		if (live)
		{
			LineType primary, secondary;
			GetLineType(primary, secondary, line);

			if (primary.type == 0)
			{
				primary.count = ValueOfNoType(step, direction[i]);
			}

			lineTypeVector.push_back(primary);

			if (secondary.type != 0)
			{
				lineTypeVector.push_back(secondary);
			}
		}
		else
		{
			lineTypeVector.push_back({-1, 0});
		}
	}//end for 遍历4个方向

	std::sort(lineTypeVector.rbegin(), lineTypeVector.rend());
}

void HumanVsAI::GetLineType(HumanVsAI::LineType &primary, HumanVsAI::LineType &secondary, const std::vector<int> &line)
{
	//请保证line不为死棋
	if (line.size() < 7)
	{
		std::cerr << "line error" << std::endl;
		return;
	}

	const std::vector<int>::const_iterator begin = line.cbegin() + 1, end = line.cend() - 2;
	std::vector<int>::const_iterator five = begin, betterFive = five;	//代表以迭代器为首连续5个棋
	int count = 0, betterCount;//代表连续5个棋的同色棋数

	for (int i = 0; i < 5; ++i)
	{
		count += *(five + i);
	}
	betterCount = count;

	++five;
	bool isNewFive = false;
	while (five + 4 <= end)
	{
		if (*(five - 1) == 1)
		{
			isNewFive = true;
		}
		count -= *(five - 1);
		count += *(five + 4);
		if (count > betterCount)
		{
			betterCount = count;
			betterFive = five;
			isNewFive = false;
		}
		else if (count == betterCount && isNewFive)
		{
			if (MoreDense(five, betterFive))
			{
				betterCount = count;
				betterFive = five;
				isNewFive = false;
			}
		}
		++five;
	}

//	if (betterFive == begin)
//	{
//		std::cout << "right" << std::endl;
//	}

	//判定是否有双棋型
	if (end - begin == 8 && betterFive == begin)
	{
//		std::cout << "right" << std::endl;
		GetLineType(primary, begin);
		GetLineType(secondary, begin + 4);
	}
	else if (end - begin == 8 && betterFive + 4 == end)
	{
		GetLineType(primary, betterFive);
		GetLineType(secondary, begin);
	}
	else
	{
		GetLineType(primary, betterFive);
		secondary.type = 0;
		secondary.count = 1;
	}

	if (primary.type != 1 || secondary.type != 1)
	{
		secondary.type = 0;
		secondary.count = 1;
	}

}

void HumanVsAI::GetLineType(HumanVsAI::LineType &lineType, const std::vector<int>::const_iterator &five)
{
	//five代表以迭代器为首连续5个棋
	//求five的棋型，以lineType返回
	int count = 0;
	for (int i = 0; i < 5; ++i)
	{
		count += *(five + i);
	}

	lineType.count = count;
	int liveSpace, holdSpace;
	GetLiveSpace(five, liveSpace, holdSpace);

	//lineType.type : -1代表死，4代表连活，3代表跳活，2代表大跳活，1代表冲或眠，0代表没有棋型
	if (count == 1)
	{
		lineType.type = 0;
		return;
	}
	if (liveSpace == 5)
	{
		lineType.type = 1;
		return;
	}
	else if (liveSpace < 5)
	{
		std::cerr << "GetLiveSpace error" << std::endl;
	}

	switch (count)
	{
	case 5:
		break;
	case 4:
		lineType.type = 4;
		break;
	case 3:
		if (holdSpace == 3)
			lineType.type = 4;
		else if (holdSpace == 4)
			lineType.type = 3;
		else
			std::cerr << "GetLiveSpace error" << std::endl;
		break;
	case 2:
		if (holdSpace == 2)
			lineType.type = 4;
		else if (holdSpace == 3)
			lineType.type = 3;
		else if (holdSpace == 4)
			lineType.type = 2;
		else
			std::cerr << "GetLiveSpace error" << std::endl;
		break;
	case 1:
		lineType.type = 0;
		break;
	default:
		std::cerr << "GetLineType error 1" << std::endl;
	}
}

bool HumanVsAI::MoreDense(const std::vector<int>::const_iterator &first, const std::vector<int>::const_iterator &second)
{
	//first、second代表以迭代器为首连续5个棋，请保证first、second的同色棋数相等
	//比较first、second的密度
	std::vector<int>::const_iterator first_begin = first;
	while(*first_begin != 1)
	{
		++first_begin;
	}
	std::vector<int>::const_iterator first_end = first + 4;
	while(*first_end != 1)
	{
		--first_end;
	}

	std::vector<int>::const_iterator second_begin = second;
	while(*second_begin != 1)
	{
		++second_begin;
	}
	std::vector<int>::const_iterator second_end = second + 4;
	while(*second_end != 1)
	{
		--second_end;
	}

	if (first_end - first_begin < second_end - second_begin)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void HumanVsAI::GetLiveSpace(const std::vector<int>::const_iterator &five, int &liveSpace, int &holdSpace)
{
	std::vector<int>::const_iterator five_begin = five;
	while(*five_begin != 1)
	{
		++five_begin;
	}
	std::vector<int>::const_iterator five_end = five + 4;
	while(*five_end != 1)
	{
		--five_end;
	}

	holdSpace = five_end - five_begin + 1;

	std::vector<int>::const_iterator b = five_end - 4, e = five_begin + 4;
	while (five_begin >= b && *five_begin != -1)
	{
		--five_begin;
	}
	while (five_end <= e && *five_end != -1)
	{
		++five_end;
	}

	liveSpace = five_end - five_begin - 1;
}

int HumanVsAI::ValueOfNoType(const ChessStep step, const Direction direction)
{
	ChessType otherSide = (step.side == white) ? black : white;
	ChessPoint left = step.chessPoint - direction;
	ChessPoint right = step.chessPoint + direction;
	while (IsInBounds(left) && IsInBounds(right)
		   && chess[left.x][left.y] != otherSide
		   && chess[right.x][right.y] != otherSide)
	{
		left = left - direction;
		right = right + direction;
	}

	int dx = std::abs(right.x - left.x);
	int dy = std::abs(right.y - left.y);
	if (dx != 0)
		return dx / 2 - 1;
	else
		return dy / 2 - 1;
}

ChessPoint HumanVsAI::FindBestStep()
{
	ChessPoint best{-1, -1};
	int bestValue = -100, value;
	for (int x = 0; x < 15; ++x)
	{
		for (int y = 0; y < 15; ++y)
		{
			if (chess[x][y] == blank)
			{
				ChessPoint temp{x, y};
				value = ValueOfStep({temp, side});
				if (bestValue < value)
				{
					bestValue = value;
					best = temp;
				}
			}
		}
	}
	if (bestValue == -28)
		return {-1, -1};
	return best;
}

void HumanVsAI::PrintLineType(const HumanVsAI::LineType &primary, const HumanVsAI::LineType &secondary)
{
	std::cout << "primary: " << primary.count;
	switch (primary.type)
	{
	case 4:
		std::cout << " lian huo";
		break;
	case 3:
		std::cout << " tiao huo";
		break;
	case 2:
		std::cout << " da tiao huo";
		break;
	case 1:
		std::cout << " mian";
		break;
	case 0:
		std::cout << " none";
		break;
	default:
		std::cout << " error" << primary.type;
		break;
	}
	std::cout << std::endl;

	if (secondary.type != 0)
	{
		std::cout << "secondary: " << secondary.count;
		switch (secondary.type)
		{
		case 4:
			std::cout << " lian huo";
			break;
		case 3:
			std::cout << " tiao huo";
			break;
		case 2:
			std::cout << " da tiao huo";
			break;
		case 1:
			std::cout << " mian";
			break;
		default:
			std::cout << " error";
			break;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void HumanVsAI::PrintLineType(const HumanVsAI::LineType &l)
{
	if (l.type == -1)
	{std::cout << "dead" << std::endl;return;}

	if (l.type != 0)
		std::cout << l.count;
	switch (l.type)
	{
	case 4:
		std::cout << " lian huo";
		break;
	case 3:
		std::cout << " tiao huo";
		break;
	case 2:
		std::cout << " da tiao huo";
		break;
	case 1:
		std::cout << " mian";
		break;
	case 0:
		std::cout << "notype value:" << l.count;
		break;
	default:
		std::cout << " error" << l.type;
		break;
	}
	std::cout << std::endl;
}

void HumanVsAI::PrintLineTypeVector(const std::vector<HumanVsAI::LineType> &lineTypeVector)
{
	for (const LineType &l : lineTypeVector)
	{
		PrintLineType(l);
	}
	std::cout << std::endl;
}

void HumanVsAI::mousePressEvent(QMouseEvent *e)
{
	ChessPoint chessPoint;
	if (e->button() == Qt::LeftButton)
	{
		chessPoint = QPoint2ChessPoint(e->pos());
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
		else
		{
			return;
		}
	}
	else
	{
		SetSide((side == black) ? white : black);
	}

	chessPoint = FindBestStep();
	if (chessPoint == ChessPoint{-1, -1})
	{
		DrawInformation();
		return;
	}
	PlaceChess(chessPoint);
	if (IsWin(chessPoint))
	{
		WinInformation();
		return;
	}
	SetSide((side == black) ? white : black);
	update();
}

void HumanVsAI::enterEvent(QEvent *)
{
	emit SideChanged(side);
}

bool HumanVsAI::LineType::operator <(const HumanVsAI::LineType &l) const
{
	return HumanVsAI::ValueOfLineType(*this) < HumanVsAI::ValueOfLineType(l);
}
