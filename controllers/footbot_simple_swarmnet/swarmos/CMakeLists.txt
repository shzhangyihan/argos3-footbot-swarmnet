file(GLOB swarmos_SRC
    "*.h"
    "*.cpp"
)
add_library(swarmos SHARED ${swarmos_SRC})
target_link_libraries(swarmos
  swarmnet)