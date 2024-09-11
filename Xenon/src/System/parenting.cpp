#include "parenting.hpp"
#include "core/appData.hpp"
#include "System/Components/Transform.hpp"
#include "System/Components/Parenting.hpp"
#include "devTools/logger_core.hpp"

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

	// TEST:
	auto& transforms = AppData::getComponentManager().get<Transform>();

	XN_LOG_DEB("Check transform validity:");
	for(auto& transform : transforms) {
		XN_LOG_DEB("Entity {0}: Transform: x={0}, y={0}, angle={0}, xScale={0}, yScale={0}", 
			 transform.m_owner, transform.x, transform.y, transform.angle, transform.xScale, transform.yScale);
	}
}
