yPlusLESWCompressible
=====================

yPlusLES with the "-compressible" option, based on OpenFOAM 2.2.x (commit c698e3ffdb38148860426882e3da5fc45ff5e8e4).

It has been created for two reasons:

  * Because of this feature request: http://www.openfoam.org/mantisbt/view.php?id=802

  * And because some people also need this on OpenFOAM 2.1: http://www.cfd-online.com/Forums/openfoam/117857-yplusles.html

How to get+build it
===================

These instructions are for OpenFOAM 2.2.x:

For getting and building from git:
```
git clone git://github.com/wyldckat/yPlusLESWCompressible.git
cd yPlusLESWCompressible
git checkout of22x
wmake yPlusLESWCompressible
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/yPlusLESWCompressible/archive/of22x.zip" -O yPlusLESWCompressible.zip
cd yPlusLESWCompressible-of22x
wmake yPlusLESWCompressible
```

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
