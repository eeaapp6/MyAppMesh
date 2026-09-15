mkdir output
cd output
mkdir bin
cd ../

cp -r ./Tools/Linux64/dime/lib/* ./output/bin
cp -r ./Tools/Linux64/OCC/lib/* ./output/bin
cp -r ./Tools/Linux64/PythonQt/lib/* ./output/bin
cp -r ./Tools/Linux64/hdf5/lib/* ./output/bin
cp -r ./Tools/Linux64/Python37 ./output/