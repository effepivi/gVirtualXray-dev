IF (USE_GLFW)
    ADD_SUBDIRECTORY (MarchingCubes)
    ADD_SUBDIRECTORY (MonoEnergy)
    ADD_SUBDIRECTORY (Sinogram)

    IF (USE_ITK)
        ADD_SUBDIRECTORY (VolumeFromSlices)
        ADD_SUBDIRECTORY (DicomSeriesToVolume)
    ENDIF (USE_ITK)
ENDIF ()

ADD_SUBDIRECTORY (Python)
ADD_SUBDIRECTORY (Wrappers)
