add_rules("mode.debug", "mode.release")

target("farsh")
    set_targetdir(".")
    set_kind("binary")
    add_files("src/*.c", "src/**/*.c")
