#include "sc2api/sc2_api.h"

#include "CCBot.h"
#include "Util.h"

CCBot::CCBot()
    : m_map(*this)
    , m_bases(*this)
    , m_unitInfo(*this)
    , m_workers(*this)
    , m_gameCommander(*this)
    , m_strategy(*this)
    , m_techTree(*this)
{
    
}

void CCBot::OnGameStart() 
{
    m_config.readConfigFile();
    
    // get my race
    auto playerID = Observation()->GetPlayerID();
    for (auto & playerInfo : Observation()->GetGameInfo().player_info)
    {
        if (playerInfo.player_id == playerID)
        {
            m_playerRace[Players::Self] = playerInfo.race_actual;
        }
        else
        {
            m_playerRace[Players::Enemy] = playerInfo.race_requested;
        }
    }
    
    m_techTree.onStart();
    m_strategy.onStart();
    m_map.onStart();
    m_unitInfo.onStart();
    m_bases.onStart();
    m_workers.onStart();

    m_gameCommander.onStart();
}

void CCBot::OnStep()
{
    Control()->GetObservation();

    m_map.onFrame();
    m_unitInfo.onFrame();
    m_bases.onFrame();
    m_workers.onFrame();
    m_strategy.onFrame();

    m_gameCommander.onFrame();

    Debug()->SendDebug();
}

// TODO: Figure out my race
const sc2::Race & CCBot::GetPlayerRace(int player) const
{
    BOT_ASSERT(player == Players::Self || player == Players::Enemy, "invalid player for GetPlayerRace");
    return m_playerRace[player];
}

BotConfig & CCBot::Config()
{
     return m_config;
}

const MapTools & CCBot::Map() const
{
    return m_map;
}

const StrategyManager & CCBot::Strategy() const
{
    return m_strategy;
}

const BaseLocationManager & CCBot::Bases() const
{
    return m_bases;
}

const UnitInfoManager & CCBot::UnitInfo() const
{
    return m_unitInfo;
}

const TypeData & CCBot::Data(const sc2::UnitTypeID & type) const
{
    return m_techTree.getData(type);
}

const TypeData & CCBot::Data(const sc2::UpgradeID & type) const
{
    return m_techTree.getData(type);
}

const TypeData & CCBot::Data(const BuildType & type) const
{
    return m_techTree.getData(type);
}

WorkerManager & CCBot::Workers()
{
    return m_workers;
}

const sc2::Unit * CCBot::GetUnit(const UnitTag & tag) const
{
    return Observation()->GetUnit(tag);
}

sc2::Point2D CCBot::GetStartLocation() const
{
    return Observation()->GetStartLocation();
}

void CCBot::SetConfigFileLocation(std::string configFileLocation)
{
	m_config.ConfigFileLocation = configFileLocation;
}

bool CCBot::HasBuildOrder() {
	return false;
}

void CCBot::QueueProductionItems(ProductionManager *pm) {
	return;
}

int CCBot::CountUnitType(const sc2::ObservationInterface* observation, sc2::UnitTypeID unit_type) {
	int count = 0;
	sc2::Units my_units = observation->GetUnits(sc2::Unit::Alliance::Self);
	for (const sc2::Unit& unit : my_units) {
		if (unit.unit_type == unit_type)
			++count;
	}

	return count;
}

bool CCBot::IsBuildingStructure(const sc2::ObservationInterface* observation, sc2::AbilityID ability_type_for_structure) {
	sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
	for (const auto& unit : units) {
		for (const auto& order : unit.orders) {
			if (order.ability_id == ability_type_for_structure) {
				return true;
			}
		}
	}
	return false;
}

void CCBot::OnError(const std::vector<sc2::ClientError> & client_errors, const std::vector<std::string> & protocol_errors)
{
    
}