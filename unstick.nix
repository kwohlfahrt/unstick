{ lib, stdenv, curl, meson, ninja, pkg-config, libseccomp }:

stdenv.mkDerivation {
  name = "unstick";
  src = ./src;

  nativeBuildInputs = [ meson ninja pkg-config ];
  buildInputs = [ libseccomp ];
}
