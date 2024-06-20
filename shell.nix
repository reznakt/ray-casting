{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    SDL2
    SDL2_image
    SDL2_gfx
    llvmPackages.openmp
  ];
}
