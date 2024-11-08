& cmake -S . -B build "-DCMAKE_C_COMPILER:FILEPATH=clang.exe" -G Ninja
& cmake --build build
& .\build\four_in_a_row.exe



