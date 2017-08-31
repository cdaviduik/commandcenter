#include "sc2api/sc2_api.h"
#include "../../CCBot.h"

class CDTerranMarineRushBot : public CCBot
{
public:
	CDTerranMarineRushBot();
	bool HasBuildOrder() override;
	void QueueProductionItems(ProductionManager *pm) override;
};
