#FILE(GLOB  examples  "*.cpp")

add_executable( datatem datatem.cpp )
target_link_libraries(datatem "lemmacore")

add_executable( wireantenna wireantenna.cpp )
target_link_libraries(wireantenna "lemmacore")

add_executable( DCIPElectrode  DCIPElectrode.cpp )
target_link_libraries(DCIPElectrode "lemmacore")

add_executable( digitalfiltercostrans  digitalfiltercostrans.cpp )
target_link_libraries(digitalfiltercostrans "lemmacore")

add_executable( dipolefreqsweep  dipolefreqsweep.cpp )
target_link_libraries(dipolefreqsweep "lemmacore")

add_executable( dipolesource  dipolesource.cpp )
target_link_libraries(dipolesource "lemmacore")

add_executable( edipole  edipole.cpp )
target_link_libraries(edipole "lemmacore")

add_executable( emdipearth1d  emdipearth1d.cpp )
target_link_libraries(emdipearth1d "lemmacore")

add_executable( femforward  femforward.cpp )
target_link_libraries(femforward "lemmacore")

add_executable( filter  filter.cpp )
target_link_libraries(filter "lemmacore")





