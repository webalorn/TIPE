# Copyright 1999-2008 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="A csv to LaTeX file converter"
HOMEPAGE="http://brouits.free.fr/csv2latex/"
SRC_URI="${HOMEPAGE}/${P}-0.13.tar.gz"

LICENSE="GPL"
SLOT="0"
KEYWORDS="~x86"
IUSE=""

src_compile() {
    emake || die "emake failed"
}

src_install() {
    PREFIX="usr/local"
    mkdir -p ${D}/${PREFIX}/bin
    emake install DESTDIR="${D}" PREFIX=${PREFIX} || die "Install failed"
}
