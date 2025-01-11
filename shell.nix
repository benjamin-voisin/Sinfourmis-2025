let
  pkgs = import <nixpkgs> { };
in
pkgs.mkShell {
  packages = [
    pkgs.cmake
    pkgs.python3
    # pkgs.npm
    pkgs.nlohmann_json
  ];
}
