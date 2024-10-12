add_rules("mode.debug", "mode.release")

add_requires("glfw", "glad", "glm")

add_includedirs(".")

-- common config

rule("opengles_config")
    on_load(function (target)
        if is_plat("android") then
            target:add("links", "GLESv2", "EGL")
        elseif is_plat("ios") then
            target:add("frameworks", "OpenGLES", "QuartzCore")
        elseif is_plat("linux") then
            target:add("links", "GLESv2", "EGL")
        elseif is_plat("windows") then
            target:add("links", "libGLESv2", "libEGL")
        elseif is_plat("macosx") then
            -- target:add("frameworks", "OpenGL")
        else
            target:add("links", "GLESv2", "EGL")
        end

        target:add("packages", "egl-header")

        -- if not is_plat("android") and not is_plat("ios") then
        --     target:add("packages", "glfw")
        -- end

        -- target:add("packages", "glm")
    end)



-- gli-opengl lib
target("gli-opengl")
    set_kind("shared")
    add_files("backends/gli_opengl.cpp")
    add_packages("glad", "glm")

    add_packages("opengl")

target("gli-opengl.static")
    set_kind("static")
    add_files("backends/gli_opengl.cpp")
    add_packages("glad", "glm")

    add_packages("opengl")

-- gli-opengles lib
-- sudo apt install libgles2-mesa-dev libegl1-mesa-dev
target("gli-opengles")
    set_kind("shared")
    add_files("backends/gli_opengles.cpp")
    add_packages("glm")

    add_rules("opengles_config")

target("gli-opengles.static")
    set_kind("static")
    add_files("backends/gli_opengles.cpp")
    add_packages("glm")

    add_rules("opengles_config")

-- gli-opengl-examples
includes("examples/xmake.lua")

-- gli demos
includes("demos/xmake.lua")

-- TODO: rust-ffi bindgen --with-derive-default --output tmp.rs gl_interface.h
