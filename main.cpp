#include <iostream>
#include <ranges>
#include <unordered_map>
#include <libxml++/libxml++.h>
#include <limits>
#include <filesystem>

enum GAS {
	OXYGEN,
	NITROGEN,
	CARBON_DIOXIDE,
	VOLATILES,
	POLLUTANT,
	WATER,
	POLLUTED_WATER,
	NITROUS_OXIDE,
	LIQUID_NITROGEN,
	LIQUID_OXYGEN,
	LIQUID_VOLATILES,
	STEAM,
	LIQUID_CARBON_DIOXIDE,
	LIQUID_POLLUTANT,
	LIQUID_NITROUS_OXIDE
};

struct PlanetAtmos {
	std::unordered_map<GAS, double> atmosphere;

	explicit PlanetAtmos() {
		atmosphere.emplace(OXYGEN, 0.0f);
		atmosphere.emplace(NITROGEN, 0.0f);
		atmosphere.emplace(CARBON_DIOXIDE, 0.0f);
		atmosphere.emplace(VOLATILES, 0.0f);
		atmosphere.emplace(POLLUTANT, 0.0f);
		atmosphere.emplace(WATER, 0.0f);
		atmosphere.emplace(POLLUTED_WATER, 0.0f);
		atmosphere.emplace(NITROUS_OXIDE, 0.0f);
		atmosphere.emplace(LIQUID_NITROGEN, 0.0f);
		atmosphere.emplace(LIQUID_OXYGEN, 0.0f);
		atmosphere.emplace(LIQUID_VOLATILES, 0.0f);
		atmosphere.emplace(STEAM, 0.0f);
		atmosphere.emplace(LIQUID_CARBON_DIOXIDE, 0.0f);
		atmosphere.emplace(LIQUID_POLLUTANT, 0.0f);
		atmosphere.emplace(LIQUID_NITROUS_OXIDE, 0.0f);
	}

	void addGas(GAS gas, double value) { atmosphere[gas] += value; }
};

struct AtmosData {
	std::unordered_map<GAS, double> atmosphere;
	double energy{};
	double volume{};
	double networkRefId{};
	double thingRefId{};
	double mothershipRefId{};
	int posX{};
	int posY{};
	int posZ{};

	explicit AtmosData() {
		atmosphere.emplace(OXYGEN, 0.0f);
		atmosphere.emplace(NITROGEN, 0.0f);
		atmosphere.emplace(CARBON_DIOXIDE, 0.0f);
		atmosphere.emplace(VOLATILES, 0.0f);
		atmosphere.emplace(POLLUTANT, 0.0f);
		atmosphere.emplace(WATER, 0.0f);
		atmosphere.emplace(POLLUTED_WATER, 0.0f);
		atmosphere.emplace(NITROUS_OXIDE, 0.0f);
		atmosphere.emplace(LIQUID_NITROGEN, 0.0f);
		atmosphere.emplace(LIQUID_OXYGEN, 0.0f);
		atmosphere.emplace(LIQUID_VOLATILES, 0.0f);
		atmosphere.emplace(STEAM, 0.0f);
		atmosphere.emplace(LIQUID_CARBON_DIOXIDE, 0.0f);
		atmosphere.emplace(LIQUID_POLLUTANT, 0.0f);
		atmosphere.emplace(LIQUID_NITROUS_OXIDE, 0.0f);
	}

	void addGas(GAS gas, double value) { atmosphere[gas] += value; }

	bool hasOnlyGases(const std::vector<GAS>& gases) const {
		for (auto [gas, value] : atmosphere) {
			bool gasFound = false;
			for (auto checkGas : gases) {
				//
				if (gas == checkGas && value > 0)
					//
					gasFound = true;
			}
			if (!gasFound && value > 0) return false;
		}
		return true;
	}
};

