#include <vector>
#include <map>
#include <set>
#include <utility>

// regex will search from 1st to the last one
// therefore, longer unit must be in front
// e.g. mm must come before m
// otherwise, mm will be misidentified as 2 separate m s
const std::vector<std::string> AvaliableUnits
	{	"mm",
		"fm",
		"nm",
		"um",
		"cm",
		"m",
		"rad",
		"deg",
		"eV",
		"MeV",
		"keV"};

class ParticleHistory
{
public:
	ParticleHistory(const std::string& t_particle_name, unsigned t_track_id, unsigned t_parent_id) : 
	particle_type_(t_particle_name), 
	track_id_(t_track_id), 
	parent_id_(t_parent_id)
	{
		for(auto&& name : unitless_information_)
			unit_less_[name] = std::vector<std::string>{};
		for(auto&& name : unit_information_)
			unit_[name] = std::vector<std::pair<double, std::string>>{};
	};

	// this is to write unitless information
	void WriteInformation(	const std::string& t_information_name, 
				const std::string& t_information_value)
	{
		unit_less_.at(t_information_name).push_back(t_information_value);
	};
		
	// and this is for information with unit
	void WriteInformation(	const std::string& t_information_name, 
				double t_information_value,
				const std::string& t_information_unit)
	{
		unit_.at(t_information_name).push_back(std::make_pair(t_information_value, t_information_unit));
	};

	const std::map<std::string, std::vector<std::string>>& GetUnitLessInformation() const
	{
		CheckForCompleteness();
		return unit_less_;
	};

	const std::map<std::string, std::vector<std::pair<double, std::string>>>& GetInformation() const
	{
		CheckForCompleteness();
		return unit_;
	};

	const void Print() const
	{
		CheckForCompleteness();
		// number of histories
		unsigned int num_entries = unit_.begin()->second.size();
		// print the keys for the particle
		for(auto const& key: unit_information_)
			std::cout << key << "\t";
		for(auto const& key: unitless_information_)
			std::cout << key << "\t";
		std::cout << "\n";

		// print the content
		for(unsigned int i = 0; i < num_entries; ++i)
		{
			for(auto const& key : unit_information_) 
				std::cout << unit_.at(key)[i].first << " " << unit_.at(key)[i].second << "\t";
			for(auto const& key : unitless_information_)
				std::cout << unit_less_.at(key)[i] << "\t";
			std::cout << "\n";
		}
	}

private:
	// function to check if the size of vectors across each field is equal
	void CheckForCompleteness() const
	{
		unsigned int num_entries = unit_less_.begin()->second.size();
		for(auto key = unit_less_.begin(); key != unit_less_.end(); key++)
			if(key->second.size() != num_entries) 
				throw std::runtime_error(
					"Size of each key in history is inconsistance. [" 
					+ unit_less_.begin()->first + "] has " + std::to_string(unit_less_.begin()->second.size())
					+ "elements while ["
					+ key->first + "] has " + std::to_string(key->second.size()) + "elements\n");

		for(auto key = unit_.begin(); key != unit_.end(); key++)
			if(key->second.size() != num_entries) 
				throw std::runtime_error(
					"Size of each key in history is inconsistance. [" 
					+ unit_less_.begin()->first + "] has " + std::to_string(unit_less_.begin()->second.size()) 
					+ "elements while ["
					+ key->first + "] has " + std::to_string(key->second.size()) + "elements\n");
	}
	std::string particle_type_;
	int track_id_;
	int parent_id_;
	std::map<std::string, std::vector<std::string>> unit_less_;
	std::map<std::string, std::vector<std::pair<double, std::string>>> unit_;

	const std::set<std::string> unitless_information_
		{"Step#", "Volume", "Process"};

	const std::set<std::string> unit_information_
		{"X", "Y", "Z", "Angle", "KineE", "dEStep", "StepLeng", "TrakLeng"};
};
