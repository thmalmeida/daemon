#!/bin/bash

# build directory name
DBUILD=build

Blue='\e[0;34m'
Green='\e[0;32m'
Purple='\e[0;35m'
Red='\e[0;31m'
Reset='\e[0m'

echo -e "${Purple}Run start${Reset}"

if [ "$1" = "build" ] || [ -z "$1" ]; then
  if [ ! -d "$DBUILD" ]; then
    echo -e "${Green}Creating build directory.${Reset}"
    mkdir $DBUILD
  fi

  cd build
  echo -e "${Green}Starting cmake..${Reset}"
  cmake ..
  echo -e "${Green}Making project...${Reset}"
  make

  if [ -z "$1" ]; then
    echo -e "${Purple}Running...${Reset}"
    ./Daemon0
  fi
fi

if [ "$1" = "clean" ]; then
  if [ -d "$DBUILD" ]; then
      cd build
      echo -e "${Green}Clearing project${Reset}"
      make clean
  fi
fi

if [ "$1" = "fullclean" ]; then
  if [ -d "$DBUILD" ]; then
    echo -e "${Red}Erasing project${Reset}"
    rm -rf build
  fi
fi



