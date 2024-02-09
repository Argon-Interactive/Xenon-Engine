add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb)

include_directories(vendor/glad/include/glad)
include_directories(vendor/glad/include/KHR)
include_directories(vendor/glad/include)
include_directories(vendor/glfw/include/GLFW)
include_directories(vendor/glm)
include_directories(vendor/stb)
include_directories(vendor/all)

add_subdirectory(src)