#pragma once
#include "cell.h"

extern int countMatch, countMismatch, countOpeningGap, countGap;
extern Cell T[120][240];
extern char align[3][350];

class LocalAlignment
{
	int maxI;
	int maxJ;
	int maxScore;

public:
	void localMain(std::string s1, std::string s2, std::string* parameters);
	void createMatrixLocal(std::string s1, std::string s2, std::string* parameters);
	void optimalScore(std::string s1, std::string s2, std::string* parameters, LocalAlignment &local);
	void alignmentLocal(std::string s1, std::string s2, std::string* parameters, LocalAlignment &local);

	LocalAlignment()
	{
		maxI = 0;
		maxJ = 0;
		maxScore = 0;
	}
};
