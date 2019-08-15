from setuptools import setup, Extension, find_packages
from setuptools.dist import Distribution
import sys

if sys.version_info < (3,0):
  sys.exit('Sorry, Python < 3.0 is not supported')

class BinaryDistribution(Distribution):
    """Distribution which always forces a binary package with platform name"""
    def has_ext_modules(foo):
        return True

setup(
  name             = 'pyLemma',
  version          = '0.0.4', 
  author           = 'Trevor Irons and others',
  author_email     = 'Trevor.Irons@lemmasoftware.org',
  description      = 'A short description of the app/lib',
  long_description = 'A longer one',
  classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Mozilla Public License 2.0 (MPL 2.0)',
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
    '': ['LemmaCore.*.so','FDEM1D.*.so']
  },
  zip_safe=False,
  distclass=BinaryDistribution,
)

    #ext_modules=[CMakeExtension('proj_name')],
    #cmdclass=dict(build_ext=CMakeBuild),

