tar -xf ./dependency.tar -C ./QMIDIPlayer
mkdir ./build
cd ./build
qmake ../QMIDIPlayer/QMIDIPlayer.pro
mingw32-make -j16
cd ..
mkdir ./deploy
copy ./build/release/QMIDIPlayer.exe ./deploy/QMIDIPlayer.exe
copy ./QMIDIPlayer/lib/SDL2.dll ./deploy/SDL2.dll
copy ./README.md ./deploy/README.txt
windeployqt --no-translations --no-webkit2 --compiler-runtime --no-opengl-sw ./deploy/QMIDIPlayer.exe