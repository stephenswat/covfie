.. _installation:

Installation
============

The library part of covfie is a header-only library, leaving the user with many
ways to install it -- including simply copying the headers to a location
accessible by the dependent project. However, the recommended way to install
(and use) covfie is through CMake. In order to install it, obtain the source
code from Github:

.. code-block:: console

    $ git clone git@github.com:acts-project/covfie.git

Alternatively, if you would like to obtain a specific version of the library,
you can specify this desire as follows:

.. code-block:: console

    $ git clone git@github.com:acts-project/covfie.git --branch v0.1.0

It is also possible to obtain the source code through other means, of course,
but we will not cover those methods here.

Requirements
------------

Installing the library requires CMake 3.21 or newer. Compiling code against it
requires a C++20 compiler, as the headers use concepts. The CUDA, SYCL and HIP
parts of the library additionally require the compiler and runtime of the
platform in question.

Building the tests, examples or benchmarks has further requirements, which are
covered in :ref:`the build guide <building>`.

Installing with CMake
---------------------

You will need to determine where you want to install covfie. In the following
instructions, the installation path will be denoted :code:`[prefix]`. To
install covfie, proceed with the following commands:

.. code-block:: console

    $ cmake -S covfie -B covfie_build -DCMAKE_INSTALL_PREFIX=[prefix]
    $ cmake --build covfie_build
    $ cmake --install covfie_build

This places the headers for all platforms in :code:`[prefix]/include/covfie`
and the CMake package files in :code:`[prefix]/share/covfie/cmake`. The
:code:`COVFIE_PLATFORM_*` options have no effect here, as they only select
which platform code is built for the tests, examples and benchmarks.

Using the headers directly
--------------------------

Because the library is header-only, you can also skip CMake and point the
compiler at the headers yourself, either in an installation prefix:

.. code-block:: console

    $ c++ -std=c++20 -I [prefix]/include my_application.cpp

or straight from a checkout, without installing anything at all:

.. code-block:: console

    $ c++ -std=c++20 -I covfie/lib/core my_application.cpp

The headers for the other platforms live in :code:`covfie/lib/cpu`,
:code:`covfie/lib/cuda`, :code:`covfie/lib/sycl` and :code:`covfie/lib/hip`.
