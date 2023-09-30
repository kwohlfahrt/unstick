{
  description = "Utility to filter out sticky-bit setting";

  inputs.nixpkgs.url = "nixpkgs";

  # adapted from https://hoverbear.org/blog/a-flake-for-your-crate/#flake-nix
  outputs = { self, nixpkgs } : let
    systems = [ "x86_64-linux" "aarch64-linux" ];
    forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f system);
  in {
    defaultPackage = forAllSystems (system: (import nixpkgs { inherit system; overlays = [ self.overlay ]; }).unstick);
    overlay = self: super: { unstick = self.callPackage ./unstick.nix {}; };
  };
}

