#include <iostream>
#include <iterator>
#include <string>
#include <fstream>
#include <regex>
#include <stdexcept>
#include "ParticleHistory.h"

int main()
{
	ParticleHistory part_hist("deutron", 1, 0);
	std::ifstream log_file("MyLogFile3");
	if(!log_file.is_open()) throw std::runtime_error("Log file not opened\n");

	// parse to realise a new particle is being read
	std::regex beginning_new_particle("^\\*+$");

	// parse to realise the next information in line is a number
	std::regex next_number("^\\s*([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)");

	// parse to realise the next infromation is a unit
	std::string unit_regex_command = "^\\s*(";
	for(auto const& unit : AvaliableUnits)
		unit_regex_command += unit +"|";
	unit_regex_command.erase(unit_regex_command.size() - 1);
	unit_regex_command += ")";
	std::cout << unit_regex_command << "\n";
	std::regex unit_regex(unit_regex_command);
		

	// parse to capture the next infomration which is not a float
	std::regex next_str("^\\s*([^\\s]+)");
	std::smatch match1, match2;
	std::string line;
	std::vector<std::string> keys{	
		"Step#",      
		"X",         
		"Y",
		"Z",
		"Angle",
		"KineE",
		"dEStep",
		"StepLeng",
		"TrakLeng",
		"Volume",
		"Process"};

	while(std::getline(log_file, line))
	{
		int row = 0;
		double value;
		std::string unit, text;
		while(true)
		{
			bool have_value = false, have_unit = false;
			std::cout << line << "\n";
			if(std::regex_search(line, match1, next_number))
			{
				text = match1.str(1);
				value = atof(text.c_str());
				line = match1.suffix().str();

				// see if the number is followed by a unit
				if(std::regex_search(line, match2, unit_regex))
				{
					part_hist.WriteInformation(keys[row], value, match2.str(1));
					line = match2.suffix().str();
					++row;
				}
				else 
				{
					part_hist.WriteInformation(keys[row], text);
					++row;
				}
			} else if(std::regex_search(line, match2, next_str))
			{
				part_hist.WriteInformation(keys[row], match2.str(1));
				++row;
				std::cout << line << "\n";
				line = match2.suffix().str();
			}else break;
		}
	}
	part_hist.Print();
	return 0;
}
