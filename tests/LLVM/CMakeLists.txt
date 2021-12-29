set(EXECUTABLE_OUTPUT_PATH "${Sysmel_BINARY_DIR}/llvm-codegen-tests")
set(LIBRARY_OUTPUT_PATH "${Sysmel_BINARY_DIR}/llvm-codegen-test")

set(ObjExtension ${CMAKE_CXX_OUTPUT_EXTENSION})

function(sysmel_llvm_codegen_unittest TestName)
    add_custom_command(
        OUTPUT  ${TestName}${ObjExtension}
        COMMAND sysmelc
                -g -c
                -module ${TestName}
                -o ${CMAKE_CURRENT_BINARY_DIR}/${TestName}${ObjExtension}

                ${CMAKE_CURRENT_SOURCE_DIR}/Printf.sysmel
                ${CMAKE_CURRENT_SOURCE_DIR}/BasicMacros.sysmel
                ${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting.sysmel
                ${CMAKE_CURRENT_SOURCE_DIR}/${TestName}.sysmel
        DEPENDS
            sysmelc
            ${CMAKE_CURRENT_SOURCE_DIR}/Printf.sysmel
            ${TestName}.sysmel   # Treated as relative to CMAKE_CURRENT_SOURCE_DIR
    )

    add_executable(LLVMCodegenTest${TestName} ${TestName}${ObjExtension})
    set_target_properties(LLVMCodegenTest${TestName} PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME "${TestName}")

    add_test(NAME RunLLVMCodegenTest${TestName} COMMAND LLVMCodegenTest${TestName})
    set_tests_properties(RunLLVMCodegenTest${TestName} PROPERTIES ${ARGN})
endfunction()

sysmel_llvm_codegen_unittest(HelloWorld PASS_REGULAR_EXPRESSION "Hello World")
sysmel_llvm_codegen_unittest(PrimitiveScalars FAIL_REGULAR_EXPRESSION "Failure")
