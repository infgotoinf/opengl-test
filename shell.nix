{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    ccache

    clang-tools

    # Wayland dependencies
    wayland-scanner
    pkg-config
  ];

  buildInputs = with pkgs; [
    libGL

    # X11 dependencies
    libX11
    libXcursor
    libXi
    libXinerama
    libXrandr

    # Wayland dependencies
    gtk3
    libxkbcommon
  ];

  CMAKE_CXX_COMPILER_LAUNCHER = "ccache";

  # Wayland dependencies
  LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
    wayland
    libxkbcommon
  ]);
}
