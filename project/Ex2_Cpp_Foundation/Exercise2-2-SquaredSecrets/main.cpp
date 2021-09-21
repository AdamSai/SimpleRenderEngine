#include <iostream>
#include <vector>

int main() {
	std::string input;
	std::cout << "Enter message to be encrypted:\n";
	std::cin >> input;
	int depth = sqrt(input.length()) + 1;
	int index = 0;


	std::vector<std::vector<char>> grid(depth, std::vector<char>(depth));

	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			if ((index) == input.length())
				break;
			grid[j][i] = input.at(index++);
		}
	}

	// Print the encrypted message
	std::string messageGrid;
	std::string message;

	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			messageGrid += grid[j][i];
			message += grid[i][j];
			if (j == depth - 1)
			{
				messageGrid += "\n";
				message += " ";
			}
		}
	}

	std::cout << std::endl << "Grid:" << std::endl << messageGrid << std::endl;
	std::cout << "message:" << std::endl << message << std::endl;



	//// todo output secret message

	//// Examples:
	//// input -> output
	//// ifmanwasmeanttostayonthegroundgodwouldhavegivenusroots -> imtgdvs  fearwer  mayoogo  anouuio  ntnnlvt  wttddes  aohghn   sseoau
	//// haveaniceday -> hae and via ecy
	//// feedthedog -> fto ehg ee dd
	//// chillout -> cluhltio 

	//int x; // waits for user input. Workaround for not closing window on Windows
	//std::cin >> x;

	return 0;
}