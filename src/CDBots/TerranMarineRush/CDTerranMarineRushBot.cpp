#include "CDTerranMarineRushBot.h"

CDTerranMarineRushBot::CDTerranMarineRushBot() {}

bool CDTerranMarineRushBot::HasBuildOrder() {
	return true;
}

void CDTerranMarineRushBot::QueueProductionItems(ProductionManager *pm) {
	const sc2::ObservationInterface* observation = Observation();
	CCBot &m_bot = *this;

	pm->clearQueue();

	const int scv_threshold = 18;
	const int early_game_barracks = 4;
	const int midgame_supply = 25;

	int supply_threshold = 2;
	if (observation->GetFoodCap() >= midgame_supply) {
		supply_threshold = 8;
	}

	const int num_barracks = CountUnitType(observation, sc2::UNIT_TYPEID::TERRAN_BARRACKS);
	const int current_supply = observation->GetFoodUsed();
	const int supply_cap = observation->GetFoodCap();
	const int remaining_supply = supply_cap - current_supply;

	// prioritize supply depots
	bool should_build_depot = remaining_supply < supply_threshold && current_supply != 15;
	if (!IsBuildingStructure(observation, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT) && should_build_depot) {
		pm->queueAsHighestPriority(BuildType("SupplyDepot", m_bot), true);
		return;
	}

	// build scvs until saturated
	int scv_count = CountUnitType(observation, sc2::UNIT_TYPEID::TERRAN_SCV);
	if (pm->meetsReservedResources(BuildType("SCV", m_bot)) && !IsBuildingStructure(observation, sc2::ABILITY_ID::TRAIN_SCV) && scv_count < scv_threshold) {
		pm->queueAsHighestPriority(BuildType("SCV", m_bot), false);
	}

	// build barracks
	bool should_build_barracks = num_barracks < early_game_barracks || current_supply > midgame_supply;
	if (pm->meetsReservedResources(BuildType("Barracks", m_bot)) && should_build_barracks) {
		pm->queueAsLowestPriority(BuildType("Barracks", m_bot), false);
	}

	// build marines
	if (num_barracks > 0 && pm->meetsReservedResources(BuildType("Marine", m_bot))) {
		pm->queueAsLowestPriority(BuildType("Marine", m_bot), false);
	}

	// if floating minerals and getting low on supply build another depot
	if (current_supply > midgame_supply && remaining_supply < (supply_threshold * 2) && pm->meetsReservedResources(BuildType("SupplyDepot", m_bot))) {
		pm->queueAsLowestPriority(BuildType("SupplyDepot", m_bot), false);
	}
}
