{ ocamlVersion ? "4_10" }:

let
  sources = import ./sources.nix { inherit ocamlVersion; };
  inherit (sources) pkgs overlays;
  inherit (pkgs) lib callPackage;
in
  {
    native = callPackage ./generic.nix {};

    musl64 =
      let pkgs = (import "${overlays}/static" {
        inherit ocamlVersion;
        pkgsPath = "${overlays}/sources.nix";
      });
      in
      pkgs.callPackage ./generic.nix {
        static = true;
        ocamlPackages = pkgs.ocaml-ng."ocamlPackages_${ocamlVersion}";
      };
  }
