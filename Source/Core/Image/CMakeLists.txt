target_sources(Photoshop PUBLIC
    Image.cpp
    PgmImage.cpp
    PpmImage.cpp
)

add_subdirectory(Reader)
add_subdirectory(Writer)