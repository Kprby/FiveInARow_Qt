#ifndef AI_H
#define AI_H

#include "chess.h"
#include <vector>

class AI
{
private:
	const ChessType (&chess)[15][15];	//15*15棋盘
	const ChessType side; //当前出棋的一方
	const bool difficulty;

	int ValueOfStep(ChessStep step);
	bool GetLine(std::vector<int> &line, const ChessStep &step, const Direction &direction);	//初步测试过关
	void GetLineTypeVector(std::vector<LineType> &lineTypeVector, const ChessStep &step);	//初步测试过关
	void GetLineType(LineType &primary, LineType &secondary, const std::vector<int> &line);	//初步测试过关
	void GetLineType(LineType &lineType, const std::vector<int>::const_iterator &five);
	static bool MoreDense(const std::vector<int>::const_iterator &first, const std::vector<int>::const_iterator &second);
	static void GetLiveSpace(const std::vector<int>::const_iterator &five, int &liveSpace, int &holdSpace);
	int ValueOfNoType(const ChessStep &step, const Direction &direction);

public:
	AI(const ChessType (&chess)[15][15], const ChessType side, const bool difficulty);

	//测试函数
	void PrintLineType(const LineType &primary, const LineType &secondary);
	void PrintLineType(const LineType &l);
	void PrintLineTypeVector(const std::vector<LineType> &lineTypeVector);

	ChessPoint FindBestStep();
};

#endif // AI_H
