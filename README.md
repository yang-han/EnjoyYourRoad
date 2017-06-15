# Enjoy Your Road
An openGL game.

All platforms are supported.

| Windows 8.0+ | Linux (Deepin Desktop 15.4) | MacOS  |
| ------------ | --------------------------- | ------ |
| passed       | passed                      | passed |



## 0. Required

### 0.1 Mandatory

   - openGL 330 version +
   - all other libraries have been linked to the binary file.

### 0.2 Optional

(This is used to run the start interface. It can be used to change the map, though we just implement one map.)

- python 3.x
- tkinter

## 1. How to run it

### 1.1 Binay package

You can 

```shell
python3 run.py
```

Or just run the `main` executable file

```shell
./main # For *nix system
main # For Windows
```

Also, `double click` to run it is also acceptable.

In fact, we will use the Command Line Arguments to change the Map in the game in the future. (That's also why we need a start interface.)

### 1.2 Build from source

```shell
# in the root dir of source
mkdir build
cd build
cmake ..
make
cd ..
python3 run.py # or ./main
```



## 2. Tasks List 

- [x] LoadObj  (Function is weak, only simple *.obj can be loaded)
- [x] LoadDDS (Read in the texture)
- [x] control (Press key will trigger a lot of Matrix computation to make sure you can see a world like real.)
- [x] AABB box (for collide detection)
- [x] real-time light
- [x] real-time shadow map
- [x] sky box (for a infinite view)
- [x] terrain generator
- [x] simple physical engine (very very very naive and simple)
- [ ] OBB box
- [ ] pariticles system
- [ ] battle and life system
- [ ] more maps


## 3. Things I'd like to mention

In order to hand in the homework, I (with two partners) try hard to learn openGL using variaties of shaders. Within one week's spare time, we finally completed this, exhaustedly.

All the models(*.obj) are created by hand, with no professional model constructor, and we nearly used no libraries, though some codes are learned from some tutorials on the web.

## 4. Contributors

(No priority)

YANG Han

WANG Daxin

WEN Zihao

