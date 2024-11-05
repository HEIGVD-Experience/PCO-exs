{
  description = "A Nix-flake-based C/C++ development environment";

  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0.1.*.tar.gz";

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit system; };
      });
    in
    {
      devShells = forEachSupportedSystem
        ({ pkgs }:
          let
            dependencies = with pkgs;
              [
                clang-tools
                cmake
                codespell
                conan
                cppcheck
                doxygen
                gtest
                lcov
                vcpkg
                vcpkg-tool
                qt5.full
                (pkgs.python3.withPackages (python-pkgs: with python-pkgs; [
                  pandas
                  matplotlib
                ]))
              ] ++ (if system == "aarch64-darwin" then [ ] else [ gdb ]);


            pcosynchro = pkgs.stdenv.mkDerivation
              rec {
                pname = "pcosynchro";
                version = "1.0.0"; # You may want to adjust this version

                src = pkgs.fetchgit {
                  url = "https://reds-gitlab.heig-vd.ch/reds-public/pco-synchro.git";
                  rev = "3cc9592b"; # You might want to pin this to a specific commit or tag
                  sha256 = "sha256-kc4DvDdiTQjJ2WKGjXC2m9QQBwxvEupw+WeNJRwNkTE="; # You'll need to add the correct SHA256 hash here
                };

                buildInputs = [ pkgs.qt5.qtbase ];
                nativeBuildInputs = [ pkgs.qt5.wrapQtAppsHook ];

                preConfigure = ''
                  cd lib/pcosynchro
                '';

                installPhase = ''
                  runHook preInstall
                  qmake
                  make -j4
                  make install INSTALL_ROOT=$out
                  runHook postInstall
                '';

                postInstall = ''
                  mv $out/usr/local/* $out/
                  rm -r $out/usr
                '';
              };
          in
          {
            default = pkgs.mkShell.override
              {
                # Override stdenv in order to change compiler:
                # stdenv = pkgs.clangStdenv;
              }
              {
                packages = dependencies ++ [ pcosynchro ];

                TEST_ENV = "${pcosynchro}/lib";
              };
          });
    };
}