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

## IAQ

#### What the hell is an IAQ?

Infrequently Asked Questions. Next question.

#### Does this work on Android?

Probably.

#### Is this going to replace mGBA Android?

Fuck no, lmao.

#### "Why don't you provide X?"

Why don't you provide me money? Next question.

#### I played through the entirety of _Space Memes 2: Electric Boogaloo, featuring Nintendo Switch from the Devil May Cry series_!

Cool. That isn't a question.

#### I played through the entirety of _Space Memes 2: Electric Boogaloo, featuring Nintendo Switch from the Devil May Cry series_ and saved but no save file was created!

Cool. That isn't a question.

#### I ran mgba-love and now my cat is gone and my arm feels numb.

Cool. Sounds like a personal problem.

#### Every time I try to run it I get an error saying "Unsupported arch: X"

That isn't a question, but this is:

Have you tried turning it off and on again?

How about compiling the native library, you nincompoop.

#### Every time I try to run it I get an error saying "Unsupported operating system: X"

That isn't a question... Create an issue I guess ¯\\\_(ツ)\_/¯.

#### It crashes with "Segmentation Fault" or "Windows has found a problem"!

That isn't a question.

Have you tried compiling mGBA at the revision above?

If you want to support mGBA 2.0 (which hopefully has 3DS support... maybe Half Life 3 is out for the Nintendo Switch), you need to piece together another `mgba.h` file, which is trivial if your brain `#include`s a C preprocessor.

#### Are you bored?

Yes.