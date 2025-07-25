cmake_minimum_required(VERSION 3.8)  # 不要修改
project(CodeCraft)                   # 不要修改

set(CMAKE_CXX_STANDARD      17)      # 不要修改
set(CMAKE_C_STANDARD        11)      # 不要修改

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/) # 不要修改
option(BUILD_SHARED_LIBS    ""  OFF) # 不要修改

aux_source_directory(./                     cur_src) # 不要修改
# 如果需要，可以使用aux_source_directory增加目录

include_directories(./moderator)
include_directories(./reader)
include_directories(./read_regulator)
include_directories(./disk)
include_directories(./object)
include_directories(./)
include_directories(./global)
include_directories(./writer)
include_directories(./write_regulator)
add_executable(code_craft                   ${cur_src}
        ichihan.h
        object/object.h
        disk/disk.h
        reader/reader.h
        read_regulator/read_regulator.h
        writer/writer.h
        write_regulator/write_regulator.h
        moderator/moderator.h
        global/global.h
        global/global.cpp
        object/object.cpp
        read_regulator/read_regulator.cpp
        disk/disk.cpp
        write_regulator/write_regulator.cpp
        moderator/most_greed_moderator.cpp
        reader/reader.cpp
        writer/writer.cpp
        moderator/moderator.cpp
        writer/brute_writer.cpp
        write_regulator/most_greed_write_regulator.cpp
        read_regulator/most_greed_read_regulator.cpp
        reader/read_only_greed_reader.cpp
        tag/tag.h
        tag/tag.cpp
        write_regulator/object_unit_write_regulator.cpp
        moderator/object_unit_rw_area_moderator.cpp
        writer/object_writer.cpp
) # ！！！不要修改 code_craft 名称，直接影响结果；可以根据语法在 ${cur_src} 后面追加

# 以下可以根据需要增加需要链接的库
#if (NOT WIN32)
#    target_link_libraries(code_craft  pthread  rt  m)
#endif (NOT WIN32)
