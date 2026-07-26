# SwitchLive

Live streaming client for Nintendo Switch.

## Features

- Bilibili, Douyu, Huya, Douyin live streams
- Category browsing and room list
- Quality selection
- Controller navigation (D-pad + A/B)

## Build

### Desktop (test)

```bash
# Install deps (Ubuntu/Debian)
sudo apt-get install cmake g++ libcurl4-openssl-dev nlohmann-json3-dev libglfw3-dev libgl-dev

# Clone borealis
mkdir -p library && cd library
git clone https://github.com/nickoala/borealis.git
cd ..

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./SwitchLive
```

### Nintendo Switch

```bash
# Install devkitPro
sudo dkp-pacman -S switch-dev switch-tools

# Clone borealis
mkdir -p library && cd library
git clone https://github.com/nickoala/borealis.git
cd ..

# Build
mkdir build && cd build
cmake .. -DPLATFORM_SWITCH=ON
make -j$(nproc)
```

## Controls

| Button | Action |
|--------|--------|
| D-pad | Navigate |
| A | Select |
| B | Back |
| + | Quit |

## Credits

- [wiliwili](https://github.com/908468180/wiliwili)
- [borealis](https://github.com/nickoala/borealis)
- [dart_simple_live](https://github.com/xiaoyaocz/dart_simple_live)
