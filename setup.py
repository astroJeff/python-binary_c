from distutils.core import setup, Extension


# binary_c must be installed. In this case, add libbinary_c.so to /usr/local/lib and run ldconfig as sudo

setup(
    name='binary_c',
    version='0.1',
    description='This is a python API for binary_c by Rob Izzard and collaborators',
    author='Jeff Andrews',
    author_email='andrews@physics.uoc.gr',
    license='',
    ext_modules=[Extension("binary_c",
                           ["binary_c_python.c"],
                           libraries=["binary_c", "bfd", "binary_c_api"],
                           library_dirs = ['../src', './'],
                           )]  # binary_c must be loaded
)
