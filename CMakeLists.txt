cmake_minimum_required(VERSION 3.13)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_compile_definitions(_ENABLE_EXTENDED_ALIGNED_STORAGE)

project(Vulkan_Graphics_Engine)

option(BUILD_SHARED_LIBS "Build the project as a shared library" OFF)
option(BUILD_IMGUI "Build the imgui version provided by the engine. Turn this off if you're using your own version" ON)
option(BUILD_GLFW "Build the GLFW version provided by the engine. Turn this off if you're using your own version" ON)
option(USE_NATIVE_VULKAN_IMPLEMENTATION "Uses the Vulkan implementation the user has, instead of the one bundled with the engine. This can be useful when wanting to use a newer version for bug fixes" OFF)

set (Engine_VERSION_MAJOR 1)
set (Engine_VERSION_MINOR 0)
set (Engine_VERSION_PATCH 0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(USE_NATIVE_VULKAN_IMPLEMENTATION)
find_package(Vulkan)

if(VULKAN_FOUND)
	message("Vulkan library found")
else()
	set(USE_NATIVE_VULKAN_IMPLEMENTATION OFF)
	message(ERROR "No Vulkan installation has been found by CMake. Using the bundled implementation instead")
endif(VULKAN_FOUND)
endif(USE_NATIVE_VULKAN_IMPLEMENTATION)

##### SOURCES #####
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" )
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB UTILITY_HEADERS CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/include/utility/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/include/utility/*.h")
file(GLOB UTILITY_SOURCES CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.c")
file(GLOB VULKAN_HEADERS CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/*.h")
file(GLOB VULKAN_SOURCES CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/*.c")
file(GLOB VULKAN_INTERNAL_HEADERS CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/internal/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/internal/*.h")
file(GLOB VULKAN_INTERNAL_SOURCES CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/internal/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/internal/*.c")
file(GLOB VULKAN_INTERNAL_UTILITY_HEADERS CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/internal/utility/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/internal/utility/*.h")
file(GLOB VULKAN_INTERNAL_UTILITY_SOURCES CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/internal/utility/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/internal/utility/*.c")
file(GLOB VULKAN_INTERNAL_RENDERERS_HEADERS CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/internal/renderers/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/internal/renderers/*.h")
file(GLOB VULKAN_INTERNAL_RENDERERS_SOURCES CONFIGURE_DEPENDS] 
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/internal/renderers/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan/internal/renderers/*.c")

foreach(FILE ${HEADERS}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/include" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "include${GROUP}")

    # Group into "Source Files" and "Header Files"
    set(GROUP "${GROUP}")
	
    source_group("${GROUP}" FILES "${FILE}")
endforeach()

foreach(FILE ${SOURCES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "source${GROUP}")

    # Group into "Source Files" and "Header Files"
    set(GROUP "${GROUP}")
	
    source_group("${GROUP}" FILES "${FILE}")
endforeach()

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

## file(COPY "${CMAKE_SOURCE_DIR}/deps/glm/glm" DESTINATION "${CMAKE_BINARY_DIR}/deps/include")

if (BUILD_GLFW)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glfw ${CMAKE_BINARY_DIR}/glfw)
set(GLFW_BUILD_DOCS ON)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
endif (BUILD_GLFW)

if(BUILD_SHARED_LIBS)
add_library(Vulkan_Rendering_Engine SHARED ${HEADERS} ${SOURCES})
else()
add_library(Vulkan_Rendering_Engine STATIC ${HEADERS} ${SOURCES})
endif()

target_include_directories(Vulkan_Rendering_Engine PUBLIC ${CMAKE_SOURCE_DIR}/deps/glm)
target_include_directories(Vulkan_Rendering_Engine PUBLIC ${CMAKE_SOURCE_DIR}/deps/stb)
target_include_directories(Vulkan_Rendering_Engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

if (BUILD_IMGUI)
target_include_directories(Vulkan_Rendering_Engine PUBLIC ${CMAKE_SOURCE_DIR}/deps/imgui)

file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/deps/imgui/*.cpp")
file(GLOB IMGUI_HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/deps/imgui/*.h")

add_library(ImGui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})
target_link_libraries(Vulkan_Rendering_Engine ImGui)
endif (BUILD_IMGUI)

if (BUILD_GLFW)
target_link_libraries(Vulkan_Rendering_Engine glfw)
endif()

if(USE_NATIVE_VULKAN_IMPLEMENTATION)
target_link_libraries(Vulkan_Rendering_Engine ${VULKAN_STATIC_LIBRARY})
else()
target_include_directories(Vulkan_Rendering_Engine PUBLIC ${CMAKE_SOURCE_DIR}/deps/Vulkan/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
target_link_libraries(Vulkan_Rendering_Engine ${CMAKE_SOURCE_DIR}/deps/Vulkan/libs/x64/vulkan-1.lib)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
target_link_libraries(Vulkan_Rendering_Engine ${CMAKE_SOURCE_DIR}/deps/Vulkan/libs/x86/vulkan-1.lib)
endif()
endif()