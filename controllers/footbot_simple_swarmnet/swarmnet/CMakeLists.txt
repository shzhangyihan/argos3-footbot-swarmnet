file(GLOB swarmnet_SRC
    "*.h"
    "*.cpp"
)
add_library(swarmnet SHARED ${swarmnet_SRC})
