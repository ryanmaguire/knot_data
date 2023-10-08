---
layout: default
---

# **Knots at Dartmouth**
This site contains a collection of data that knot theorists may find useful
in their experiments. This includes tables of DT codes for all prime knots
with crossing number bounded by 19 and several knot polynomials.

## **Knot Data**

The data sets are compressed using `xz` to save space. For large files over
`30MB` the maximum level of compression is used. This results in a
6:1 compression ratio, but also means it takes some time to decompress. To
do this, you'll need a tool that can handle `xz` files.

For most `GNU`, `Linux`, and `FreeBSD` users, this is built-in. If you are
using `GNOME` as your desktop environment you can right-click the file and hit
**extract**.

`macOS` users can use the built-in Archive Utility.

`Windows` users will need to download a recent version of
[**7-zip**](https://7-zip.org/).

### **Data Sets**

[**Jones Polynomial**](./jones_polynomial/)<br />
[**HOMFLY-PT Polynomial**](./homfly_polynomial/)<br />
[**Khovanov Polynomial**](./khovanov_polynomial/)

## **Tools**

The data was created using a combination of several libraries. If you wish
to reproduce this data, you'll need these installed.

1. [**regina**](https://regina-normal.github.io/)<br />
    A powerful `C++` library with routines for computing several knot
    invariants, including the Jones and Homfly polynomials.
2. [**sage**](https://www.sagemath.org/)<br />
    The knot library in `sage` contains many useful tools for converting
    between different representations of knots, and for the computation of
    certain knot polynomials.
3. [**SnapPy**](https://snappy.computop.org/)<br />
    A `Python` wrapper, and extension, of the `C` *SnapPea* library. This
    contains a valuable set of routines for working with knots.
4. [**JavaKh**](http://katlas.math.toronto.edu/wiki/Khovanov_Homology)<br />
    A `Java` library used for computing the Khovanov polynomial. The original
    version does not compile on modern versions of `OpenJDK`. Nikolay Pultsin
    made some cosmetic changes that allow it to run. These can be found at
    [**https://github.com/geometer/JavaKh-v2/**](https://github.com/geometer/JavaKh-v2/)
5. [**knot_data**](https://github.com/ryanmaguire/knot_data)<br />
    Our own set of tools and scripts.
6. [**libtmpl**](https://github.com/ryanmaguire/libtmpl)<br />
    An experimental library with new routines for the computation of knot
    invarients. Written in `C`.

