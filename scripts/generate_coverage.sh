#!/bin/bash

build/test/sdl3-example-app-tests

rm -rf coverage_report

# Kapsamı topla
lcov --capture --directory . --output-file coverage.info      

# Test kodunu hariç tut (opsiyonel)
lcov --remove coverage.info '*test*' '/usr/*' --output-file coverage_filtered.info

# HTML raporu üret
genhtml coverage_filtered.info --output-directory coverage_report

# Ayrica gcovr da uretelim
rmdir gcovr_report
mkdir gcovr_report
gcovr -r .. --html --html-details -o gcovr_report/coverage.html 