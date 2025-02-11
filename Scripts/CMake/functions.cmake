# 定义一个函数递归添加头文件目录
function(add_include_directories_recursively dir)
    # 添加当前目录为头文件路径
    include_directories(${dir})
    # 获取当前目录下的所有子目录
    file(GLOB children RELATIVE ${dir} "${dir}/*")
    foreach(child ${children})
        if(IS_DIRECTORY ${dir}/${child})
            # 对于每个子目录，递归调用该函数
            add_include_directories_recursively(${dir}/${child})
        endif()
    endforeach()
endfunction()


function(add_source_files target)
    foreach(file ${ARGN})
        target_sources(${target} PRIVATE ${file})
    endforeach()
endfunction()


function(remove_source_files from_source_list)
    foreach (file ${ARGN})
        list(REMOVE_ITEM source_list ${file})
    endforeach ()
endfunction()

