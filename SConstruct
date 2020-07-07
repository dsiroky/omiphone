import os
import sys

#==========================================================================

def setup_linker(env, builder_name):
    def builder(env, target, sources, **args):
        orig_builder = env["BUILDERS"][builder_name + "original"]
        target = orig_builder(env, target, sources, **args)
        # for incremental linking
        env.Precious(target)
        return target

    env["BUILDERS"][builder_name + "original"] = env["BUILDERS"][builder_name]
    env["BUILDERS"][builder_name] = builder

#--------------------------------------------------------------------------

def pretify_output(env, verbose, commands):
    # pretty print
    if not verbose:
      for k in commands:
          env[k + "COMSTR"] = "%s $TARGET" % k

    # coloring
    if sys.stdout.isatty():
        for k in commands:
            envk = k + "COMSTR"
            if envk in env:
                env[envk] = "\033[36m%s\033[0m" % env[envk]

#==========================================================================

PROJECT_ROOT = Dir("#").abspath

#==========================================================================
# build options

debug = bool(int(ARGUMENTS.get("debug", 1)))
verbose = bool(int(ARGUMENTS.get("verbose", 0)))

Help("""
Variants
--------
scons verbose=(0|1)              print short or long processing commands (default: %(verbose)i)
scons debug=(0|1)                compile debug or release (default: %(debug)i)
scons run_e2e_tests              execute end-to-end tests
""" % {"verbose":verbose, "debug":debug })

#==========================================================================

EnsurePythonVersion(3, 0)
EnsureSConsVersion(3, 1)
Decider("MD5-timestamp")
CacheDir(os.path.join(PROJECT_ROOT, ".build_cache"))

#==========================================================================

env = Environment(ENV=os.environ)

env["PROJECT_ROOT"] = PROJECT_ROOT

#==========================================================================

env["CC"] = os.environ.get("CC") or env["CC"]
env["CXX"] = os.environ.get("CXX") or env["CXX"]

env["BUILD_DEBUG"] = debug
env["BUILD_VERBOSE"] = verbose
env["BUILD_BASE"] = "build"
build_dir_name = "$BUILD_BASE/%s" % ("debug" if debug else "release")
env["BUILD_DIR"] = env.Dir(build_dir_name)
env.SConsignFile("$BUILD_DIR/.sconsign")

env["CXXFILESUFFIX"] = ".cpp" # default is ".cc"

setup_linker(env, "Program")
setup_linker(env, "SharedLibrary")
pretify_output(env, verbose, ["CC", "CXX", "LINK", "AR", "RANLIB"])

#==========================================================================

env["CONFIGUREDIR"] = "$BUILD_DIR/sconf_temp"
env["CONFIGURELOG"] = "$BUILD_DIR/config.log"
if not env.GetOption("clean"):
    print("Checking compiler warnings...")
    code = "void f(){}"
    conf = Configure(env.Clone())
    c_warnings = []
    cxx_warnings = []
    for warning in (
            "-Wabstract-vbase-init",
            "-Waddress-of-array-temporary",
            "-Warray-bounds",
            "-Warray-bounds-pointer-arithmetic",
            "-Wassign-enum",
            "-Wattributes",
            "-Wbool-conversion",
            "-Wbridge-cast",
            "-Wbuiltin-requires-header",
            "-Wc++11-narrowing",
            "-Wcast-align",
            "-Wchar-subscripts",
            "-Wconditional-uninitialized",
            "-Wconstant-conversion",
            "-Wconstant-logical-operand",
            "-Wconstexpr-not-const",
            "-Wconsumed",
            "-Wconversion",
            "-Wdangling-else",
            "-Wdangling-field",
            "-Wdeprecated",
            "-Wdeprecated-increment-bool",
            "-Wduplicate-method-match",
            "-Weffc++",
            "-Wempty-body",
            "-Wenum-conversion",
            "-Wfloat-conversion",
            "-Wfloat-equal",
            "-Wint-conversion",
            "-Wmissing-braces",
            "-Wnon-virtual-dtor",
            "-Wold-style-cast",
            "-Wparentheses",
            "-Wpointer-sign",
            "-Wshadow",
            "-Wshorten-64-to-32",
            "-Wstrict-aliasing",
            "-Wswitch",
            "-Wswitch-default",
            "-Wswitch-enum",
            "-Wtautological-compare",
            "-Wthread-safety-analysis",
            "-Wundeclared-selector",
            "-Wuninitialized",
            "-Wunreachable-code",
            "-Wunused-function",
            "-Wunused-parameter",
            "-Wunused-result",
            "-Wunused-value",
            "-Wunused-variable",
            ):

        conf.env["CFLAGS"] = ["-Werror", warning]
        if conf.TryCompile(code, ".c"):
            c_warnings.append(warning)

        conf.env["CXXFLAGS"] = ["-Werror", warning]
        if conf.TryCompile(code, ".cpp"):
            cxx_warnings.append(warning)

    assert len(c_warnings) > 0
    assert len(cxx_warnings) > 0
    env.AppendUnique(CFLAGS=c_warnings)
    env.AppendUnique(CXXFLAGS=cxx_warnings)

#==========================================================================

env.AppendUnique(
    CPPPATH=[
        "#/src",
        "#/thirdparty/strong_type/include",
        ],
    CPPDEFINES=[
    ],
    CCFLAGS=[
            "-pipe",
            "-pthread",

            "-fPIC",
            "-fstrict-aliasing",
            "-fsigned-char",

            "-ffunction-sections",
            "-fdata-sections",

            "-pedantic",
            "-Wall",
            "-Wextra",
        ],
    CXXFLAGS=[
            "-std=c++17",
        ],
    LINKFLAGS=[
            "-pthread",
            "-Wl,--gc-sections",
        ],
    RPATH=[
        ],
    )

if "clang" in env["CXX"]:
    env.AppendUnique(
            CPPDEFINES=[
                # clang does not turn on WUR checking in libstdc++
                "__attribute_warn_unused_result__=__attribute__ ((__warn_unused_result__))",
                "__wur=__attribute__ ((__warn_unused_result__))",
            ],
            CCFLAGS=[
                "-fcolor-diagnostics",
                "-ferror-limit=10",
            ],
        )
else:
    env.AppendUnique(
            CCFLAGS=[
                "-fmax-errors=10"
            ]
        )

if debug:
    env.AppendUnique(
            CPPDEFINES = ["_DEBUG"],
            CCFLAGS=[
                "-O0",
                "-ggdb",
                "-fno-omit-frame-pointer",
                "-fno-optimize-sibling-calls",
                "-fno-inline-functions",
                "-funwind-tables"
            ]
        )
else:
    env.AppendUnique(
            CPPDEFINES=["NDEBUG"],
            CCFLAGS=[
                "-O3",
                "-Werror",
                "-Wdisabled-optimization",
                "-Wno-error=strict-overflow", # gcc false warnings
            ]
        )

#==================================================================

Export("env")
SConscript("SConscript", variant_dir=env["BUILD_DIR"])

env.Execute("ln -sfrn $BUILD_DIR $BUILD_BASE/last")
