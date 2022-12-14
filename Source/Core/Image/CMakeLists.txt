target_sources(Photoshop PUBLIC
    Image.cpp
)

add_subdirectory(Dither)
add_subdirectory(Filter)
add_subdirectory(Reader)
add_subdirectory(Writer)
add_subdirectory(ColorModel)
add_subdirectory(Editors)