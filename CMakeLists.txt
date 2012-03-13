add_library (gfxmath
  quaternion.cpp
  rotation.cpp
  matrix.cpp
  vec.cpp
  frustum.cpp
  )

if (BOOST_FOUND)
   add_executable(math_test math_test.cpp)
   target_link_libraries(math_test gfxmath ${Boost_LIBRARIES})
   add_test(MathTest ${EXECUTABLE_OUTPUT_PATH}/math_test)
endif ()
