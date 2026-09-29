{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    libGL
    glfw

    # X11 dependencies
    libX11
    libX11.dev
    libXcursor
    libXi
    libXinerama
    libXrandr

    cmake
    ninja
    ccache

    clang-tools
  ];

  CMAKE_CXX_COMPILER_LAUNCHER = "ccache";
  CMAKE_GENERATOR = "Ninja";
}
