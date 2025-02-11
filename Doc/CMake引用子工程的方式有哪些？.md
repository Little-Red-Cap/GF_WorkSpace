# CMake引用子工程的方式有哪些？

[CMake最新版本文档](https://cmake.org/cmake/help/latest/index.html)

在实际项目中，我们经常会遇到需要引用其他项目的情况，比如依赖其他库、子模块等。

在本文中，我们将使用图形库 [LVGL](https://lvgl.io/) 作为示例，介绍 CMake 引用子工程常用的几种方式，并对比它们的优缺点。

### 1. `find_package()`

适用于已经 **编译并安装** 过的库，例如 `lvgl` **作为一个独立安装的 CMake 包** 时：

```cmake
find_package(lvgl REQUIRED)
target_link_libraries(my_target PRIVATE lvgl)
```

### 前提条件

* `lvgl` 必须有 CMake install() 规则，并且提供 lvglConfig.cmake 或 lvglConfigVersion.cmake。
* 你需要确保 `CMAKE_PREFIX_PATH` 包含 `lvgl` 的安装目录。

如果 `lvgl` 没有 CMake 配置文件，可以手动指定：

```c
find_package(lvgl REQUIRED PATHS "/your/custom/path")
```

### 2. `FetchContent()`
   适用于 自动从远程仓库下载并构建子工程：

```cmake

include(FetchContent)
FetchContent_Declare(
        lvgl
        GIT_REPOSITORY https://github.com/lvgl/lvgl.git
        GIT_TAG v8.3.0
)
FetchContent_MakeAvailable(lvgl)

target_link_libraries(my_target PRIVATE lvgl)
```

**优点**
* **不需要手动 clone 代码**，CMake 会自动拉取 lvgl 并构建。
* **可以指定版本**，保证项目的可复现性。

### 3. `ExternalProject_Add()`

适用于 **下载并构建外部项目**，但不会直接成为 CMake 的 `target`，而是作为 **独立的构建步骤**：

```cmake
include(ExternalProject)
ExternalProject_Add(
        lvgl_project
        GIT_REPOSITORY https://github.com/lvgl/lvgl.git
        GIT_TAG master
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lvgl-install
)
```

然后在 `my_target` 里手动包含头文件：

```cmake
include_directories(${CMAKE_BINARY_DIR}/lvgl-install/include)
link_directories(${CMAKE_BINARY_DIR}/lvgl-install/lib)
target_link_libraries(my_target PRIVATE lvgl)
```

**适用场景**
* 适用于 不需要修改 `lvgl` 源码，只想下载并编译它。

### 4. 直接使用 `add_library()` 引入预编译库

如果 `lvgl` **已经编译成静态库或动态库**，可以手动添加：

```cmake
add_library(lvgl STATIC IMPORTED)
set_target_properties(lvgl PROPERTIES
        IMPORTED_LOCATION "/path/to/liblvgl.a"
        INTERFACE_INCLUDE_DIRECTORIES "/path/to/lvgl/include"
)
target_link_libraries(my_target PRIVATE lvgl)
```

### 适用场景

* 适用于 已经有 `.a` 或 `.so/.dll` 库，不想重新编译。

### 不同方法的比较

| 方法	                      | 适用场景	           | 需要源码	 | 需要编译	 | 自动下载 |
|--------------------------|-----------------|-------|-------|------|
| add_subdirectory()	      | 子工程在同一仓库	       | ✅	    | ✅	    | ❌    |
| find_package()           | 	安装过的 CMake 库	  | ❌     | ❌	    | ❌    |
| FetchContent()	          | 自动拉取 & 构建	      | ✅	    | ✅	    | ✅    |
| ExternalProject_Add()    | 	构建外部依赖，不影响主工程	 | ✅	    | ✅	    | ✅    |
| add_library() (imported) | 	使用已有库文件	       | ❌	    | ❌	    | ❌    |

* `add_subdirectory()`：最适合 **本地子工程**，如果 lvgl 在你的源码里，推荐使用这个方法。
* `find_package()`：适合 **已经安装的库**，如果 lvgl 是官方包，可以用这个。
* `FetchContent()`：最适合 **自动下载源码**，如果 lvgl 是远程仓库但你需要修改源码，推荐使用。
* `ExternalProject_Add()`：适合 **下载并编译但不影响主工程**，适用于不修改 lvgl。
* `add_library()`**（imported）**：适合 **直接使用预编译库**。

你可以根据实际情况选择合适的方法，如果 lvgl 是你代码的一部分，add_subdirectory() 是最简单可靠的方法。