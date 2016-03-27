set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

add_definitions( -DBOOST_NO_AUTO_PTR )

find_package(Boost REQUIRED
  filesystem
  thread
  timer
  chrono
  system
)

include_directories(${Boost_INCLUDE_DIRS})
