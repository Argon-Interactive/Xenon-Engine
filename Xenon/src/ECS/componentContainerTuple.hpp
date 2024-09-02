#ifndef _XENON_ECS_COMPONENTCONTAINERTUPLE_
#define _XENON_ECS_COMPONENTCONTAINERTUPLE_

#include "componentImplementations/TestComp.hpp"
#include "System/Transform.hpp"

#include "containerTuple.hpp"

namespace Core {

template<template<typename Contained> typename Container>
using ComponentContainerTuple = ContainerTuple<Container,
/*------------------------------------------------------------------------------------------------*/
/*                                     List of all components                                     */
/*------------------------------------------------------------------------------------------------*/
	Comp, FComp, Transform
>;

}

#endif // !_XENON_ECS_COMPONENTCONTAINERTUPLE_
