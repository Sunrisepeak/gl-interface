add_rules("mode.debug", "mode.release")

add_requires("glfw", "glad", "glm")

add_includedirs(".")

-- gli-opengl lib
target("gli-opengl")
    set_kind("shared")
    add_files("backends/gli_opengl.cpp")
    add_packages("glad", "glm")

-- gli-opengl-examples
includes("examples/xmake.lua")

-- gli demos
includes("demos/xmake.lua")