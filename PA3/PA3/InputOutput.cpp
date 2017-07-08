#include"InputOutput.h"
int numAlphabets;
unsigned long m;

std::string InputOutput::inputGenome(char *inputPath)
{
	std::string line, str = "";
	std::ifstream input;
	int count = 0;
	std::string gene;
	
	input.open(inputPath);

	if (input.is_open())
	{
		getline(input, line);
		while (getline(input, line))
		{
			if (line[0] != '>') {
				gene = gene + line;
			}
		}
		gene = gene + '$';
		input.close();
		transform(gene.begin(), gene.end(), gene.begin(), toupper);
		remove_if(gene.begin(), gene.end(), isspace);
		return gene;
	}

	else {
		std::cout << "\nUnable to open input file\n";
		exit(EXIT_FAILURE);
	}
}

int InputOutput::alphabetParse(char *alphabetFile)
{
	int numalpha = 0;
	ifstream input;
	input.open(alphabetFile);
	string line;
	int loop = 0;
	if (input.is_open())
	{
		getline(input, line);
		while (line[loop] != '\0')
		{
			if ((line[loop] <= 'z' && line[loop] >= 'a') || (line[loop] <= 'Z' && line[loop] >= 'A'))
				numalpha++;
			loop++;
		}
	}
	input.close();
	return numalpha;
}