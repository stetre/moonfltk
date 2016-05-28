
## How to install MoonFLTK

### On Ubuntu

##### Install Lua

```sh
$ sudo apt-get install libreadline-dev
$ wget https://www.lua.org/ftp/lua-5.3.2.tar.gz
$ tar -zxpvf lua-5.3.2.tar.gz
$ cd lua-5.3.2
lua-5.3.2$ make linux
lua-5.3.2$ sudo make install
```

##### Install FLTK

```sh
$ sudo apt-get install libfltk1.3-dev
```

##### Install MoonFLTK

Download MoonFLTK's [latest release](https://github.com/stetre/moonfltk/releases)
(e.g. moonfltk-0.1.tar.gz).

```sh
$ tar -zxpvf moonfltk-0.1.tar.gz
$ cd moonfltk-0.1
moonfltk-0.1$ make
moonfltk-0.1$ sudo make install
```

### On Windows with MSYS2/MinGW

##### Set up the MSYS2/MinGW environment

Download the [MSYS2 installer](https://msys2.github.io/) and
follow the instructions from the download page.

In the MSYS2 shell:

```sh
$ pacman -S make tar wget
$ pacman -S mingw-w64-i686-gcc            # (32bit)
$ pacman -S mingw-w64-x86_64-gcc          # (64bit)
```

Assuming MSYS2 is installed in _C:\msys32_, append the following paths
to the PATH environment variable:
- _C:\msys32\mingw\bin_   (corresponding to _/mingw/bin_ under MSYS2), and
- _C:\msys32\usr\local\bin_ (corresponding to _/usr/local/bin_ . We will install the DLLs here).

(To edit PATH, right click My Computer -> Properties -> Advanced ->  Environment variables).

##### Install Lua

```sh
$ pacman -S libreadline-devel
$ wget https://www.lua.org/ftp/lua-5.3.2.tar.gz
$ tar -zxvf lua-5.3.2.tar.gz
$ cd lua-5.3.2
lua-5.3.2$ make mingw
lua-5.3.2$ make test
lua-5.3.2$ make install 
lua-5.3.2$ cp src/lua53.dll /usr/local/bin
```


##### Install FLTK

Download the latest [FLTK sources](http://www.fltk.org) (e.g. fltk-1.3.3-source.tar.gz).

```sh
$ tar -xvf fltk-1.3.3-source.tar.gz
$ cd fltk-1.3.3
fltk-1.3.3$ ./configure
fltk-1.3.3$ make
fltk-1.3.3$ make install
```
##### Install MoonFLTK

Download MoonFLTK's [latest release](https://github.com/stetre/moonfltk/releases)
(e.g. moonfltk-0.1.tar.gz).

```sh
$ tar -zxpvf moonfltk-0.1.tar.gz
$ cd moonfltk-0.1
moonfltk-0.1$ make platform=mingw
moonfltk-0.1$ make install platform=mingw
```

