#from distutils.core import setup
from setuptools import setup, Extension, find_packages

import sys
if sys.version_info < (3,0):
  sys.exit('Sorry, Python < 3.0 is not supported')

setup(
  name             = 'pyLemma',
  version          = '0.0.2', 
  author           = 'Trevor Irons and others',
  author_email     = 'Trevor.Irons@lemmasoftware.org',
  description      = 'A short description of the app/lib',
  long_description = 'A longer one',
  classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MPL License',
        'Topic :: Scientific/Engineering',
        'Programming Language :: Python :: 3',
  ],
  #packages    = ['pyLemma.pyFDEM1D'],
  packages=find_packages(),
  #package_dir = {
  #  '': '../'
  #},
  include_package_data=True,
  package_data = {
    #'pyLemma': ['pyFDEM1D.*.so']
    #'pyLemma.pyFDEM1D': ['pyFDEM1D.*.so']
    #'pyLemma.pyFDEM1D': ['pyFDEM1D.*.so']
    '': ['pyFDEM1D.*.so']
  },
  zip_safe=False,
)


    #ext_modules=[CMakeExtension('proj_name')],
    #cmdclass=dict(build_ext=CMakeBuild),

