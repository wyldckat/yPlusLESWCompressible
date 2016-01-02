yPlusLESWCompressible
=====================

yPlusLES with the "-compressible" option, based on OpenFOAM® 2.2.x (commit c698e3ffdb38148860426882e3da5fc45ff5e8e4).

It has been created for two reasons:

  * Because of this feature request: http://www.openfoam.org/mantisbt/view.php?id=802

  * And because some people also need this on OpenFOAM 2.1: http://www.cfd-online.com/Forums/openfoam/117857-yplusles.html

This [git repository](https://github.com/wyldckat/yPlusLESWCompressible) was brought to you by Bruno Santos (wyldckat@github working at [blueCAPE Lda](http://www.bluecape.com.pt)).


Warning
-------

There was a bug fixed on OpenFOAM 2.2.x related to this:

  * http://www.openfoam.org/mantisbt/view.php?id=1141

  * https://github.com/OpenFOAM/OpenFOAM-2.2.x/commit/5cc33a23168577c9f91564fe6117f4ec1bde1348

Although the bug fix was applied to `yPlusLESWCompressible`, there are some small discrepancies in results, because the `rho` field being used is not exactly the same.

There is an issue open for fixing this, so feel free to contribute: https://github.com/wyldckat/yPlusLESWCompressible/issues/1



Disclaimer and License
======================

This offering is not approved or endorsed by OpenCFD Limited, the producer of the OpenFOAM software and owner of the OPENFOAM® and OpenCFD® trade marks.

This is bound to the same license as OpenFOAM, namely GNU Public License v3. Quoting from OpenFOAM's license statement:

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM. If not, see <http://www.gnu.org/licenses/>.



How to get+build yPlusLESWCompressible
======================================

Follow the instructions from the respective branch in the same section "How to get+build yPlusLESWCompressible":

  * For OpenFOAM 2.1.0: https://github.com/wyldckat/yPlusLESWCompressible/tree/of210#how-to-getbuild-yplusleswcompressible

  * For OpenFOAM 2.1.1 and 2.1.x: https://github.com/wyldckat/yPlusLESWCompressible/tree/of21x#how-to-getbuild-yplusleswcompressible

  * For OpenFOAM 2.2.0 to 2.2.x: https://github.com/wyldckat/yPlusLESWCompressible/tree/of22x#how-to-getbuild-yplusleswcompressible

  * For OpenFOAM 2.3.0 to 2.3.x: https://github.com/wyldckat/yPlusLESWCompressible/tree/of23x#how-to-getbuild-yplusleswcompressible

  * For all of the other more recent versions of OpenFOAM (including OpenFOAM-dev), this is already implemented.

How to use it
=============

After you run a case using LES, then run accordingly:

  * For an incompressible case: `yPlusLESWCompressible`

  * For a compressible case: `yPlusLESWCompressible -compressible`

Test cases
==========

Inside the folder `testCases`, you can find a couple of test cases. Inside each case are a couple of `All*` scripts:

  * To run each case, simply run: `./Allrun`

  * To clean up the case, simply run: `./Allclean`

Once each case is executed, you can check the error of the "yPlusLES" vs "yPlus" field, the first being calculated by using the function object version, versus the second one calculated by `yPlusLESWCompressible` version. The resulting difference is given in the field "yPerror".

Authorship
==========

This variant of the "yPlusLES" application was created by Bruno Santos (wyldckat@github).

Code is based on the original "yPlusLES" from OpenFOAM 2.2.x, as well as:
  * from "yPlusRAS", got the information for the basis of the changes.
  * from "src/postProcessing/functionObjects/utilities/yPlusLES" got how the compressible version of yPlusLES is calculated.
  * from "rhoPimpleFoam", got some ideas on how to set-up the compressible part.

So in essence, it was a lot of _precision copy-paste-adapt_ coding.

