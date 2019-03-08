setlocal

@rem enter this directory
cd /d %~dp0

set PROTOC=..\..\..\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe
set PLUGIN=..\..\..\vcpkg\installed\x64-windows\tools\grpc\grpc_python_plugin.exe

%PROTOC% --grpc_out=..\ProtoPy --plugin=protoc-gen-grpc=%PLUGIN% TrafficAPI.proto
%PROTOC% --python_out=..\ProtoPy TrafficAPI.proto

endlocal
