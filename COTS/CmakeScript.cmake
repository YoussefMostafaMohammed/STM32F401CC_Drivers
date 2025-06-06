macro(FlashAndBurn)
    set(FLASH_FLAGS "-R .eeprom -R .lock -R .signature")
    add_custom_target(
        flash
        ALL
        DEPENDS ${PROJECT_NAME}.elf
        COMMAND avr-objcopy ${FLASH_FLAGS} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
        VERBATIM
        COMMENT "Generating .hex file from .elf file"
    )

    # Modify the clean build rule to delete the .hex file
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.hex)

    # Adding a custom target for burning .hex file using avrdude
    add_custom_target(
        burn
        DEPENDS flash
        COMMAND avrdude -c ${PROGRAMMER} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex:i -B 1
        VERBATIM
        COMMENT "Burning .hex file into MCU flash"
    )
endmacro()
