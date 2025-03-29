#!/bin/bash
cmake -S . -B build
cmake --build build
./build/four_in_a_row

