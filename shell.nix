{ pkgs ? import <nixpkgs> { } }:

pkgs.clangStdenv.mkDerivation {
  name = "Hydrogen";

  nativeBuildInputs = with pkgs; [
    gdb
    nasm
  ];
}
