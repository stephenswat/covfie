.. _building:

Building
========

The library itself is header-only, so building covfie is only necessary if you
want to run the tests, the examples or the benchmarks. The build is driven by
CMake and requires CMake 3.21 or newer and a C++20 compiler.

Build options
-------------

The following options select which parts of the repository are built. All three
are off by default, and all three require covfie to be the top level project;
they are rejected if covfie is pulled in through :code:`add_subdirectory` or
:code:`FetchContent`.

.. list-table::
    :header-rows: 1

    * - Option
      - Effect
    * - :code:`COVFIE_BUILD_TESTS`
      - Build the test executables.
    * - :code:`COVFIE_BUILD_EXAMPLES`
      - Build the example executables.
    * - :code:`COVFIE_BUILD_BENCHMARKS`
      - Build the benchmark executables.

The platform options below decide which platforms that code is built for. Only
:code:`COVFIE_PLATFORM_CPU` is on by default. They do not affect which headers
are installed.

.. list-table::
    :header-rows: 1

    * - Option
      - Effect
    * - :code:`COVFIE_PLATFORM_CPU`
      - Enable building of CPU code. On by default.
    * - :code:`COVFIE_PLATFORM_OPENMP`
      - Enable building of OpenMP code.
    * - :code:`COVFIE_PLATFORM_CUDA`
      - Enable building of CUDA code.
    * - :code:`COVFIE_PLATFORM_SYCL`
      - Enable building of SYCL code.
    * - :code:`COVFIE_PLATFORM_HIP`
      - Enable building of HIP code.

Finally, :code:`COVFIE_FAIL_ON_WARNINGS` turns compiler warnings into errors,
which is what the continuous integration uses.

Dependencies
------------

The tests need Google Test and the benchmarks need Google Benchmark. Neither is
downloaded during the build, so they have to be available beforehand. The
easiest way to get them is the Spack environment included in the repository:

.. code-block:: console

    $ spack env create covfie spack.yaml
    $ spack -e covfie concretize -f
    $ spack -e covfie install
    $ spack env activate covfie

If you already have them installed somewhere else, point CMake at that location
with :code:`-DCMAKE_PREFIX_PATH=[path]` instead.

Building and running the tests
------------------------------

.. code-block:: console

    $ cmake -S covfie -B build -DCOVFIE_BUILD_TESTS=On
    $ cmake --build build

This produces one test executable per enabled platform. With the default
options that is:

.. code-block:: console

    $ ./build/tests/core/test_core
    $ ./build/tests/cpu/test_cpu

The tests are not registered with CTest, so run the executables directly rather
than through :code:`ctest`. Each of them is an ordinary Google Test binary, so
the usual flags such as :code:`--gtest_filter` work as expected.

Building the examples
---------------------

The examples are built in the same way, and end up in :code:`build/examples`:

.. code-block:: console

    $ cmake -S covfie -B build -DCOVFIE_BUILD_EXAMPLES=On
    $ cmake --build build
