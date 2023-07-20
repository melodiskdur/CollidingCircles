# Clear any old build files before running cmake.
Remove-Item -Path ".\build\*" -Force -Recurse -ErrorAction SilentlyContinue

# Generate build system files
cmake -S . -B build -G "Visual Studio 16 2019" -A x64

# Navigate to the build directory
cd build

# Build the project
cmake --build . --config Release