double getValue(xmlpp::Node* node) {
	return stod(dynamic_cast<xmlpp::Element*>(node)->get_attribute("Value")->get_value());
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: \t" << argv[0] << " <xml file to cleanup> <cleaned xml file> " << std::endl;
		std::cerr << "\tor\n";
		std::cerr << "\t" << argv[0] << " <xml file to cleanup> " << std::endl;
		return 1;
	}
	std::filesystem::path xmlFile = argv[1];
	std::cout << "Parsing XML file: " << xmlFile << std::endl;
	xmlpp::DomParser parser(xmlFile);
	auto document = parser.get_document();
	auto worldData = document->get_root_node();
	PlanetAtmos planetAtmos;

	// Read the planet atmosphere
	auto planet = worldData->get_first_child("PlanetaryAtmosphere")->get_first_child("GlobalGasMix");

	planetAtmos.addGas(OXYGEN, getValue(planet->get_first_child("Oxygen")));
	planetAtmos.addGas(NITROGEN, getValue(planet->get_first_child("Nitrogen")));
	planetAtmos.addGas(CARBON_DIOXIDE, getValue(planet->get_first_child("CarbonDioxide")));
	planetAtmos.addGas(VOLATILES, getValue(planet->get_first_child("Volatiles")));
	planetAtmos.addGas(POLLUTANT, getValue(planet->get_first_child("Pollutant")));
	planetAtmos.addGas(WATER, getValue(planet->get_first_child("Water")));
	planetAtmos.addGas(POLLUTED_WATER, getValue(planet->get_first_child("PollutedWater")));
	planetAtmos.addGas(NITROUS_OXIDE, getValue(planet->get_first_child("NitrousOxide")));
	planetAtmos.addGas(LIQUID_NITROGEN, getValue(planet->get_first_child("LiquidNitrogen")));
	planetAtmos.addGas(LIQUID_OXYGEN, getValue(planet->get_first_child("LiquidOxygen")));
	planetAtmos.addGas(LIQUID_VOLATILES, getValue(planet->get_first_child("LiquidVolatiles")));
	planetAtmos.addGas(STEAM, getValue(planet->get_first_child("Steam")));
	planetAtmos.addGas(LIQUID_CARBON_DIOXIDE, getValue(planet->get_first_child("LiquidCarbonDioxide")));
	planetAtmos.addGas(LIQUID_POLLUTANT, getValue(planet->get_first_child("LiquidPollutant")));
	planetAtmos.addGas(LIQUID_NITROUS_OXIDE, getValue(planet->get_first_child("LiquidNitrousOxide")));

	// Save gases that actually contain in the atmosphere
	std::vector<GAS> planetGases;
	for (auto pair : planetAtmos.atmosphere) { if (pair.second > 0) planetGases.emplace_back(pair.first); }

	//Read the atmospheres
	auto atmosList = worldData->find("Atmospheres/AtmosphereSaveData");
	std::list<AtmosData> atmosDatas;
	int removedAtmosCount = 0;
	for (auto atmosphere : atmosList) {
		AtmosData data;
		auto pos = atmosphere->get_first_child("Position");
		data.posX = static_cast<int>(pos->eval_to_number("x"));
		data.posY = static_cast<int>(pos->eval_to_number("y"));
		data.posZ = static_cast<int>(pos->eval_to_number("z"));
		data.addGas(OXYGEN, atmosphere->eval_to_number("Oxygen"));
		data.addGas(NITROGEN, atmosphere->eval_to_number("Nitrogen"));
		data.addGas(CARBON_DIOXIDE, atmosphere->eval_to_number("CarbonDioxide"));
		data.addGas(VOLATILES, atmosphere->eval_to_number("Volatiles"));
		data.addGas(POLLUTANT, atmosphere->eval_to_number("Chlorine"));
		data.addGas(WATER, atmosphere->eval_to_number("Water"));
		data.addGas(POLLUTED_WATER, atmosphere->eval_to_number("PollutedWater"));
		data.addGas(NITROUS_OXIDE, atmosphere->eval_to_number("NitrousOxide"));
		data.addGas(LIQUID_NITROGEN, atmosphere->eval_to_number("LiquidNitrogen"));
		data.addGas(LIQUID_OXYGEN, atmosphere->eval_to_number("LiquidOxygen"));
		data.addGas(LIQUID_VOLATILES, atmosphere->eval_to_number("LiquidVolatiles"));
		data.addGas(STEAM, atmosphere->eval_to_number("Steam"));
		data.addGas(LIQUID_CARBON_DIOXIDE, atmosphere->eval_to_number("LiquidCarbonDioxide"));
		data.addGas(LIQUID_POLLUTANT, atmosphere->eval_to_number("LiquidPollutant"));
		data.addGas(LIQUID_NITROUS_OXIDE, atmosphere->eval_to_number("LiquidNitrousOxide"));
		data.energy = atmosphere->eval_to_number("Energy");
		data.volume = atmosphere->eval_to_number("Volume");
		data.networkRefId = atmosphere->eval_to_number("NetworkReferenceId");
		data.thingRefId = atmosphere->eval_to_number("ThingReferenceId");
		data.mothershipRefId = atmosphere->eval_to_number("MothershipReferenceId");

		//Check if the current atmosphere consists from the planet atmosphere, not linked with anything, has volume same
		//as the world grid cell (2x2x2=8m^3) and its Y-coordinate above the adequate height.
		if (data.hasOnlyGases(planetGases) && data.networkRefId == 0 && data.thingRefId == 0 && data.volume == 8000 &&
			data.posY > 100) {
			//Remove the atmosppere
			xmlpp::Element::remove_node(atmosphere);
			removedAtmosCount++;
			continue;
		}
		atmosDatas.emplace_back(data);
	}
	if (removedAtmosCount <= 0) {
		std::cout << "No atmospheres to remove found" << std::endl;
		return 0;
	}

	//Save the changed file
	std::filesystem::path resultXmlFile;
	if (argc > 3) resultXmlFile = argv[2];
	else {
		resultXmlFile = xmlFile;
		auto backupFile = xmlFile.replace_extension(xmlFile.extension().string() + ".bak");
		//Backup
		rename(resultXmlFile, backupFile);
	}
	std::cout << "Removed " << removedAtmosCount << " atmospheres" << std::endl;
	document->write_to_file(resultXmlFile);

	return 0;
}
