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
sha256sums=('5606aa7e99864675aaec359084dbe06d1fa6363da9bc977f1643c95c18ca4e16')

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
