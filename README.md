##MoonFLTK: Lua bindings for FLTK

MoonFLTK is a Lua binding library for the [Fast Light Toolkit (FLTK)](http://www.fltk.org/).

It runs on GNU/Linux and requires [Lua](http://www.lua.org/) (>=5.3)
and [FLTK](http://www.fltk.org/software.php) (>=1.3.3).

_Authored by:_ _[Stefano Trettel](https://www.linkedin.com/in/stetre)_

[![Lua logo](./doc/powered-by-lua.gif)](http://www.lua.org/)

#### License

MIT/X11 license (same as Lua). See [LICENSE](./LICENSE).

#### Documentation

See the [Reference Manual](https://stetre.github.io/moonfltk/doc/index.html).

#### Getting and installing (on Ubuntu)

Install Lua:

```sh
$ sudo apt-get install libreadline-dev
$ wget https://www.lua.org/ftp/lua-5.3.2.tar.gz
$ tar -zxpvf lua-5.3.2.tar.gz
$ cd lua-5.3.2
lua-5.3.2$ make linux
lua-5.3.2$ sudo make install
```

Install FLTK:

```sh
$ sudo apt-get install libfltk1.3-dev
```

Download MoonFLTK's [latest release](https://github.com/stetre/moonfltk/releases)
(e.g. moonfltk-0.1.tar.gz), unpack, compile and install it:

```sh
$ tar -zxpvf moonfltk-0.1.tar.gz
$ cd moonfltk-0.1
moonfltk-0.1$ make
moonfltk-0.1$ sudo make install
```

#### Example

```lua
-- Script: hello.lua

fl = require("moonfltk")

win = fl.window(340, 180, arg[0])
box = fl.box(20, 40, 300, 100, "Hello, World!");
box:box('up box')
box:labelfont(fl.BOLD + fl.ITALIC)
box:labelsize(36)
box:labeltype('shadow')
win:done() -- 'end' is a keyword in Lua
win:show(arg[0], arg)

return fl.run()
```

The script can be executed at the shell prompt with the standard Lua interpreter:

```shell
$ lua hello.lua
```

Other examples can be found in the **examples/** directory contained in the release package
(the **examples/fltk** subdirectory contains portings to MoonFLTK of most of the examples
that come with the FLTK distribution).

#### See also

* Windowing:
[MoonGLFW](https://github.com/stetre/moonglfw),
[MoonGLUT](https://github.com/stetre/moonglut).

* Rendering:
[MoonGL](https://github.com/stetre/moongl).

* Math:
[MoonGLMATH](https://github.com/stetre/moonglmath).

* Image and model loading:
[MoonAssimp](https://github.com/stetre/moonassimp),
[MoonSOIL](https://github.com/stetre/moonsoil).

* Audio:
[LuaJACK](https://github.com/stetre/luajack).

