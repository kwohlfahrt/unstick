{ lib, stdenv, curl, meson, ninja, pkgconfig, libseccomp }:

stdenv.mkDerivation {
  name = "unstick";
  src = ./src;

  nativeBuildInputs = [ meson ninja pkgconfig ];
  buildInputs = [ libseccomp ];
}
