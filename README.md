# justomize

A command-line tool that merges/patches JSON files — like [kustomize](https://kustomize.io/) but for JSON.

## How it works

`justomize` deep-merges `base.json` with `overlay.json`:

- Keys present **only in base** are kept as-is.
- Keys present **only in overlay** are added to the result.
- Keys present **in both**: the overlay value **overrides** the base value.
- Both values are **objects**: the merge recurses into them (hierarchy is obeyed).
- **Type mismatch** (base and overlay have different JSON types for the same key): the tool errors out unless `--ignore-types` is supplied.

## Usage

```
justomize [--ignore-types] <base.json> <overlay.json> [-f <output.json>]
```

| Option | Description |
|---|---|
| `<base.json>` | Base JSON file (required) |
| `<overlay.json>` | Overlay / patch JSON file (required) |
| `-f <output.json>` | Write result to file instead of stdout |
| `--ignore-types` | Override base field even when JSON types differ |

### Examples

```sh
# Print merged JSON to stdout
justomize base.json overlay.json

# Write merged JSON to result.json
justomize base.json overlay.json -f result.json

# Ignore type mismatches
justomize --ignore-types base.json overlay.json
```

## Build

### Prerequisites

- CMake ≥ 3.20
- C++17-capable compiler (clang++ / g++)
- Internet access (to fetch [nlohmann/json](https://github.com/nlohmann/json) via CMake FetchContent)

### Native build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The binary is at `build/justomize`.

### macOS ARM (Apple Silicon)

Use the provided toolchain file:

```sh
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-mac-toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### Run tests

```sh
ctest --test-dir build --output-on-failure
```

## Dependencies

- [nlohmann/json](https://github.com/nlohmann/json) v3.11.3 — fetched automatically by CMake
