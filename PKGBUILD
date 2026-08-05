# Maintainer: armoredvortex <https://github.com/armoredvortex>
pkgname=sudodoku
pkgver=1.0.0
pkgrel=1
pkgdesc="Fast, distraction-free, cli-first Sudoku for Linux"
arch=('x86_64')
url="https://github.com/armoredvortex/sudodoku"
license=('MIT')
depends=('libx11' 'gcc-libs')
makedepends=('gcc' 'make')
install=sudodoku.install
source=("$pkgname-$pkgver.tar.gz::https://github.com/armoredvortex/sudodoku/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('e36a622d5631b13154231ff729b00b1e3e35c87a7ce2d3180389e6252637aca9')

prepare() {
    cd "$pkgname-$pkgver"
    # Regenerate makefiles with the install prefix baked in
    cd build
    INSTALL_PREFIX=/usr ./premake5 gmake
    cd ..
}

build() {
    cd "$pkgname-$pkgver"
    make config=release_x64 PREFIX=/usr
}

package() {
    cd "$pkgname-$pkgver"
    make -f install.make install PREFIX=/usr DESTDIR="$pkgdir"
}
