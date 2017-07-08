
#include"LocalAlignment.h"
#include<algorithm>

int countMatch = 0, countMismatch = 0, countOpeningGap = 0, countGap = 0;
Cell T[120][240];
char align[3][350];

void LocalAlignment::localMain(std::string s1, std::string s2, std::string* parameters)
{
	createMatrixLocal(s1, s2, parameters);
	Cell alignment;
	LocalAlignment local;

	optimalScore(s1, s2, parameters, local);
	alignmentLocal(s1, s2, parameters, local);
}

void LocalAlignment::alignmentLocal(std::string s1, std::string s2, std::string* parameters, LocalAlignment &local)
{
	float m_a = stof(parameters[0]);
	float m_i = stof(parameters[1]);
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);

	Cell alignment;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 330; j++) {
			align[i][j] = NULL;
		}
	}

	int i = local.maxI, m = local.maxI;
	int j = local.maxJ, n = local.maxJ;
	int count = m + n;

	char direction;

	float current = max(max(T[i][j].S, T[i][j].I), T[i][j].D);
	if (current == T[i][j].S) direction = 'S';
	if (current == T[i][j].D) direction = 'D';
	if (current == T[i][j].I) direction = 'I';

	while (i > 0 && j > 0 && current != 0)
	{
		float sTemp;
		float dTemp;
		float iTemp;

		if (T[i][j].D == current && direction == 'D')
		{
			align[0][count] = s1[i - 1];
			align[1][count] = ' ';
			align[2][count] = '_';

			sTemp = T[i - 1][j].S + h + g;
			iTemp = T[i - 1][j].I + h + g;
			dTemp = T[i - 1][j].D + g;

			if (T[i][j].D == dTemp)
			{
				current = T[i - 1][j].D;
				direction = 'D';
			}
			else if (T[i][j].D == iTemp)
			{
				current = T[i - 1][j].I;
				direction = 'I';
			}
			else if (T[i][j].D == sTemp)
			{
				current = T[i - 1][j].S;
				direction = 'S';
			}
			i--;
			count--;
		}
		else if (T[i][j].S == current && direction == 'S')
		{
			align[0][count] = s1[i - 1];
			align[2][count] = s2[j - 1];
			if (s1[i - 1] == s2[j - 1])
				align[1][count] = '|';
			else
				align[1][count] = ' ';

			sTemp = T[i - 1][j - 1].S + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);
			iTemp = T[i - 1][j - 1].I + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);
			dTemp = T[i - 1][j - 1].D + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);

			if (T[i][j].S == sTemp)
			{
				current = T[i - 1][j - 1].S;
				direction = 'S';
			}
			else if (T[i][j].S == iTemp)
			{
				current = T[i - 1][j - 1].I;
				direction = 'I';
			}
			else if (T[i][j].S == dTemp)
			{
				current = T[i - 1][j - 1].D;
				direction = 'D';
			}
			i--;
			j--;
			count--;
		}

		else if (T[i][j].I == current && direction == 'I')
		{
			align[0][count] = '_';
			align[1][count] = ' ';
			align[2][count] = s2[j - 1];

			sTemp = T[i][j - 1].S + h + g;
			iTemp = T[i][j - 1].I + g;
			dTemp = T[i][j - 1].D + h + g;

			if (T[i][j].I == sTemp)
			{
				current = T[i][j - 1].S;
				direction = 'S';
			}
			else if (T[i][j].I == iTemp)
			{
				current = T[i][j - 1].I;
				direction = 'I';
			}
			else if (T[i][j].I == dTemp)
			{
				current = T[i][j - 1].D;
				direction = 'D';
			}
			j--;
			count--;
		}
	}

	//calculating counts of match,mismatch, gaps and opening gaps
	for (int loop = count; loop <= m + n; loop++)
	{
		if (align[0][loop] == align[2][loop] && align[0][loop] != '\0' && align[2][loop] != '\0')
			countMatch++;
		if (align[0][loop] != align[2][loop] && align[0][loop] != '_'
			&& align[2][loop] != '_' && align[0][loop] != '\0' && align[2][loop] != '\0')
			countMismatch++;
		if (align[0][loop] == '_')
		{
			countGap++;
			if (align[0][loop + 1] != '_')
				countOpeningGap++;
		}
		if (align[2][loop] == '_')
		{
			countGap++;
			if (align[2][loop + 1] != '_')
				countOpeningGap++;
		}
	}
}

void LocalAlignment::createMatrixLocal(std::string s1, std::string s2, std::string* parameters)
{
	//initializations

	T[0][0].S = 0;
	T[0][0].D = -INFINITY;
	T[0][0].I = -INFINITY;

	for (int i = 1; i < 110 + 1; i++)
	{
		T[i][0].S = -INFINITY;
		T[i][0].I = -INFINITY;
		T[i][0].D = 0;
	}

	for (int j = 1; j < 220 + 1; j++)
	{
		T[0][j].S = -INFINITY;
		T[0][j].I = 0;
		T[0][j].D = -INFINITY;
	}

	for (int i = 1; i < 110 + 1; i++) {
		for (int j = 1; j < 220 + 1; j++) {
			T[i][j].S = -INFINITY;
			T[i][j].I = -INFINITY;
			T[i][j].D = -INFINITY;
		}
	}
}

void LocalAlignment::optimalScore(std::string s1, std::string s2, std::string* parameters, LocalAlignment &local)
{
	Cell alignment;
	float m_a = stof(parameters[0]);
	float m_i = stof(parameters[1]);
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);
	const int m = s1.length();
	const int n = s2.length();
	float temp, zero = 0.0;

	//calculating matrix and finding max scoring cell
	for (int i = 1; i < m + 1; i++)
	{
		for (int j = 1; j < n + 1; j++)
		{
			//Substitution
			temp = max(max(T[i - 1][j - 1].S, T[i - 1][j - 1].I), T[i - 1][j - 1].D) + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);
			T[i][j].S = max(temp, zero);
			//Delete
			temp = max((max(T[i - 1][j].S, T[i - 1][j].I) + h + g), (T[i - 1][j].D + g));
			T[i][j].D = max(temp, zero);
			//Insert
			temp = max((max(T[i][j - 1].S, T[i][j - 1].D) + h + g), (T[i][j - 1].I + g));
			T[i][j].I = max(temp, zero);

			float score = max(max(T[i][j].S, T[i][j].D), T[i][j].I);
			if (score>local.maxScore)
			{
				local.maxScore = score;
				local.maxI = i;
				local.maxJ = j;
			}
		}
	}
}