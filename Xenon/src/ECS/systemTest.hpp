#pragma once
namespace Core{
class ComponentManager;
namespace Systems {
	struct testMetadata {};
	void systemTest([[maybe_unused]] ComponentManager* cmgr,[[maybe_unused]] testMetadata& metadata, int& comp);
}
}

