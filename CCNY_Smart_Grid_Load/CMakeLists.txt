add_library(CollectdSPACESequence STATIC src/CollectdSPACESequence.cpp)
add_library(CollectKeypadSequence STATIC src/CollectKeypadSequence.cpp)
add_library(CollectPCSequence STATIC src/CollectPCSequence.cpp)
add_library(InputSequence STATIC src/InputSequence.cpp)
add_library(LoadProfile STATIC src/LoadProfile.cpp)
add_library(ValidTokens STATIC src/ValidTokens.cpp)
add_library(RelayStateGenerator STATIC src/RelayStateGenerator.cpp)
target_link_libraries(RelayStateGenerator InputSequence ValidTokens)
add_library(Encode STATIC src/Encode.cpp)
