cmake_minimum_required(VERSION 3.20)

# Test helper: writes two JSON files and runs justomize, checking stdout/exit
function(add_justomize_test name base_content overlay_content expected_output expected_exit)
    set(base_file    "${CMAKE_CURRENT_BINARY_DIR}/${name}_base.json")
    set(overlay_file "${CMAKE_CURRENT_BINARY_DIR}/${name}_overlay.json")
    set(expected_file "${CMAKE_CURRENT_BINARY_DIR}/${name}_expected.json")

    file(WRITE "${base_file}"     "${base_content}")
    file(WRITE "${overlay_file}"  "${overlay_content}")
    file(WRITE "${expected_file}" "${expected_output}")

    add_test(
        NAME "${name}"
        COMMAND ${CMAKE_COMMAND}
            -Djustomize=$<TARGET_FILE:justomize>
            -Dbase=${base_file}
            -Doverlay=${overlay_file}
            -Dexpected=${expected_file}
            -Dexit_code=${expected_exit}
            -P "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
    )
endfunction()

# Basic override
add_justomize_test(
    test_override
    [[{"a": 1, "b": 2}]]
    [[{"a": 99}]]
    [[{
  "a": 99,
  "b": 2
}
]]
    0
)

# Basic merge (new key from overlay)
add_justomize_test(
    test_merge_new_key
    [[{"a": 1}]]
    [[{"b": 2}]]
    [[{
  "a": 1,
  "b": 2
}
]]
    0
)

# Nested object merge
add_justomize_test(
    test_nested_merge
    [[{"outer": {"x": 1, "y": 2}}]]
    [[{"outer": {"x": 99}}]]
    [[{
  "outer": {
    "x": 99,
    "y": 2
  }
}
]]
    0
)

# Array override (arrays are treated as scalar – overlay replaces)
add_justomize_test(
    test_array_override
    [[{"arr": [1, 2, 3]}]]
    [[{"arr": [4, 5]}]]
    [[{
  "arr": [
    4,
    5
  ]
}
]]
    0
)

# Type mismatch should exit with non-zero
add_justomize_test(
    test_type_mismatch_error
    [[{"a": 1}]]
    [[{"a": "string"}]]
    ""
    1
)
