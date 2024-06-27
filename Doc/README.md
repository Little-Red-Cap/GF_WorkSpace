## How to Clone

This repository contains other, necessary GF_WorkSpace software repositories as
[git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules). Those
submodules are not pulled in with the normal git clone command and they will be
needed. There are a couple of techniques to pull in the submodules.

### Everything at Once

This command will clone the this repository and all submodules
in a single step.

```
git clone --recurse-submodules https://github.com/Little-Red-Cap/GF_WorkSpace.git
```

### Main Repository First, Submodules Second

If you've already cloned the main repository you can pull in the submodules
with a second command. Both commands are shown below.

```
git clone https://github.com/Little-Red-Cap/GF_WorkSpace.git
cd GF_WorkSpace
git submodule update --init --recursive
```

### Keeping Your Clone Up-To-Date

If you have cloned this repository and would like to pull in the latest
changes, you will have to do this in two steps. The first step will pull in
updates to the main repo, including updated _references_ to the submodules. The
second step will update the code in the submodules to match those references.
The two commands needed to accomplish this are shown below, run these commands
from inside the main repository's directory (top level `GF_WorkSpace`
directory works fine).

```
git pull
git submodule update --init --recursive
```

If you have chosen to fork this repository then updating the fork from upstream
will require a different, more involved procedure.

## Third-party libraries

| Library               | Address                                                       | License    | Link                                               |
|-----------------------|---------------------------------------------------------------|------------|----------------------------------------------------|
| FATFS                 | --                                                            | --         | http://elm-chan.org/fsw/ff/00index_e.html          |
| MicroPython           | --                                                            | --         |                                                    |
| QRCode                | --                                                            | --         |                                                    |
| FreeType              | ThirdParty/FreeType/raw                                       |            | https://freetype.org/                              |
| SGL                   | ThirdParty/SGL/raw                                            | --         |                                                    |
| CmBackTrace           | ThirdParty/CmBackTrace/raw                                    | --         | https://github.com/armink/CmBacktrace              |
| Hi-Link FPM383C-80x64 | --                                                            | --         |                                                    |
| setjmp                | source/hal/arch/x86/win                                       | BSD        |                                                    |
| PLOOC                 | ThirdParty/PLOOC/raw                                          | Apache 2.0 | https://github.com/GorgonMeducer/PLOOC             |
| btstack               | source/component/3rd-party/btstack/raw                        | Other      | https://github.com/bluekitchen/btstack             |
| coremark              | source/component/3rd-party/coremark/raw                       | Apache     | https://github.com/eembc/coremark                  |
| zlib                  | source/component/3rd-party/zlib/raw                           | zlib       | http://zlib.net/                                   |
| nuklear               | source/component/3rd-party/nuklear/raw                        | MTI        | https://github.com/Immediate-Mode-UI/Nuklear       |
| nnom                  | source/component/3rd-party/nnom/raw                           | Apache 2.0 | https://github.com/majianjia/nnom                  |
| LUA                   | ThirdParty/lua/raw                                            | MIT        | https://www.lua.org/                               |
| lwip                  | source/component/3rd-party/lwip/raw                           | BSD        | https://savannah.nongnu.org/projects/lwip/         |
| libpng                | source/component/3rd-party/libpng/raw                         | PNG2       | https://libpng.sf.net                              |
| libjpeg-turbo         | source/component/3rd-party/libjpeg-turbo/raw                  | BSD        | https://libjpeg-turbo.org/                         |
| SDL_ttf               | source/shell/media/sdl2/3rd-party/SDL_ttf                     | zlib       | https://hg.libsdl.org/SDL_ttf/                     |
| SDL_image             | source/shell/media/sdl2/3rd-party/SDL_image                   | zlib       | https://hg.libsdl.org/SDL_image/                   |
| lvgl                  | ThirdParty/lvgl/raw/lvgl                                      | MIT        | https://lvgl.io/                                   |
| lv_lib_freetype       | source/component/3rd-party/lvgl/extension/lv_lib_freetype/raw | MIT        | https://lvgl.io/                                   |
| CMSIS                 | source/utilities/compiler/arm/3rd-party/CMSIS                 | Apache 2.0 | https://github.com/ARM-software/CMSIS_5            |
| GuiLite               | ThirdParty/GuiLite/raw                                        | Apache 2.0 | https://github.com/idea4good/GuiLite               |
| LittleFS              | ThirdParty/littlefs/raw                                       | BSD        | https://github.com/littlefs-project/littlefs       |
| evm                   | source/component/3rd-party/evm/raw                            | Apache 2.0 | https://github.com/scriptiot/evm                   |
| LingLongGUI           | source/component/3rd-party/LingLongGUI/raw                    | Apache 2.0 | https://gitee.com/gzbkey/LingLongGUI               |
| mbedtls               | source/component/3rd-party/mbedtls/raw                        | Apache 2.0 | https://tls.mbed.org/                              |
| Segger_RTT            | source/component/3rd-party/segger/raw/RTT                     | segger     | https://wiki.segger.com/RTT                        |
| Segger_SystemView     | source/component/3rd-party/segger/raw/SystemView              | segger     | https://wiki.segger.com/SystemView                 |
| nuconsole             | source/component/3rd-party/nuconsole/raw                      | nuvoton    | https://www.nuvoton.com.cn/                        |
| getopt_long           | source/shell/sys/linux/lib/3rd-party/getopt                   | OpenBSD    | https://github.com/openbsd/src                     |
| regex                 | source/shell/sys/linux/lib/3rd-party/regex                    | OpenBSD    | https://github.com/openbsd/src                     |
| fnmatch               | source/shell/sys/linux/lib/3rd-party/fnmatch                  | BSD        | http://www.jbox.dk/sanos/source/lib/fnmatch.c.html |
| glob                  | source/shell/sys/linux/lib/3rd-party/glob                     | BSD        | https://github.com/cloudius-systems/musl           |
| libtuv                | source/shell/sys/linux/lib/3rd-party/libtuv/raw               | Apache 2.0 | https://github.com/Samsung/libtuv                  |






