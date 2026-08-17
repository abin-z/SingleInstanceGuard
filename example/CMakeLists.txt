add_executable(main_test
    main.cpp
)
target_link_libraries(main_test PRIVATE single_instance_guard)