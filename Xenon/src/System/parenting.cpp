#include "parenting.hpp"
#include "core/appData.hpp"
#include "System/Components/Transform.hpp"
#include "System/Components/Parenting.hpp"

void updateRecursive(Core::Child& child, Core::Transform& parentTransform) {
	// TODO: make it actually work as intended
	child.transform->x = parentTransform.x + child.x;
	child.transform->y = parentTransform.y + child.y;
	child.transform->angle = parentTransform.angle + child.angle;
	child.transform->xScale = parentTransform.xScale * child.xScale;
	child.transform->yScale = parentTransform.yScale * child.yScale;

	if(!child.next.isNull())
		updateRecursive(child.next.get(), parentTransform);
	if(!child.selfParent.isNull())
		updateRecursive(child.selfParent->childList.get(), child.transform.get());
}

void Core::updateChildPositions() {
	auto& parents = AppData::getComponentManager().get<Parent>();

	for(auto& parent : parents) {
		if(parent.root)
			updateRecursive(parent.childList.get(), parent.transform.get());
	}
}
