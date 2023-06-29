# CMakeList.txt : CMake project for RayScenes, include source and define
# project specific logic here.
#

find_package(OpenMP REQUIRED)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd6993")
endif()


# Add source to this project's executable.
add_executable (RayScenes "RayScenes.cpp" "RayScenes.h"  "stb_image.h" "stb_image_write.h"  "Image.h" "Maths.h" "Maths.cpp"  "Entity.h" "Entity.cpp" "Material.h" "Material.cpp" "tiny_obj_loader.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

target_link_libraries(RayScenes PRIVATE OpenMP::OpenMP_CXX)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RayScenes PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
