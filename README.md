# oot3d_practice
This is a project to make some useful practice tools for oot3d. I decided to work on it because I wanted to use these tools. It is greatly inspired by glank's oot64 practice rom. Also most of the code in here, the patch generation, and reverse engineering work here is originally due to n3rdswithgame (I stole the code and didn't know about forks at the time <3)

------------------------------------------------------------------------

## How to use:

USA Version:
On the root of your SD card, create the folder /luma/titles/0004000000033500 and place code.ips inside it. Hold Select while you power on the 3DS to launch Luma and make sure that "Enable game patching" is turned on. Launch the game.

JP Version:
On the root of your SD card, create the folder /luma/titles/0004000000033400 and place code.ips from the regions/j folder inside it. Hold Select while you power on the 3DS to launch Luma and make sure that "Enable game patching" is turned on. Launch the game.

------------------------------------------------------------------------

## TODO:
-Find fadeout timer so I can do entrance select at any time.

-Implement age select.

-Ideally create a menu for entrance select and age select. This will require some method to allow larger patches.

-A better system for inventory modification, preferably where you can more easily see what you're modifying.

------------------------------------------------------------------------

## License

 Some of the files in this repo are modified versions of files from [devkitPro](https://devkitpro.org/wiki/Getting_Started) and [libctru](https://github.com/smealum/ctrulib). Namely the Makefiles and some implementations of the syscalls wrapper functions. To comply with their licensing here is the copy of their license / how I'm licensing this repo.

------------------------------------------------------------------------

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any
  damages arising from the use of this software.

  Permission is granted to anyone to use this software for any
  purpose, including commercial applications, and to alter it and
  redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you
     must not claim that you wrote the original software. If you use
     this software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and
     must not be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.
