Import("env")
env.Append(CPPPATH=["variants/STM32L452RET6"])
env.Append(UPLOAD_FLAGS=[
    "-c", "adapter speed 200",
    "-c", "reset_config connect_assert_srst srst_nogate",
    "-c", "sleep 200"
])