add_executable(AppSysImplClient main.cpp network.cpp xml.cpp)
target_link_libraries(AppSysImplClient pugixml ws2_32)
