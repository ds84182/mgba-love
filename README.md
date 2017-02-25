# mgba-love

A shitty attempt at getting mGBA to run a game in Love2D through FFI.

## Usage

First you need to compile mGBA. The master branch SHOULD be fine, but my tests were on commit `849f80e7a1959923c9d3d47191d0a162f0887bb9`.

After you compile mGBA, drop it in this directory under `native/<arch>/libmgba.<ext>`, where `<arch>` is an arch supported by LuaJIT (so the value of `ffi.arch`).
`ffi.arch` should evaluate to "x86", "x64", "arm", "ppc", "ppcspe", or "mips".

Then run it using Love2D (any version from 0.8.0 onward should be fine). It takes the game as an argument.

## What works?

* Loading a game
* Video output

## What doesn't work?

* Audio output
* Save files
* Save states
* You tell me! (or not, I don't really care)
