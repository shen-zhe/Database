set(DB_SOURCES DB.cpp Generator.cpp)
set(DB_HEADERS DB.h Runner.h Generator.h)

add_library(db_part STATIC ${DB_SOURCES} ${DB_HEADERS})