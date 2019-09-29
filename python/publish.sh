# manylinux 
rm -rf build dist clean pyLemma.egg.info
python setup.py build
python setup.py bdist_wheel
auditwheel repair ./dist/pyLemma*.whl -w ./clean
twine upload clean/*

#rm -rf dist
#python setup.py build
#python setup.py bdist_wheel
#auditwheel repair --plat manylinux2010_x86_64  pyLemma-0.0.2-cp37-cp37m-linux_x86_64.whl
#twine upload dist/*
