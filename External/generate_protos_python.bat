setlocal

@rem enter this directory
cd /d %~dp0

set PROTOC=..\..\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe
set PLUGIN=..\..\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe

%PROTOC% --grpc_out=..\ProtoCpp --plugin=protoc-gen-grpc=%PLUGIN% TrafficAPI.proto
%PROTOC% --cpp_out=..\ProtoCpp TrafficAPI.proto

endlocal
