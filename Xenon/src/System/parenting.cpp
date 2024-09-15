#include "parenting.hpp"
#include "core/appData.hpp"
#include "System/Components/Transform.hpp"
#include "System/Components/Parenting.hpp"

void updateRecursive(const Core::Parent& parent) {
	using Core::Child, Core::Parent, Core::Transform;

	const Transform& parentTransform = parent.transform.get();
	Child* child = parent.childList.getPtr();

	while(child != nullptr) {
		// TODO: make it actually work as intended
		child->transform->x = parentTransform.x + child->x;
		child->transform->y = parentTransform.y + child->y;
		child->transform->angle = parentTransform.angle + child->angle;
		child->transform->xScale = parentTransform.xScale * child->xScale;
		child->transform->yScale = parentTransform.yScale * child->yScale;

		Parent* childP = child->selfParent.getPtr();
		if(childP != nullptr)
			updateRecursive(*childP);

		child = child->next.getPtr();
	}
}

void Core::updateChildPositions() {
	auto& parents = AppData::getComponentManager().get<Parent>();

	for(auto& parent : parents) {
		if(parent.root)
			updateRecursive(parent);
	}
}
