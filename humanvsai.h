#ifndef HUMANVSAI_H
#define HUMANVSAI_H

#include "chessboard.h"

class HumanVsAI : public ChessBoard
{
	Q_OBJECT

private:
	struct LineType	//代表在一条阳线或阴线上连续相邻的5个点的棋型
	{
		int type;	//-1代表死，4代表连活，3代表跳活，2代表大跳活，1代表冲或眠，0代表没有棋型
		int count;	//若(type!=-1&&type!=0)则代表同色棋子的数量，
					//若type==0则代表分数

		bool operator <(const LineType &l) const;
	};

	bool difficulty = true;

	int ValueOfStep(ChessStep step);
	static int ValueOfLineType(const LineType &l);
	bool GetLine(std::vector<int> &line, const ChessStep step, const Direction direction);	//初步测试过关
	void GetLineTypeVector(std::vector<LineType> &lineTypeVector, const ChessStep step);	//初步测试过关
	void GetLineType(LineType &primary, LineType &secondary, const std::vector<int> &line);	//初步测试过关
	void GetLineType(LineType &lineType, const std::vector<int>::const_iterator &five);
	bool MoreDense(const std::vector<int>::const_iterator &first, const std::vector<int>::const_iterator &second);
	void GetLiveSpace(const std::vector<int>::const_iterator &five, int &liveSpace, int &holdSpace);
	int ValueOfNoType(const ChessStep step, const Direction direction);
	ChessPoint FindBestStep();

	void PrintLineType(const LineType &primary, const LineType &secondary);
	void PrintLineType(const LineType &l);
	void PrintLineTypeVector(const std::vector<LineType> &lineTypeVector);

public:
	HumanVsAI();

	void WinInformation();
	void DrawInformation();
	void Back();	//悔棋
	void SwitchDifficulty();

	virtual void mousePressEvent(QMouseEvent *e) override; // 鼠标点击棋盘时候触发
	virtual void enterEvent(QEvent *) override;
};

#endif // HUMANVSAI_H
