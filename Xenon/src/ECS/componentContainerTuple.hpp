#ifndef _XENON_ECS_COMPONENTCONTAINERTUPLE_
#define _XENON_ECS_COMPONENTCONTAINERTUPLE_

//TEST:
#include "System/Components/TestComp.hpp"

#include "System/Components/Transform.hpp"

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
