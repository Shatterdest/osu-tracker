[![Build osu-tracker](https://github.com/nyaruku/osu-tracker/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=master&style=flat)](https://github.com/nyaruku/osu-tracker/actions/workflows/cmake-multi-platform.yml)
[![CodeQL Advanced](https://github.com/nyaruku/osu-tracker/actions/workflows/codeql.yml/badge.svg?branch=master)](https://github.com/nyaruku/osu-tracker/actions/workflows/codeql.yml)
# osu-tracker
A web server based osu! stats/session tracker

## How to use
1. run executable
2. visit http://127.0.0.1:10727

## Bug Reporting / Questions
- Join my Discord Server and just ask me:
- https://discord.gg/FN6vauFTGA

## Credits
- osu! API by osu!
- Score Inspector API by Amayakase
- Score Rank API by Respektive


## How to build from source:
- Windows
  - Visual Studio (Recommended)
- Linux
  - Make sure u have these installed:
    - cmake
    - libcurl4-openssl-dev (provides lib files for linking) | (curl is enough, if u are using arch)
    - ninja (generator)
  - Use bash scripts
    - "release.sh" , "debug.sh"
    - Usage
      
      Configure
      > ./release.sh configure
      
      Build
      > ./release.sh build

## TODO:
- Overlay (Browser/Text based)
- Save Sessions
- Compare overall stats between sessions
- Custom Config Settings
- Auto updater
- Local Database of Clears/Maps/Stats ?????