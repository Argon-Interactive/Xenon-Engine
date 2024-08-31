#include "componentManager.hpp"

namespace Core {

ComponentManager::ComponentManager() : m_refs(std::pmr::get_default_resource()) {}

}
